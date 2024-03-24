// Variáveis de `pid_motor_esquerdo`:
float me_prev_erro = 0.0;
float me_iTerm = 0.0;
// Variáveis de `pid_motor_direito`:
float md_prev_erro = 0.0;
float md_iTerm = 0.0;
// Variáveis de `pid_linha`:
int linha_prev_erro = 0;
float linha_iTerm = 0.0;

/* 
 * `pid_linha()` baseia-se no feedback da leitura dos sensores de linha para retornar
 *   a correção, tomada como uma velocidade angular (graus por segundo), por meio
 *   de um PID para ajustar as velocidades dos motores.
 */
float pid_linha(int feedback) {
    // Constantes do PID:
    static int target = 0.0;
    static float kp = 0.0;
    static float kd = 0.0;
    static float ki = 0.0;
    static int limite_correcao = 200.0; // resolução de `analogWrite()`

    // Cálculo dos termos do PID:
    int erro = target - feedback;
    float dTerm = (erro - linha_prev_erro) / delta_tempo;
    linha_iTerm += erro * delta_tempo;
    // Cálculo da correção
    float correcao = kp * erro + kd * dTerm + ki * linha_iTerm;
    // Limitando correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    linha_prev_erro = erro;

    return correcao;
}

/*
 * `pid_motor_esquerdo()` usa `target`, a velocidade desejada no motor esquerdo,
 *   e a `ve` e retorna um valor de correção baseado em PID.
*/
int pid_motor_esquerdo(float target) {
    static float kp = 0;
    static float kd = 0;
    static float ki = 0;
    static int limite_correcao = 255; // resolução de `analogWrite()`

    // Cálculo dos termos do PID:
    float erro = target - ve;
    float dTerm = (erro - me_prev_erro) / delta_tempo;
    me_iTerm += erro * delta_tempo;
    // Cálculo da correção
    float correcao = kp * erro + kd * dTerm + ki * me_iTerm;
    // Limitando a correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    me_prev_erro = erro;

    return correcao;
}

/*
 * `pid_motor_direito()` usa `target`, a velocidade desejada no motor direito,
 *   e a `vd` e retorna um valor de correção baseado em PID.
*/
int pid_motor_direito(float target) {
    static float kp = 0;
    static float kd = 0;
    static float ki = 0;
    static int limite_correcao = 255; // resolução de `analogWrite()`

    // Cálculo dos termos do PID:
    float erro = target - vd;
    float dTerm = (erro - md_prev_erro) / delta_tempo;
    md_iTerm += erro * delta_tempo;
    // Cálculo da correção
    int correcao = kp * erro + kd * dTerm + ki * md_iTerm;
    // Limitando a correcão:
    correcao = constrain(correcao, -limite_correcao, limite_correcao);
    // Manutenção das variaveis.
    md_prev_erro = erro;

    return correcao;
}
