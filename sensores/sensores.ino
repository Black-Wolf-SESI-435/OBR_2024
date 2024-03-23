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
// Filtro de lerSensores:
#define FILTRO_ON true
#define alpha 0.1
// PID do Segue Linha:
// nota: A variáveis: target, kp, kd e ki são ajustadas dentro da função `pid_linha()`.
#define delta_tempo 0.01 // segundos
#define limite_correcao 200 // graus por segundo

// Pins:
//  * Sensores Reflexivos:
#define se_datapin A0
#define sd_datapin A1

// Variaveis de `calibrar()`:
int minmax_sensores[NUM_SENSORES][2] = {};

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

#if FILTRO_ON // Variáveis do low pass filter de `lerSensores()`:
    int prev_leitura = 0;
#endif

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
    leitura = (1 - alpha) * leitura + alpha * prev_leitura;
    prev_leitura = leitura;
    #endif

    return leitura;
}

// Variáveis de `pid_linha`:
int prev_erro = 0;
float iTerm = 0;

/* 
 * `pid_linha()` baseia-se no feedback da leitura dos sensores de linha para retornar
 *   a correção, tomada como uma velocidade angular (graus por segundo), por meio
 *   de um PID para ajustar as velocidades dos motores.
 */
float pid_linha(int feedback) {
    // Constantes do PID:
    static int target = 0;
    static int kp = 0;
    static int kd = 0;
    static int ki = 0;

    // Cálculo dos termos do PID:
    int erro = target - feedback;
    float dTerm = (erro - prev_erro) / delta_tempo;
    iTerm += erro * delta_tempo;
    // Cálculo da correção
    float correcao = kp * erro + kd * dTerm + ki * iTerm;
    // Limitando correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    prev_erro = erro;

    return correcao;
}

void setup() {
    #if DEBUG
        // Inicializa o monitor serial e espera até ser conectado.
        Serial.begin(BAUD);
        while (!Serial);
    #endif

    calibrar();
}

void loop() {
    int feedback_sensores = lerSensores();
    float correcao_omega_linha = pid_linha(feedback_sensores);

    #if DEBUG
        Serial.print(feedback_sensores);
        Serial.print(", ");
        Serial.print(correcao_omega_linha);
        Serial.println(", 0");
    #endif
}