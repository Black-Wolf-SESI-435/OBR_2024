// Pins:
//  * Ponte H: Motor Esquerdo (m2)
#define me_enable 4     // in3
#define me_amarelo 49   // in4
#define me_verde 51     // enB
//  * Ponte H: Motor Esquerdo (m1)
#define md_enable 5     // in1
#define md_amarelo 25   // in2
#define md_verde 27     // enA

/*
 * `setup_ponteH()` configura os pinos da ponteH como OUTPUT.
 * Deve ser chamada em setup() antes de qualquer uso de `ligarMotores()`.
*/
void setup_ponteH() {
    // Ponte H: motror esquerdo
    pinMode(me_enable, OUTPUT);
    pinMode(me_amarelo, OUTPUT);
    pinMode(me_verde, OUTPUT);
    // Ponte H: motor direito
    pinMode(md_enable, OUTPUT);
    pinMode(md_amarelo, OUTPUT);
    pinMode(md_verde, OUTPUT);
}

/*
 * `ligaMotores(int velME, int velMD)`, liga os motores dfinindo sua direção e
 *   velocidade baseado em `velME` e `velMD` inteiros entre -255 e 255.
 * É esperado que essa função seja usada com o PID das velocidades do robô, que
 *   devem ter sua correção limitada em 255.
*/
void ligarMotores(int velME, int velMD) {
    // Define a direção do motor da esquerda.
    if (velME > 0) {
        digitalWrite(me_amarelo, HIGH);
        digitalWrite(me_verde, LOW);
    } else if (velME < 0) {
        digitalWrite(me_amarelo, LOW);
        digitalWrite(me_verde, HIGH);
    } else {
        digitalWrite(me_amarelo, LOW);
        digitalWrite(me_verde, LOW);
    }
    // Define a direção do motor da direita.
    if (velMD > 0) {
        digitalWrite(md_amarelo, HIGH);
        digitalWrite(md_verde, LOW);
    } else if (velMD < 0) {
        digitalWrite(md_amarelo, LOW);
        digitalWrite(md_verde, HIGH);
    } else {
        digitalWrite(md_amarelo, LOW);
        digitalWrite(md_verde, LOW);
    }
    // Define as velocidades de cada motor:
    analogWrite(me_enable, velME);
    analogWrite(md_enable, velMD);
}
