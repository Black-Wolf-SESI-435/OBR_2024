#define sd_input A0
#define se_input A1

// Funções da ponte h
extern void m1_ligar(int vel);
extern void m2_ligar(int vel);

int vel_e = 180;
int vel_d = 180;

int erro_0 = 0;
unsigned long erro_total = 0;
unsigned long tempo_0 = 0;
unsigned long tempo = 0;
int pid(int erro, int kp, int kd, int ki) {
  tempo = micros();

  unsigned long delta_tempo = tempo - tempo_0;
  int delta_erro = erro - erro_0;

  erro_total += erro;

  int val_pid = kp*erro + kd*delta_erro/delta_tempo + ki*(erro_total)*tempo;

  erro_0 = erro;
  tempo_0 = tempo;
  return val_pid;
}

void setup() {
  // Coloca os pinos 9 a 13 e o 6 como OUTPUT
  Serial.begin(9600);

  int pins[6] = { 13, 12, 11, 10, 9, 6 };
  for (int i = 0; i < 6; ++i)
    pinMode(pins[i], OUTPUT);

  // Liga os motores para frente
  digitalWrite(m1_amarelo, HIGH);
  digitalWrite(m1_verde, LOW);
  digitalWrite(m2_amarelo, HIGH);
  digitalWrite(m2_verde, LOW);
}

void loop() {
  // Lê o valor dos sensores
  int val_sd = analogRead(sd_input);
  int val_se = analogRead(se_input);

  int erro = val_se - val_sd;
  //                 kp kd ki
  int acc = pid(erro, 1, 3, 0);
  unsigned long delta_tempo = tempo - tempo_0;
  vel_e -= acc * delta_tempo;
  vel_d += acc * delta_tempo;

  if (vel_e < -100)
    vel_e = -100;
  else (vel_e > 100)
    vel_e = 100;
  
  if (vel_d < -100)
    vel_d = -100;
  else (vel_d > 100)
    vel_d = 100;

  m1_ligar(vel_e);
  m2_ligar(vel_d);
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
