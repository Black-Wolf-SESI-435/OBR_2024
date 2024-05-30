// Configurações de Execução:
#define DEBUG        0  // coloque 1 para ver as leituras dos sensores no monitor serial.
#define SEGUE_LINHA  1  // coloque 0 para desligar o segue linha e 1 para ligar
#define CRUZAMENTO   1  // coloque 0 para desligar e 1 para ligar as curvas fechadas (90 graus)

// Pinos dos Sensores:
#define see_input A0 // s1
#define se_input  A1 // analogico E
#define sm_input  A2 // s3
#define sd_input  A3 // analogico D
#define sdd_input A4 // s5

#define se_vcc 52
#define sd_vcc 53

// Constantes:
#define cruz_cal    100
#define se_cal    400
#define sd_cal    200

#define VELOCIDADE_ME 30
#define VELOCIDADE_MD 30

#define GIRAR_DIREITA 0
#define GIRAR_ESQUERDA 1
#define NAO_GIRAR 2


void setup() {
    #if DEBUG
        Serial.begin(9600);
        while (!Serial);
    #endif

    setup_ponteh();

    pinMode(se_vcc, OUTPUT);
    pinMode(sd_vcc, OUTPUT);
    digitalWrite(se_vcc, HIGH);
    digitalWrite(sd_vcc, HIGH);
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
        Serial.print(val_see < cruz_cal);
        Serial.print(", ");
        Serial.print(val_se);
        Serial.print(" e ");
        Serial.print(val_se > se_cal);
        Serial.print(", ");
        Serial.print(val_sm);
        Serial.print(" m ");
        Serial.print(val_sm < cruz_cal);
        Serial.print(", ");
        Serial.print(val_sd);
        Serial.print(" d ");
        Serial.print(val_sd > sd_cal);
        Serial.print(", ");
        Serial.print(val_sdd);
        Serial.print(" dd ");
        Serial.print(val_sdd < cruz_cal);
        Serial.println();
    #endif
    
    // > calibragem ==> preto: True (1)
    // < calibragem ==> branco: False (0)
    val_sd = val_sd > sd_cal;
    val_se = val_se > se_cal;
    // < calibragem ==> preto: True (1)
    // > calibragem ==> branco: False (0)
    val_sdd = val_sdd < cruz_cal;
    val_see = val_see < cruz_cal;
    val_sm = val_sm < cruz_cal;


    #if CRUZAMENTO
        if (val_see || val_sdd) {
            hard_stop();
            delay(1000);

            int sentido;
            if (val_see && !val_sdd) {
                sentido = GIRAR_ESQUERDA;
            }
            else if (!val_see && val_sdd) {
                sentido = GIRAR_DIREITA;
            }
            else {
                // verde
                while (true);
            }

            girar(sentido);
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
            md_ligar(VELOCIDADE_MD);
            me_ligar(- VELOCIDADE_ME);
        }
        else if (!val_se && val_sd) {
            // direita
            md_ligar(- VELOCIDADE_MD);
            me_ligar(VELOCIDADE_ME);
        }
        else if (!(val_se || val_sd)) {
            // reto
            md_ligar(VELOCIDADE_MD);
            me_ligar(VELOCIDADE_ME);
        }
    #endif
}