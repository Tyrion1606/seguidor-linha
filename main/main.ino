#include <MX1508.h>

// Configuração dos Motores
#define PINA1 5   // Pino de controle PWM para motor A (direita)
#define PINA2 6   // Pino de controle PWM para motor A (direita)
#define PINB1 9   // Pino de controle PWM para motor B (esquerda)
#define PINB2 10  // Pino de controle PWM para motor B (esquerda)
#define NUMPWM 2
#define PWM 100 // Velocidade PWM desejada

MX1508 motorA(PINA1, PINA2, FAST_DECAY, NUMPWM);
MX1508 motorB(PINB1, PINB2, FAST_DECAY, NUMPWM);

// Configuração dos Sensores IR
#define IR_PIN_RIGHT 2
#define IR_PIN_LEFT 3
#define CENTER 4

bool turning_FLAG = false;

void setup() {
  pinMode(CENTER, INPUT);
  pinMode(IR_PIN_RIGHT, INPUT);
  pinMode(IR_PIN_LEFT, INPUT);

  // Motor pins
  pinMode(PINA1, OUTPUT);
  pinMode(PINA2, OUTPUT);
  pinMode(PINB1, OUTPUT);
  pinMode(PINB2, OUTPUT);

  // Configurações das interrupções
  attachInterrupt(digitalPinToInterrupt(IR_PIN_RIGHT), rightSensorISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR_PIN_LEFT), leftSensorISR, CHANGE);
}

void loop() {
  if (digitalRead(CENTER)) {
    motorA.motorGo(PWM);
    motorB.motorGo(PWM);
  }
}

void rightSensorISR() {
  motorA.motorGo(0);
  motorB.motorGo(PWM);
}

void leftSensorISR() {
  motorA.motorGo(PWM);
  motorB.motorGo(0);
}
