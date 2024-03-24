// Settings:
// Debug
#define DEBUG true
#define BAUD 9600
// Low Pass Filters:
#define FILTRO_ON true
#define alpha 0.1
// PIDs:
// A variáveis que tunam o pid dos motores, como target, kp, kd, ki, etc, estão
//   declaradas como estáticas dentro de cada função `pid_motor_*()`.
#define velocidade_base 200 // velocidade base para o target

// Pins:
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
float ve = 0;
float vd = 0;

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

// Variaveis de `medirVelocidades()`:
long tempo_0 = 0;
float prev_cont_me = 0;
float prev_cont_md = 0;
// Variáveos do low pass filter de `medirVelocidades()`
#if FILTRO_ON
    float prev_ve = 0;
    float prev_vd = 0;
#endif

void medirVelocidades() {
    long tempo = micros();
    float dt = (tempo - tempo_0)/1.0e6;

    ve = (cont_me - prev_cont_me)/dt;
    vd = (cont_md - prev_cont_md)/dt;

    #if FILTRER_ON
        // Filtra a leitura da velocidade.
        ve = (1 - alpha) * ve + alpha * prev_ve;
        vd = (1 - alpha) * vd + alpha * prev_vd;
        prev_ve = ve;
        prev_vd = vd;
    #endif

    tempo_0 = tempo;
    prev_cont_me = cont_me;
    prev_cont_md = cont_md;
}

// Variáveis de `pid_motor_esquerdo`:
float me_prev_erro = 0;
float me_iTerm = 0;

int pid_motor_esquerdo(float target) {
    static float kp = 0; // graus por segundo
    static float kd = 0; // graus por segundo
    static float ki = 0; // graus por segundo
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

// Variáveis de `pid_motor_direito`:
float md_prev_erro = 0;
float md_iTerm = 0;

int pid_motor_direito(float target) {
    static float kp = 0; // graus por segundo
    static float kd = 0; // graus por segundo
    static float ki = 0; // graus por segundo
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

    attachInterrupt(digitalPinToInterrupt(me_encoder_A), contadorEncoderME, RISING);
    attachInterrupt(digitalPinToInterrupt(md_encoder_A), contadorEncoderMD, RISING);

    #if DEBUG
        Serial.begin(BAUD);
        while (!Serial);
    #endif
}

void loop() {
    // Atualiza as variaveis de `ve` e `vd`.
    medirVelocidades();

    float ve_target = velocidade_base;
    float vd_target = velocidade_base;

    int correcao_me_enable = pid_motor_esquerdo(ve_target);
    int correcao_md_enable = pid_motor_direito(vd_target);

    ligarMotores(correcao_me_enable, correcao_md_enable);

    #if DEBUG
        Serial.print(ve);
        Serial.print(", ");
        Serial.print(ve_target);
        Serial.print(", ");
        Serial.print(vd);
        Serial.print(", ");
        Serial.print(vd_target);
        Serial.print(", ");
        Serial.println(velocidade_base);
    #endif
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
