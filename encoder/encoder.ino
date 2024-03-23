// Settings:
// Debug
#define DEBUG true
#define BAUD 9600
// Low Pass Filters:
#define FILTRO_ON true
#define alpha 0.1

// Pins:
//  * Encoder Motor Esquerdo:
#define me_encoder_A 2
#define me_encoder_B 23
//  * Encoder Motor Direito:
#define md_encoder_A 3
#define md_encoder_B 53

// Variaveis do Encoder:
long cont_me = 0;
long cont_md = 0;
// Velocidades Lineres:
float ve = 0;
float vd = 0;

// Variaveis de `medirVelocidades()`:
long tempo_0 = 0;
float prev_cont_me = 0;
float prev_cont_md = 0;
// Variáveos do low pass filter de `medirVelocidades()`
#if FILTRO_ON
    float prev_ve = 0;
    float prev_vd = 0;
#endif

void medirVelocidades() {
    long tempo = micros();
    float dt = (tempo - tempo_0)/1.0e6;

    ve = (cont_me - prev_cont_me)/dt;
    vd = (cont_md - prev_cont_md)/dt;

    #if FILTRER_ON
        // Filtra a leitura da velocidade.
        ve = (1 - alpha) * ve + alpha * prev_ve;
        vd = (1 - alpha) * vd + alpha * prev_vd;
        prev_ve = ve;
        prev_vd = vd;
    #endif

    tempo_0 = tempo;
    prev_cont_me = cont_me;
    prev_cont_md = cont_md;
}

void setup() {
    // Input
    pinMode(me_encoder_A, INPUT);
    pinMode(me_encoder_B, INPUT);
    pinMode(md_encoder_A, INPUT);
    pinMode(md_encoder_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(me_encoder_A), contadorEncoderME, RISING);
    attachInterrupt(digitalPinToInterrupt(md_encoder_A), contadorEncoderMD, RISING);

    #if DEBUG
        Serial.begin(BAUD);
        while (!Serial);
    #endif
}

void loop() {
    // Atualiza as variaveis de `ve` e `vd`.
    medirVelocidades();

    #if DEBUG
        Serial.print(ve);
        Serial.print(", ");
        Serial.print(ve);
        Serial.print(", ");
        Serial.println("500, -500");
    #endif
}

void contadorEncoderME() {
    if (digitalRead(me_encoder_B))
        cont_me--;
    else
        cont_me++;
}

void contadorEncoderMD() {
    if (digitalRead(md_encoder_B))
        cont_md--;
    else
        cont_md++;
}
