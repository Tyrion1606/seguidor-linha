#include <MX1508.h>
#include <ACS712.h>

#define PINA1 3 // Pino de controle PWM
#define PINA2 2 // Outro pino de controle que será configurado como LOW (GND)
#define PINB1 5 // Pino de controle PWM
#define PINB2 4 // Outro pino de controle que será configurado como LOW (GND)
#define NUMPWM 1
#define PWM 79
MX1508 motorA(PINA1, LOW, FAST_DECAY , NUMPWM);
MX1508 motorB(PINB1, LOW, FAST_DECAY , NUMPWM);


// Defina o pino analógico ao qual o sensor está conectado
const int analogPin = A0;

// Crie uma instância do sensor ACS712 de 5A
ACS712 sensor(analogPin, 5.0, 1023, 185); // 185 mV/A para o sensor de 5A

void setup() {
    pinMode(PINA2, OUTPUT);
    pinMode(PINB2, OUTPUT);
    digitalWrite(PINA2, LOW); // Configura PINA2 como GND
    digitalWrite(PINB2, LOW); // Configura PINB2 como GND
    
    pinMode(A0, INPUT); // Configura A0 como entrada
    pinMode(A1, INPUT); // Configura A1 como entrada
    pinMode(A2, INPUT); // Configura A2 como entrada
    pinMode(A7, INPUT); // Configura A2 como entrada
    
    Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps

      // Configuração inicial do sensor (se necessário)
  sensor.autoMidPoint(50, 10); // Ajuste automático do ponto médio
}

void loop() {
    int valA0 = digitalRead(A0);
    int valA1 = digitalRead(A1);
    int valA2 = digitalRead(A2);
    int valA7 = analogRead(A7);

      // Medição de corrente DC
  float currentDC = sensor.mA_DC(10); // Média de 10 amostras


    // Imprime os valores das portas A0, A1 e A2 no Serial Monitor
    Serial.print("A0: ");
    Serial.print(valA0);
    Serial.print("  A1: ");
    Serial.print(valA1);
    Serial.print("  A2: ");
    Serial.print(valA2);
    Serial.print("  -  A7: ");
    Serial.print(valA7);
    Serial.print("  Corrente DC: ");
    Serial.print(currentDC);
    Serial.println(" mA");
    
    if (valA0 == LOW) {
      motorA.motorGo(0);
    } else {
      motorA.motorGo(PWM);
    }

    if (valA2 == LOW) {
      motorB.motorGo(0);
    } else {
        motorB.motorGo(PWM);
    }

    // if (valA1 == LOW) {
    //   motorA.motorGo(PWM);
    //   motorB.motorGo(PWM);
    // }

    delay(200);
}
