// Configurações de Execução:
#define DEBUG        0  // coloque 1 para ver as leituras dos sensores no monitor serial.
#define SEGUE_LINHA  1  // coloque 0 para desligar o segue linha e 1 para ligar
#define CRUZAMENTO   1  // coloque 0 para desligar e 1 para ligar as curvas fechadas (90 graus)

// Pinos dos Sensores:
#define see_input A0 // s1 (marrom)
#define se_input  A1 // analogico E (amarelo)
#define sm_input  A2 // s3 (verde)
#define sd_input  A3 // analogico D (azul)
#define sdd_input A4 // s5 (roxo)

#define se_vcc 13
#define sd_vcc 12
#define sazul_gnd 11
#define botao_calibragem 10

// Calibragem:
#define DELAY_CALIBRAGEM 1000
#define NUM_SENSORES 5
#define NUM_TESTES 512

// Velocidades:
#define VELOCIDADE_ME 35
#define VELOCIDADE_MD 35

// Aliases:
#define GIRAR_ESQUERDA -1
#define NAO_GIRAR 0
#define GIRAR_DIREITA 1

long calibragem[NUM_SENSORES];

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
    #endif

    // Espera pela confirmação do botão.
    while(digitalRead(botao_calibragem));

    // Calibra o min de cada sensor no branco.
    for (int sensor = 0; sensor < NUM_SENSORES; sensor++) {
        calibragem[sensor] = 0;
        for (int teste = 0; teste < NUM_TESTES; ++teste) {
            calibragem[sensor] += analogRead(sensor);
        }
    }

    #if DEBUG
        Serial.println("    Colorque os sensores no preto.");
    #endif

    // Espera pela confirmação do botão.
    delay(DELAY_CALIBRAGEM);
    while(digitalRead(botao_calibragem));

    // Calibra o max de cada sensor no preto.
    for (int sensor = 0; sensor < NUM_SENSORES; sensor++) {
        for (int teste = 0; teste < NUM_TESTES; ++teste) {
            calibragem[sensor] += analogRead(sensor);
        }
        calibragem[sensor] = calibragem[sensor] / (2*NUM_TESTES);
    }

    #if DEBUG
        Serial.println("Fim da Calibragem.");
        for (int sensor = 0; sensor < NUM_SENSORES; sensor++) {
            Serial.print(sensor);
            Serial.print(": ");
            Serial.println(calibragem[sensor]);
        }
    #endif
    delay(DELAY_CALIBRAGEM);
}

void setup() {
    #if DEBUG
        Serial.begin(9600);
    #endif

    setup_ponteh();

    pinMode(se_vcc, OUTPUT);
    pinMode(sd_vcc, OUTPUT);
    pinMode(sazul_gnd, OUTPUT);

    pinMode(botao_calibragem, INPUT_PULLUP);

    digitalWrite(se_vcc, HIGH);
    digitalWrite(sd_vcc, HIGH);
    digitalWrite(sazul_gnd, LOW);

    calibrar();
}

void loop() {
    // Lê o valor dos sensores:
    // sensores do cruzamento
    int val_see = analogRead(see_input);
    int val_sm = analogRead(sm_input);
    int val_sdd = analogRead(sdd_input);
    // sensores da linha
    int val_se = analogRead(se_input);
    int val_sd = analogRead(sd_input);

    #if DEBUG
        Serial.print(val_see);
        Serial.print(" ee ");
        Serial.print(val_see < calibragem[0]);
        Serial.print(", ");
        Serial.print(val_se);
        Serial.print(" e ");
        Serial.print(val_se > calibragem[1]);
        Serial.print(", ");
        Serial.print(val_sm);
        Serial.print(" m ");
        Serial.print(val_sm < calibragem[2]);
        Serial.print(", ");
        Serial.print(val_sd);
        Serial.print(" d ");
        Serial.print(val_sd > calibragem[3]);
        Serial.print(", ");
        Serial.print(val_sdd);
        Serial.print(" dd ");
        Serial.print(val_sdd < calibragem[4]);
        Serial.println();
    #endif

    // Sensores do cruzamento usam > e sensores da linha usam <
    // 1 => no preto
    // 0 => no branco
    val_see = val_see < calibragem[0];
    val_se = val_se   > calibragem[1];
    val_sm = val_sm   < calibragem[2];
    val_sd = val_sd   > calibragem[3];
    val_sdd = val_sdd < calibragem[4];


    #if CRUZAMENTO
        if (val_see || val_sdd) {
            // Para
            me_ligar(-70);
            md_ligar(-70);
            delay(30);
            me_ligar(0);
            md_ligar(0);
            delay(300);

            // Avança até centralizar com a curva
            me_ligar(VELOCIDADE_MD);
            md_ligar(VELOCIDADE_ME);
            delay(300);

            val_sm = analogRead(sm_input) < calibragem[2];

            delay(500);
            me_ligar(-70);
            md_ligar(-70);
            delay(30);
            me_ligar(0);
            md_ligar(0);
            delay(300);

            int caminho = (val_see<<2) + (val_sm<<1) + val_sdd;

            switch (caminho) {
                case 0b100:
                    girar(GIRAR_ESQUERDA);
                    delay(1000);

                    girar(GIRAR_DIREITA);
                    delay(30);
                    md_ligar(0);
                    me_ligar(0);
                    delay(300);
                    break;
                case 0b001:
                    girar(GIRAR_DIREITA);
                    delay(1000);

                    girar(GIRAR_ESQUERDA);
                    delay(30);
                    md_ligar(0);
                    me_ligar(0);
                    delay(300);
                    break;
            }
        }
    #endif

    #if SEGUE_LINHA
        // Segue Linha
        if (val_se && val_sd) {
            // reto
            md_ligar(VELOCIDADE_MD);
            me_ligar(VELOCIDADE_ME);
        }
        else if (val_se && !val_sd) {
            // esquerda
            girar(GIRAR_ESQUERDA);
        }
        else if (!val_se && val_sd) {
            // direita
            girar(GIRAR_DIREITA);
        }
        else if (!(val_se || val_sd)) {
            // reto
            md_ligar(VELOCIDADE_MD);
            me_ligar(VELOCIDADE_ME);
        }
    #endif
}