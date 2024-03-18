#define m1_enable 13
#define m1_amarelo 12
#define m1_verde 11

#define m2_amarelo 10
#define m2_verde 9
#define m2_enable 6

#define s1_input A0
#define s2_input A1


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

void setup() {
  // Coloca os pinos 9 a 13 e o 6 como OUTPUT
  Serial.begin(9600);
  int pins[6] = { 13, 12, 11, 10, 9, 6 };
  for (int i = 0; i < 6; ++i) {
    pinMode(pins[i], OUTPUT);
  }
  digitalWrite(m1_amarelo, HIGH);
  digitalWrite(m1_verde, LOW);
  digitalWrite(m2_amarelo, HIGH);
  digitalWrite(m2_verde, LOW);
}

void loop() {
  // Lê o valor dos sensores
  int s1_leitura = analogRead(s1_input);
  int s2_leitura = analogRead(s2_input);

  Serial.println(s1_leitura);
  s1_seguir(s1_leitura);
  // Liga os dois motores

  /* Segue Linha: ...
	 * use m1_ligar e m2_ligar para ligar e desligar os motores de acordo com a velocidade de -100% a 100%.
	*/
}

void s1_seguir(int cor) {
  if (cor <= 80) {
    analogWrite(m1_enable, 255);
    analogWrite(m2_enable, 180);
  } else if (cor >= 81) {
    analogWrite(m1_enable, 180);
    analogWrite(m2_enable, 255);
  }
}
