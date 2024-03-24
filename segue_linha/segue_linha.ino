// Settings:
// Debug:
// Full Debug:
#define DEBUG true // Mostra as velocidades e outras informações durante a execução
#define BAUD 9600
// Medindo delta_tempo:.
#define MEDIR_DT true // Mostra o valor de delta_tempo em μs (automaticamente desabilita o DEBUG)
// Low Pass Filters:
#define FILTRO_ON true
#define ALPHA 0.1
// Constantes:
#define delta_tempo 0.01 // tempo do ciclo do loop (segundos)
#define VELOCIDADE_BASE 200 // velocidade padrão dos motores (graus por segundo)

extern float ve;
extern float vd;

void setup() {
    // pinMode ponte H
    setup_ponteH();
    // pinMode: INPUT
    setup_encoder();

    // Inicia o programa desligando os motores.
    ligarMotores(0, 0);
    // Inicia a calibragem dos sensores.
    calibrar();

    #if DEBUG || MEDINDO_DT
        // Inicializa o monitor serial e espera até ser conectado.
        Serial.begin(BAUD);
        while (!Serial);
    #endif
}

#if MEDIR_DT
    long prev_tempo;
#endif
void loop() {
    // Atualiza as variaveis de `ve` e `vd`.
    medirVelocidades();

    int feedback_sensores = lerSensores();
    float v_angular_target = pid_linha(feedback_sensores);

    float ve_target = VELOCIDADE_BASE - v_angular_target;
    float vd_target = VELOCIDADE_BASE - v_angular_target;

    int correcao_me_enable = pid_motor_esquerdo(ve_target);
    int correcao_md_enable = pid_motor_direito(vd_target);

    ligarMotores(correcao_me_enable, correcao_md_enable);
    
    #if MEDIR_DT
        // Medida do delta_tempo:
        long tempo = micros();
        Serial.println(tempo - prev_tempo); // delta_tempo
        // Manutenção das variáveis
        prev_tempo = tempo;
    #elif DEBUG
        Serial.print((ve + vd) / 2.0);  // velocidade linear medida pelo encoder
        Serial.print(", ");
        Serial.print(ve);               // velocidade medida no motor esquerdo
        Serial.print(", ");
        Serial.print(ve_target);        // velocidade target no motor esquerdo
        Serial.print(", ");
        Serial.print(vd);               // velocidade medida no motor direito
        Serial.print(", ");
        Serial.print(vd_target);        // velocidade target no motor direito
        Serial.print(", ");
        Serial.print(VELOCIDADE_BASE);  // velocidade linear esperada
        Serial.print(", ");
        Serial.print(feedback_sensores);// erro medido nos sensores
        Serial.print(", ");
        Serial.print(v_angular_target); // velocidade angular target
        Serial.println(", 0");
    #endif
}