#define m1_enable 13
#define m1_amarelo 12
#define m1_verde 11

#define m2_amarelo 10
#define m2_verde 9
#define m2_enable 6

/* Essa função liga o motor 1 controlado pelo IN_1, IN_2, EN_A
 *
 * @param vel int entre -100 e 100, representa a porcentagem
 * da velocidade onde -100 é a velocidade maxima de ré e 100
 * é a velocidade maxima de corrida
 */
void m1_ligar(int vel) {
  if (vel > 0) {
    digitalWrite(m1_amarelo, HIGH);
    digitalWrite(m1_verde, LOW);
  } else if (vel < 0) {
    digitalWrite(m1_amarelo, LOW);
    digitalWrite(m1_verde, HIGH);
  } else {
    digitalWrite(m1_amarelo, LOW);
    digitalWrite(m1_verde, LOW);
  }
  analogWrite(m1_enable, map(vel >= 0 ? vel : -vel, 0, 100, 0, 255));
}

/* Essa função liga o motor 2 controlado pelo IN_3, IN_4 e EN_B
 *
 * @param vel int entre -100 e 100, representa a porcentagem
 *  da velocidade onde -100 é a velocidade maxima de ré e 100
 *  é a velocidade maxima de corrida
 */
void m2_ligar(int vel) {
  if (vel > 0) {
    digitalWrite(m2_amarelo, HIGH);
    digitalWrite(m2_verde, LOW);
  } else if (vel < 0) {
    digitalWrite(m2_amarelo, LOW);
    digitalWrite(m2_verde, HIGH);
  } else {
    digitalWrite(m2_amarelo, LOW);
    digitalWrite(m2_verde, LOW);
  }
  analogWrite(m1_enable, map(vel >= 0 ? vel : -vel, 0, 100, 0, 255));
}