// Settings:
// DEBUG
#define DEBUG 1
#define BAUD 9600
// Low Pass Filters:
#define FILTRER_ON 1
#define DT 0.003
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
#if FILTRER_ON
    float prev_ve = 0;
    float prev_vd = 0;
#endif

void medirVelocidades() {
    long tempo = micros();
    float dt = (tempo - tempo_0)/1.0e6;

    #if FILTRER_ON
        float a = dt/(dt + DT);
        ve = (cont_me - prev_cont_me)*(1-a)/dt + prev_ve*a;
        vd = (cont_md - prev_cont_md)*(1-a)/dt + prev_vd*a;
        prev_ve = ve;
        prev_vd = vd;
    #else // FILTER_OFF
        ve = (cont_me - prev_cont_me)/dt;
        vd = (cont_md - prev_cont_md)/dt;
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
