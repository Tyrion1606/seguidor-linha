// chao ceramica  = 50 , fita= 300
// cimento = 400 , fita =580
#include <MX1508.h>
#include <ACS712.h>

#define PINA1 6 // Pino de controle PWM para motor A (direita)
#define PINA2 7 // Pino de controle que será configurado como LOW (GND) para motor A (direita)
#define PINB1 5 // Pino de controle PWM para motor B (esuqerda)
#define PINB2 4 // Pino de controle que será configurado como LOW (GND) para motor B (esquerda)
#define NUMPWM 1
#define PWM 105 // Velocidade PWM desejada

MX1508 motorA(PINA1, PINA2, FAST_DECAY, NUMPWM);
MX1508 motorB(PINB1, PINB2, FAST_DECAY, NUMPWM);


#define IR_PIN_RIGHT A1
#define IR_PIN_LEFT A0

int leitura_pino_r=0;
int leitura_pino_l =0;

void setup(){
  Serial.begin(9600);
  pinMode(IR_PIN_RIGHT,INPUT);
  pinMode(IR_PIN_LEFT,INPUT);

}
void loop (){


leitura_pino_r = analogRead(IR_PIN_RIGHT);
Serial.print("sensor A: ");
Serial.print(leitura_pino_r);
Serial.print("   sensor B: ");
leitura_pino_l = analogRead(IR_PIN_LEFT);
Serial.println(leitura_pino_l);

  
    if (leitura_pino_r > 150) {
      motorA.motorGo(-(2*PWM));
      delay(500);
    } else {
      motorA.motorGo(PWM);
    }

    if (leitura_pino_l > 150) {
      motorB.motorGo(-(2*PWM));
      delay(500);
    } else {
        motorB.motorGo(PWM);
    }
//delay(1000);
}