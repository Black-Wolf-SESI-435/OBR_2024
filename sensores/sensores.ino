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

void setup() {
    #if DEBUG
        // Inicializa o monitor serial e espera até ser conectado.
        Serial.begin(BAUD);
        while (!Serial);
    #endif

    calibrar();
}
void loop() {
}