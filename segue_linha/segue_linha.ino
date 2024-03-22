#define md_enable 13
#define md_amarelo 12
#define md_verde 11

#define me_amarelo 10
#define me_verde 9
#define me_enable 6

#define sd_input A0
#define se_input A2


/* Essa função liga o motor 1 controlado pelo IN_1, IN_2, EN_A
 *
 * @param vel int entre -100 e 100, representa a porcentagem
 * da velocidade onde -100 é a velocidade maxima de ré e 100
 * é a velocidade maxima de corrida
 */
void md_ligar(int vel) {
  if (vel > 0) {
    digitalWrite(md_amarelo, HIGH);
    digitalWrite(md_verde, LOW);
  } else if (vel < 0) {
    digitalWrite(md_amarelo, LOW);
    digitalWrite(md_verde, HIGH);
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
    digitalWrite(me_amarelo, HIGH);
    digitalWrite(me_verde, LOW);
  } else if (vel < 0) {
    digitalWrite(me_amarelo, LOW);
    digitalWrite(me_verde, HIGH);
  } else {
    digitalWrite(me_amarelo, LOW);
    digitalWrite(me_verde, LOW);
  }
  analogWrite(md_enable, map(vel >= 0 ? vel : -vel, 0, 100, 0, 255));
}

void setup() {
    // Coloca os pinos 9 a 13 e o 6 como OUTPUT
    int pins[6] = {13, 12, 11, 10, 9, 6 };
    for (int i = 0; i < 6; ++i) {
        pinMode(pins[i], OUTPUT);
    }
}

void loop() {
    int calibragem = 85;
    // Lê o valor dos sensores
    int val_sd = analogRead(sd_input) > calibragem;
    int val_se = analogRead(se_input) > calibragem;
    // 13 branco 85 preto 200

    if (val_se && val_sd) {
        // preto x preto: frete
        md_ligar(70);
        me_ligar(70);
    }
    if (val_se && !val_sd) {
        // preto x branco: tras, 0.4s, esquerda
        md_ligar(-60);
        me_ligar(-60);
        delay(400);
        md_ligar(70);
        me_ligar(20);
    }
    if (!val_se && val_sd) {
        // branco x preto: tras, 0.4s, direita
        md_ligar(-60);
        me_ligar(-60);
        delay(400);
        md_ligar(20);
        me_ligar(70);
    }
    if (!val_sd && !val_se) {
        // branco x branco: frete
        md_ligar(70);
        me_ligar(70);
    }
}