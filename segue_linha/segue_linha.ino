// Configurações de Execução:
#define DEBUG        0  // coloque 1 para ver as leituras dos sensores no monitor serial.
#define TESTE_SENSOR 0  // debug precisa estar ligado: coloque 1 para testar os sensores 0 para nn testar
#define SEGUE_LINHA  1  // coloque 0 para desligar o segue linha e 1 para ligar
#define CRUZAMENTO   0  // coloque 0 para desligar e 1 para ligar as curvas fechadas (90 graus)

// Pinos dos Sensores:
#define sd_input A0
#define se_input A1
#define sd2_input A2
#define se2_input A3
#define st_input A7

// Constantes:
#define CALIBRAGEM_1  200
#define CALIBRAGEM_2  300
#define VELOCIDADE    60
#define VEL_AJUSTE_ME 15


void setup() {
    setup_ponteh();

    #if DEBUG
        Serial.begin(9600);
    #endif
}

void loop() {
    // Lê o valor dos sensores
    int val_sd = analogRead(sd_input);
    int val_se = analogRead(se_input);
    int val_sd2 = analogRead(sd2_input);
    int val_se2 = analogRead(se2_input);

    #if DEBUG
        #if TESTE_SENSOR
            int val_st = analogRead(st_input);
            Serial.print("0, 1023, ");
            Serial.print(val_st);
            Serial.print(" t ");
            Serial.print(val_st > CALIBRAGEM_1);
            Serial.print(", ");
        #endif
        Serial.print(val_se);
        Serial.print(" e ");
        Serial.print(val_se > CALIBRAGEM_1);
        Serial.print(", ");
        Serial.print(val_sd);
        Serial.print(" d ");
        Serial.print(val_sd > CALIBRAGEM_1);
        Serial.print(", ");
        Serial.println(CALIBRAGEM_1);
    #endif
    
    // > calibragem ==> preto: True (1)
    // < calibragem ==> branco: False (0)
    val_sd = val_sd > CALIBRAGEM_1;
    val_se = val_se > CALIBRAGEM_1;
    val_sd2 = val_sd2 > CALIBRAGEM_1;
    val_se2 = val_se2 > CALIBRAGEM_2;

    #if CRUZAMENTO
        // Código do Cruzamento:
    #endif

    #if SEGUE_LINHA
        // Segue Linha
        if (val_se && val_sd) {
            // reto
            md_ligar(VELOCIDADE);
            me_ligar(VELOCIDADE - VEL_AJUSTE_ME);
        }
        else if (val_se && !val_sd) {
            // esquerda
            md_ligar(VELOCIDADE);
            me_ligar(- VELOCIDADE + VEL_AJUSTE_ME);
        }
        else if (!val_se && val_sd) {
            // direita
            md_ligar(-VELOCIDADE);
            me_ligar(VELOCIDADE - VEL_AJUSTE_ME);
        }
        else if (!(val_se || val_sd)) {
            // reto
            md_ligar(VELOCIDADE);
            me_ligar(VELOCIDADE - VEL_AJUSTE_ME);
        }
    #endif
}