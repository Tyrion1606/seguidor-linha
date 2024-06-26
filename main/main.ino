// chao ceramica  = 50 , fita= 300
// cimento = 400 , fita =580
#include <MX1508.h>
#include <ACS712.h>

// Configuração dos Motores
#define PINA1 6 // Pino de controle PWM para motor A (direita)
#define PINA2 7 // Pino de controle que será configurado como LOW (GND) para motor A (direita)
#define PINB1 5 // Pino de controle PWM para motor B (esuqerda)
#define PINB2 4 // Pino de controle que será configurado como LOW (GND) para motor B (esquerda)
#define NUMPWM 1
#define PWM 120 // Velocidade PWM desejada

MX1508 motorA(PINA1, PINA2, FAST_DECAY, NUMPWM);
MX1508 motorB(PINB1, PINB2, FAST_DECAY, NUMPWM);

// Configuração dos Encoders
#define ENCODER_A 2
#define ENCODER_B 3

// Configuração dos Sensores IR
#define CENTER A0
#define IR_PIN_RIGHT_2 A1
#define IR_PIN_LEFT_2 A3
#define threshold 150

// bool onLine_r1 = false;
bool onLine_r2 = false;
// bool onLine_l1 = false;
bool onLine_l2 = false;
bool onLine_center = false;

bool turning_FLAG = false;

// Configuração do Amperimetro
#define ANMETER A5  // Pino analógico onde o sensor está conectado
#define AnmeterZero 0 // Valor do sensor quando a corrente é 0A

void setup(){
  Serial.begin(9600);

  pinMode(CENTER,INPUT);
  pinMode(IR_PIN_RIGHT_2,INPUT);
  // pinMode(IR_PIN_LEFT_1,INPUT);
  pinMode(IR_PIN_LEFT_2,INPUT);

  pinMode(ANMETER, INPUT);

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);

  // Motor pins
  pinMode(PINA1, OUTPUT);
  pinMode(PINA2, OUTPUT);
  pinMode(PINB1, OUTPUT);
  pinMode(PINB2, OUTPUT);
}
void loop (){
  // Set Motor Speed - Walk on line
  if(!turning_FLAG){
    onLine_r2 = (analogRead(IR_PIN_RIGHT_2) > threshold);
    onLine_l2 = (analogRead(IR_PIN_LEFT_2) > threshold);

    // const int motorA_PWM = ((!onLine_r2)*PWM);
    // const int motorB_PWM = ((!onLine_l2)*PWM);
    motorA.motorGo((!onLine_r2)*PWM);
    motorB.motorGo((!onLine_l2)*PWM);
    turning_FLAG = (onLine_r2 || onLine_l2);
    if (turning_FLAG){
      if (onLine_r2){
        motorA.motorGo(-PWM);
      }
      if (onLine_l2){
        motorB.motorGo(-PWM);
      }
    }
  }
  if(turning_FLAG){
    onLine_center = (analogRead(CENTER) > threshold);
    if(onLine_center){
      onLine_r2 = (analogRead(IR_PIN_RIGHT_2) > threshold);
      onLine_l2 = (analogRead(IR_PIN_LEFT_2) > threshold);
      motorA.motorGo((!onLine_r2)*PWM);
      motorB.motorGo((!onLine_l2)*PWM);
      turning_FLAG = false;
      onLine_center = false;
    }
  }

  Serial.print(onLine_l2);
  Serial.print(onLine_center);
  Serial.print(onLine_r2);
  Serial.print(" - ");
  Serial.println(turning_FLAG);



  // if( && (onLine_r2 || onLine_l2)){
  //   turning_FLAG = true;
  //   const int motorA_PWM = ((!onLine_r2)*PWM);
  //   const int motorB_PWM = ((!onLine_l2)*PWM);
  // }

  // const int motorA_PWM = (((!onLine_r1 && !onLine_r2)*PWM)+((onLine_r2)*3*-PWM));
  // const int motorB_PWM = (((!onLine_l1 && !onLine_l2)*PWM)+((onLine_l2)*3*-PWM));
  // motorA.motorGo(motorA_PWM);
  // motorB.motorGo(motorB_PWM);

  // motorB.motorGo(0);

  // Consumo
  // float sensorValue = analogRead(ANMETER);  // Lê o valor do sensor (0-1023)
  // float voltage = ((sensorValue / 1023.0)*5);  // Converte para tensão (0-5V)
  // Serial.println(voltage - 2.5);
  // float current = (voltage - 1.56) / 0.185;  // Calcula a corrente (A)

  // Serial.print("motorA_PWM: ");
  // Serial.print(motorA_PWM);
  // Serial.print("  -  motorB_PWM: ");
  // Serial.print(motorB_PWM);
}


void irReadValues() {
  // onLine_r1 = (analogRead(IR_PIN_RIGHT_1) > threshold);
  onLine_r2 = (analogRead(IR_PIN_RIGHT_2) > threshold);
  // onLine_l1 = (analogRead(IR_PIN_LEFT_1) > threshold);
  onLine_l2 = (analogRead(IR_PIN_LEFT_2) > threshold);
  onLine_center = (analogRead(CENTER) > threshold);

  // Serial.print("  -  IR_PIN_RIGHT_1: ");
  // Serial.print(analogRead(IR_PIN_RIGHT_1));
  // Serial.print("  -  IR_PIN_RIGHT_2: ");
  // Serial.print(analogRead(IR_PIN_RIGHT_2));
  // Serial.print("  -  IR_PIN_LEFT_1: ");
  // Serial.print(analogRead(IR_PIN_LEFT_1));
  // Serial.print("  -  IR_PIN_LEFT_2: ");
  // Serial.println(analogRead(IR_PIN_LEFT_2));
}