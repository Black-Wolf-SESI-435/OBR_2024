// Settings:
// Debug:
#define DEBUG true
#define BAUD 9600
// Calibragem:
#define NUM_TESTES 500
#define NUM_SENSORES 2
#define THRESHOLD_REF 85
#define DELAY_CALIBRAGEM 1000 // 1 SEGUNDO
#define MIN_OUT 0
#define MAX_OUT 200
// Low Pass Filters:
#define FILTRO_ON true
#define ALPHA 0.1
// PIDs:
// A variáveis que tunam os PIDs, como target, kp, kd, ki, etc, estão declaradas
//   como estáticas dentro de cada função `pid_motor_*()`.
// Outras constantes:
#define VELOCIDADE_BASE 200 // velocidade padrão dos motores (graus por segundo)

// Pins:
//  * Sensores Reflexivos:
#define se_datapin A0
#define sd_datapin A1
//  * Ponte H: Motor Esquerdo (m2)
#define me_enable 4     // in3
#define me_amarelo 49   // in4
#define me_verde 51     // enB
//  * Ponte H: Motor Esquerdo (m1)
#define md_enable 5     // in1
#define md_amarelo 25   // in2
#define md_verde 27     // enA
//  * Encoder Motor Esquerdo:
#define me_encoder_A 2
#define me_encoder_B 23
//  * Encoder Motor Direito:
#define md_encoder_A 3
#define md_encoder_B 53

// Variaveis do Encoder:
long cont_me = 0;
long cont_md = 0;
// Velocidades Lineres:
float ve = 0.0;
float vd = 0.0;
// Variáveis do low pass filter
#if FILTRO_ON
    //  * `medirVelocidades()`:
    float prev_ve = 0;
    float prev_vd = 0;
    //  * `lerSensores()`:
    int prev_leitura = 0;
#endif
// Variaveis de `medirVelocidades()`:
long tempo_0 = 0;
float prev_cont_me = 0.0;
float prev_cont_md = 0.0;
// Variáveis de `pid_motor_esquerdo`:
float me_prev_erro = 0.0;
float me_iTerm = 0.0;
// Variáveis de `pid_motor_direito`:
float md_prev_erro = 0.0;
float md_iTerm = 0.0;
// Variaveis de `calibrar()`:
int minmax_sensores[NUM_SENSORES][2];
// Variáveis de `pid_linha`:
int linha_prev_erro = 0;
float linha_iTerm = 0.0;

/*
 * `ligaMotores(int velME, int velMD)`, liga os motores dfinindo sua direção e
 *   velocidade baseado em `velME` e `velMD` inteiros entre -255 e 255.
 * É esperado que essa função seja usada com o PID das velocidades do robô, que
 *   devem ter sua correção limitada em 255.
*/
void ligarMotores(int velME, int velMD) {
    // Define a direção do motor da esquerda.
    if (velME > 0) {
        digitalWrite(me_amarelo, HIGH);
        digitalWrite(me_verde, LOW);
    } else if (velME < 0) {
        digitalWrite(me_amarelo, LOW);
        digitalWrite(me_verde, HIGH);
    } else {
        digitalWrite(me_amarelo, LOW);
        digitalWrite(me_verde, LOW);
    }
    // Define a direção do motor da direita.
    if (velMD > 0) {
        digitalWrite(md_amarelo, HIGH);
        digitalWrite(md_verde, LOW);
    } else if (velMD < 0) {
        digitalWrite(md_amarelo, LOW);
        digitalWrite(md_verde, HIGH);
    } else {
        digitalWrite(md_amarelo, LOW);
        digitalWrite(md_verde, LOW);
    }
    // Define as velocidades de cada motor:
    analogWrite(me_enable, velME);
    analogWrite(md_enable, velMD);
}

void medirVelocidades() {
    long tempo = millis();
    float dt = (tempo - tempo_0)/1.0e3;

    ve = (cont_me - prev_cont_me)/dt;
    vd = (cont_md - prev_cont_md)/dt;

    #if FILTRER_ON
        // Filtra a leitura da velocidade.
        ve = (1 - ALPHA) * ve + ALPHA * prev_ve;
        vd = (1 - ALPHA) * vd + ALPHA * prev_vd;
        prev_ve = ve;
        prev_vd = vd;
    #endif
    // Manutenção das variáveis:
    tempo_0 = tempo;
    prev_cont_me = cont_me;
    prev_cont_md = cont_md;
}

int pid_motor_esquerdo(float target) {
    static float kp = 0;
    static float kd = 0;
    static float ki = 0;
    static float delta_tempo = 0.01; // segundos
    static int limite_correcao = 255; // resolução de `analogWrite()`

    // Cálculo dos termos do PID:
    float erro = target - ve;
    float dTerm = (erro - me_prev_erro) / delta_tempo;
    me_iTerm += erro * delta_tempo;
    // Cálculo da correção
    float correcao = kp * erro + kd * dTerm + ki * me_iTerm;
    // Limitando a correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    me_prev_erro = erro;

    return correcao;
}

int pid_motor_direito(float target) {
    static float kp = 0;
    static float kd = 0;
    static float ki = 0;
    static float delta_tempo = 0.01; // segundos
    static int limite_correcao = 255; // resolução de `analogWrite()`

    // Cálculo dos termos do PID:
    float erro = target - vd;
    float dTerm = (erro - md_prev_erro) / delta_tempo;
    md_iTerm += erro * delta_tempo;
    // Cálculo da correção
    int correcao = kp * erro + kd * dTerm + ki * md_iTerm;
    // Limitando a correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    md_prev_erro = erro;

    return correcao;
}

void contadorEncoderME() {
    if (digitalRead(me_encoder_B))
        cont_me--;
    else
        cont_me++;
}

void contadorEncoderMD() {
    if (digitalRead(md_encoder_B))
        cont_md--;
    else
        cont_md++;
}

/*
 * `calibrar()` usa as cosntantes `NUM_SENSORES` e `NUM_TESTES` para encontrar o
 *   máximo e mínimo de cada sensor.
 * Para isso ligue os sensores de forma que eles preencham os pinos analógicos
 *   em ordem crescende. Com isso, não precisa ser atualizado caso o numero de
 *   sensores mude.
 */
void calibrar() {
    #if DEBUG
        Serial.println("Calibrando sensores:");
        Serial.println("    Colorque os sensores no branco.");
        delay(DELAY_CALIBRAGEM);
    #endif

    bool no_branco = false;
    while (!no_branco)
        for (int sensor = 0; sensor < NUM_SENSORES; sensor++)
            no_branco &= analogRead(sensor) < THRESHOLD_REF;
    // Calibra o min de cada sensor no branco.
    for (int sensor = 0; sensor < NUM_SENSORES; sensor++)
    {
        minmax_sensores[sensor][0] = 1023;
        for (int teste = 0; teste < NUM_TESTES; ++teste)
        {
            int leitura = analogRead(sensor);

            if (leitura < minmax_sensores[sensor][0])
                minmax_sensores[sensor][0] = leitura;
        }
    }

    #if DEBUG
        Serial.println("    Colorque os sensores no preto.");
        delay(DELAY_CALIBRAGEM);
    #endif

    bool no_preto = false;
    while (!no_preto)
        for (int sensor = 0; sensor < NUM_SENSORES; sensor++)
            no_preto &= analogRead(sensor) > THRESHOLD_REF;

    // Calibra o max de cada sensor no preto.
    for (int sensor = 0; sensor < NUM_SENSORES; sensor++)
    {
        minmax_sensores[sensor][1] = 0;
        for (int teste = 0; teste < NUM_TESTES; ++teste)
        {
            int leitura = analogRead(sensor);

            if (leitura > minmax_sensores[sensor][1])
                minmax_sensores[sensor][1] = leitura;
        }
    }

    #if DEBUG
        Serial.println("Fim da Calibragem.");
    #endif
}

/*
 * `lerSensores()` retorna a leitura dos senroes, sendo um número de -400 á +400
 * Deve ser atualizado caso o número de sensroes mude.
 */
int lerSensores() {
    // Le os pinos analógicos dos sensores
    int se_read = analogRead(A0);
    int sd_read = analogRead(A1);
    // Normaliza as leituras usando a calibragem
    sd_read = map(se_read, minmax_sensores[0][0], minmax_sensores[0][1], MIN_OUT, MAX_OUT);
    sd_read = map(sd_read, minmax_sensores[1][0], minmax_sensores[1][1], MIN_OUT, MAX_OUT);
    // Retorna o calculo da posição relativa á linha

    int leitura = se_read - sd_read;
    
    #if FILTRO_ON
    // Low pass filter:
    leitura = (1 - ALPHA) * leitura + ALPHA * prev_leitura;
    prev_leitura = leitura;
    #endif

    return leitura;
}

/* 
 * `pid_linha()` baseia-se no feedback da leitura dos sensores de linha para retornar
 *   a correção, tomada como uma velocidade angular (graus por segundo), por meio
 *   de um PID para ajustar as velocidades dos motores.
 */
float pid_linha(int feedback) {
    // Constantes do PID:
    static int target = 0.0;
    static float kp = 0.0;
    static float kd = 0.0;
    static float ki = 0.0;
    static float delta_tempo = 0.01; // segundos
    static int limite_correcao = 200.0; // resolução de `analogWrite()`

    // Cálculo dos termos do PID:
    int erro = target - feedback;
    float dTerm = (erro - linha_prev_erro) / delta_tempo;
    linha_iTerm += erro * delta_tempo;
    // Cálculo da correção
    float correcao = kp * erro + kd * dTerm + ki * linha_iTerm;
    // Limitando correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    linha_prev_erro = erro;

    return correcao;
}

void setup() {
    // pinMode: OUTPUT
    // Ponte H: motror esquerdo
    pinMode(me_enable, OUTPUT);
    pinMode(me_amarelo, OUTPUT);
    pinMode(me_verde, OUTPUT);
    // Ponte H: motor direito
    pinMode(md_enable, OUTPUT);
    pinMode(md_amarelo, OUTPUT);
    pinMode(md_verde, OUTPUT);
    // pinMode: INPUT
    // Encoder: motor esquerdo
    pinMode(me_encoder_A, INPUT);
    pinMode(me_encoder_B, INPUT);
    // Encoder: motor direito
    pinMode(md_encoder_A, INPUT);
    pinMode(md_encoder_B, INPUT);

    // Inicia o programa desligando os motores.
    ligarMotores(0, 0);
    // Inicia a calibragem dos sensores.
    calibrar();

    attachInterrupt(digitalPinToInterrupt(me_encoder_A), contadorEncoderME, RISING);
    attachInterrupt(digitalPinToInterrupt(md_encoder_A), contadorEncoderMD, RISING);

    #if DEBUG
        // Inicializa o monitor serial e espera até ser conectado.
        Serial.begin(BAUD);
        while (!Serial);
    #endif
}

void loop() {
    // Atualiza as variaveis de `ve` e `vd`.
    medirVelocidades();

    int feedback_sensores = lerSensores();
    float v_angular_target = pid_linha(feedback_sensores);

    float ve_target = VELOCIDADE_BASE - v_angular_target;
    float vd_target = VELOCIDADE_BASE - v_angular_target;

    int correcao_me_enable = pid_motor_esquerdo(ve_target);
    int correcao_md_enable = pid_motor_direito(vd_target);

    ligarMotores(correcao_me_enable, correcao_md_enable);

    #if DEBUG
        Serial.print((ve + vd) / 2.0);  // velocidade linear medida pelo encoder
        Serial.print(", ");
        Serial.print(ve);               // velocidade medida no motor esquerdo
        Serial.print(", ");
        Serial.print(ve_target);        // velocidade target no motor esquerdo
        Serial.print(", ");
        Serial.print(vd);               // velocidade medida no motor direito
        Serial.print(", ");
        Serial.print(vd_target);        // velocidade target no motor direito
        Serial.print(", ");
        Serial.print(VELOCIDADE_BASE);  // velocidade linear esperada
        Serial.print(", ");
        Serial.print(feedback_sensores);// erro medido nos sensores
        Serial.print(", ");
        Serial.print(v_angular_target); // velocidade angular target
        Serial.println(", 0");
    #endif
}