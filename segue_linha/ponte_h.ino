// Pinos da ponte H

#define md_enable 4
#define md_amarelo 22
#define md_verde 23

#define me_amarelo 24
#define me_verde 25
#define me_enable 7

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

/* Seta os pinos da ponte H como OUTPUT e desliga os motores.
 * Deve ser chamado no `setup` e apenas uma vez.
 */
void setup_ponteh() {
    // Seta os pinos da ponte H como OUTPUT:
    pinMode(md_enable, OUTPUT);
    pinMode(md_amarelo, OUTPUT);
    pinMode(md_verde, OUTPUT);
    pinMode(me_enable, OUTPUT);
    pinMode(me_amarelo, OUTPUT);
    pinMode(me_verde, OUTPUT)

    me_ligar(0);
    md_ligar(0);
}