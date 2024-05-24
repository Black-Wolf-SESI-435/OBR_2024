#define DEBUG 1 // coloque 1 para ver as leituras dos sensores no monitor serial.
#define TESTE_SENSOR 1 // debug precisa estar ligado: coloque 1 para testar os sensores 0 para nn testar
#define SEGUE_LINHA 0 // coloque 0 para desligar o segue linha e 1 para ligar
#define CRUZAMENTO 1

#define md_enable 4
#define md_amarelo 22
#define md_verde 23

#define me_amarelo 24
#define me_verde 25
#define me_enable 7

#define sd_input A0
#define se_input A1
#define sd2_input A2
#define se2_input A3
#define st_input A7

// CALIBRAGEM
#define CALIBRAGEM_1 200
#define CALIBRAGEM_2 300
#define VELOCIDADE 70
#define VEL_AJUSTE_ME 15

/* Essa função liga o motor 1 controlado pelo IN_1, IN_2, EN_A
 *
 * @param vel int entre -100 e 100, representa a porcentagem
 * da velocidade onde -100 é a velocidade maxima de ré e 100
 * é a velocidade maxima de corrida
 */
void md_ligar(int vel) {
  if (vel > 0) {
    digitalWrite(md_amarelo, LOW);
    digitalWrite(md_verde, HIGH);
  } else if (vel < 0) {
    digitalWrite(md_amarelo, HIGH);
    digitalWrite(md_verde, LOW);
  } else {
    digitalWrite(md_amarelo, LOW);
    digitalWrite(md_verde, LOW);
  }
  analogWrite(md_enable, map(vel >= 0 ? vel : -vel, 0, 100, 0, 255));
}

/* Essa função liga o motor 2 controlado pelo IN_3, IN_4 e EN_B
 *
 * @param vel int entre -100 e 100, representa a porcentagem
 *  da velocidade onde -100 é a velocidade maxima de ré e 100
 *  é a velocidade maxima de corrida
 */
void me_ligar(int vel) {
  if (vel > 0) {    
    digitalWrite(me_amarelo, LOW);
    digitalWrite(me_verde, HIGH);
  } else if (vel < 0) {
    digitalWrite(me_amarelo, HIGH);
    digitalWrite(me_verde, LOW);
  } else {
    digitalWrite(me_amarelo, LOW);
    digitalWrite(me_verde, LOW);
  }
  analogWrite(me_enable, map(vel >= 0 ? vel : -vel, 0, 100, 0, 255));
}

void setup() {
    // Seta os pinos da ponte H como OUTPUT:
    pinMode(md_enable, OUTPUT);
    pinMode(md_amarelo, OUTPUT);
    pinMode(md_verde, OUTPUT);
    pinMode(me_enable, OUTPUT);
    pinMode(me_amarelo, OUTPUT);
    pinMode(me_verde, OUTPUT);

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
    if (val_sd2 || val_se2) {
        // Cruzamento
        if (val_se2 && !val_sd2) {
            // Esquerda
            // GIRA ate se estar branco
            me_ligar(0);
            md_ligar(VELOCIDADE+20);
            delay(1500);
        }
        else if (!val_se2 && val_sd2) {
            // Direita
            me_ligar(VELOCIDADE - VEL_AJUSTE_ME+20);
            md_ligar(0);
            delay(1500);
        }
        else if (val_se2 && val_se2) {
            // Frente
        }
        me_ligar(0);
        md_ligar(0);
    }
#endif

#if SEGUE_LINHA
    // Segue Linha
    if (!(val_se || val_sd)) {
        // reto
        md_ligar(VELOCIDADE);
        me_ligar(VELOCIDADE - VEL_AJUSTE_ME);
    }
    if (!val_se && val_sd) {
        // direita
        md_ligar(0);
        me_ligar(VELOCIDADE);
    }
    if (val_se && !val_sd) {
        // esquerda
        md_ligar(VELOCIDADE);
        me_ligar(0);
    }
#endif
}