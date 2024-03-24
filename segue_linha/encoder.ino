// Pins:
//  * Encoder Motor Esquerdo:
#define me_encoder_A 2
#define me_encoder_B 23
//  * Encoder Motor Direito:
#define md_encoder_A 3
#define md_encoder_B 53

// Rotação medida no motor esquerdo:
float ve = 0.0;
// Rotação medida no motor direito:
float vd = 0.0;
// Variaveis do Encoder:
long cont_me = 0;
long cont_md = 0;
// Variaveis de `medirVelocidades()`:
float prev_cont_me = 0.0;
float prev_cont_md = 0.0;
// Low pass filter de  `medirVelocidades()`:
#if FILTRO_ON
    float prev_ve = 0;
    float prev_vd = 0;
#endif

/*
 * `setup_encoder()` configura os pinos do encoder como INPUT.
 * Deve ser chamada em setup() ou a leitura das velocidades não irá funcionar.
*/
void setup_encoder() {
    // Encoder: motor esquerdo
    pinMode(me_encoder_A, INPUT);
    pinMode(me_encoder_B, INPUT);
    // Encoder: motor direito
    pinMode(md_encoder_A, INPUT);
    pinMode(md_encoder_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(me_encoder_A), contadorEncoderME, RISING);
    attachInterrupt(digitalPinToInterrupt(md_encoder_A), contadorEncoderMD, RISING);
}

/*
 * `medirVelocidades` usa as leituras do encoder para mensurar a rotação de cada
 * motor, essas velocidades são guardadas nas variáveis globais `ve` e `vd`.
*/
void medirVelocidades() {
    ve = (cont_me - prev_cont_me)/delta_tempo;
    vd = (cont_md - prev_cont_md)/delta_tempo;

    #if FILTRER_ON
        // Filtra a leitura da velocidade.
        ve = (1 - ALPHA) * ve + ALPHA * prev_ve;
        vd = (1 - ALPHA) * vd + ALPHA * prev_vd;
        prev_ve = ve;
        prev_vd = vd;
    #endif
    // Manutenção das variáveis:
    prev_cont_me = cont_me;
    prev_cont_md = cont_md;
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
