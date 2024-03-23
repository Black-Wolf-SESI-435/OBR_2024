// Settings:
// PID do Segue Linha:
// nota: A variáveis: target, kp, kd e ki são ajustadas dentro da função `pid_linha()`.
#define delta_tempo 0.01 // segundos
#define limite_correcao 200 // graus por segundo

// Variáveis de `pid_linha`:
int prev_erro = 0;
float iTerm = 0;

/* 
 * `pid_linha()` baseia-se no feedback da leitura dos sensores de linha para retornar
 *   A correção tomada como uma velocidade angular por meio de um PID para ajustar
 *   as velocidades dos motores.
 */
float pid_linha(int feedback) {
    // Constantes do PID:
    static int target = 0;
    static int kp = 0;
    static int kd = 0;
    static int ki = 0;

    // Cálculo dos termos do PID:
    int erro = target - feedback;
    float dTerm = (erro - prev_erro) / delta_tempo;
    iTerm += erro * delta_tempo;
    // Cálculo da correção
    float correcao = kp * erro + kd * dTerm + ki * iTerm;
    // Limitando correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    prev_erro = erro;

    return correcao;
}

void setup() {
}

void loop() {
}
