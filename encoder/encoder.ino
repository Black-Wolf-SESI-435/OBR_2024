// Encoder:
#define me_encoder_A 2
#define me_encoder_B 23
long cont_me = 0;
float ve = 0;

#define md_encoder_A 3
#define md_encoder_B 53
long cont_md = 0;
float vd = 0;

long tempo_0 = 0;
float prev_ve = 0;
float prev_vd = 0;
float prev_cont_me = 0;
float prev_cont_md = 0;
float updateVelocidades() {
    long tempo = micros();
    float dt = (tempo - tempo_0)/1.0e6;

    // Filtro
    float a = dt/(dt + 0.003);
    ve = (cont_me - prev_cont_me)*(1-a)/dt + prev_ve*a;
    vd = (cont_md - prev_cont_md)*(1-a)/dt + prev_vd*a;
    prev_ve = ve;
    prev_vd = vd;
    // ---

    // Sem Filtro
    //ve = (cont_me - prev_cont_me)/dt + prev_ve;
    //vd = (cont_md - prev_cont_md)/dt + prev_vd;
    // ---

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

    Serial.begin(9600);
    while (!Serial);
}

void loop() {
    updateVelocidades();

    Serial.print(ve);
    Serial.print(", ");
    Serial.print(ve);
    Serial.print(", ");
    Serial.println("500, -500");
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
