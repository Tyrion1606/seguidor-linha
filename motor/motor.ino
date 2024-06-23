#include <MX1508.h>
#include <SPI.h>
#include <SdFat.h>

// Pinos dos sensores ir : sensor da direita está na A1 e o da esquerda está na A0
// Pino dos motores : motor direito está nas saídas d4,d5,d6,d7
// Pino D5,D6 são PWM são cada um para um motor diferente
// Pino D4,D7 são de GND
// Encoder estão nos pinos d2(encoder esquerda) e d3 (encoder direita)
#define PINA1 6 // Pino de controle PWM para motor A (direita)
#define PINA2 7 // Pino de controle que será configurado como LOW (GND) para motor A (direita)
#define PINB1 5 // Pino de controle PWM para motor B (esuqerda)
#define PINB2 4 // Pino de controle que será configurado como LOW (GND) para motor B (esquerda)
#define NUMPWM 1
#define PWM 105 // Velocidade PWM desejada

#define IR_PIN_RIGHT A1
#define IR_PIN_LEFT A0
#define PWM_SPEED 79 // Velocidade PWM desejada
#define ENCODER_PPR 1024 // Pulsos por Rotação do Encoder
#define INTERVALO_CALCULO 1000 // Intervalo de tempo em milissegundos para cálculo da velocidade

leitura_pino_r = analogRead(IR_PIN_RIGHT);
Serial.println(leitura_pino_r);
leitura_pino_l = analogRead(IR_PIN_LEFT);
Serial.println(leitura_pino_l);


MX1508 motorA(PINA1, LOW, FAST_DECAY, NUMPWM);
MX1508 motorB(PINB1, LOW, FAST_DECAY, NUMPWM);

// Variável global para armazenar o tempo de início
unsigned long startTime = 0;

// Variáveis para contagem de pulsos
volatile unsigned long pulsosMotorA = 0;
volatile unsigned long pulsosMotorB = 0;
unsigned long ultimoTempo = 0;

// Objeto da biblioteca SdFat para controle do cartão SD
SdFat sd;
// Objeto para o arquivo no cartão SD
SdFile file;

// Nome do arquivo no cartão SD
const char *fileName = "bosta_motor.txt";

void startTimer() {
    startTime = millis(); // Salva o tempo atual em milissegundos
}

unsigned long getTimeElapsed() {
    return millis() - startTime; // Calcula e retorna o tempo decorrido
}

// Interrupções para contagem de pulsos
void contaPulsosMotorA() {
    pulsosMotorA++;
}

void contaPulsosMotorB() {
    pulsosMotorB++;
}

void setup() {
    pinMode(PINA2, OUTPUT);
    pinMode(PINB2, OUTPUT);
    digitalWrite(PINA2, LOW); // Configura PINA2 como GND para motor A
    digitalWrite(PINB2, LOW); // Configura PINB2 como GND para motor B

    Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps

    // Inicialização dos pinos SPI manualmente (de acordo com a configuração do seu hardware)
    const int chipSelectPin = 10; // Pino do chip select do cartão SD

    // Inicializa o cartão SD
    if (!sd.begin(chipSelectPin, SPI_HALF_SPEED)) {
        Serial.println("Falha ao inicializar o cartão SD.");
        return;
    }

    // Abre o arquivo no cartão SD para escrita
    if (!file.open(fileName, O_CREAT | O_WRITE | O_APPEND)) {
        Serial.println("Falha ao abrir o arquivo no cartão SD.");
        return;
    }

    Serial.println("Cartão SD inicializado e arquivo aberto.");

    startTimer(); // Inicia a contagem de tempo no início do programa
    ultimoTempo = millis(); // Inicializa o tempo para contagem de pulsos

    // Configuração das interrupções do encoder
    attachInterrupt(digitalPinToInterrupt(2), contaPulsosMotorA, RISING); // Atribua o pino correto do encoder para motor A
    attachInterrupt(digitalPinToInterrupt(3), contaPulsosMotorB, RISING); // Atribua o pino correto do encoder para motor B
}

void loop() {
    // Verifica se há dados disponíveis para leitura no Serial Monitor
    if (Serial.available() > 0) {
        char input = Serial.read();
        if (input == 'p') {
            Serial.println("Programa encerrado.");
            // Fecha o arquivo antes de encerrar
            file.close();
            while (true) {} // Loop infinito para manter o programa parado
        }
    }

    unsigned long tempoAtual = millis();
    if (tempoAtual - ultimoTempo >= INTERVALO_CALCULO) {
        // Calcular a frequência dos pulsos
        float freqMotorA = (float)pulsosMotorA / (INTERVALO_CALCULO / 1000.0); // pulsos por segundo
        float freqMotorB = (float)pulsosMotorB / (INTERVALO_CALCULO / 1000.0); // pulsos por segundo

        // Calcular a velocidade em RPM
        float velocidadeA = (freqMotorA * 60.0) / ENCODER_PPR;
        float velocidadeB = (freqMotorB * 60.0) / ENCODER_PPR;

        // Resetar contadores de pulsos
        pulsosMotorA = 0;
        pulsosMotorB = 0;
        ultimoTempo = tempoAtual;

        // Formata os dados para escrever no arquivo no cartão SD
        char buffer[100];
        sprintf(buffer, "Tempo: %lu ms, Velocidade Motor A: %.2f RPM, Velocidade Motor B: %.2f RPM\n", getTimeElapsed(), velocidadeA, velocidadeB);

        // Escreve os dados no arquivo no cartão SD
        if (!file.println(buffer)) {
            Serial.println("Erro ao escrever no arquivo no cartão SD.");
        } else {
            Serial.println("Dados salvos no cartão SD.");
        }

        // Imprime os dados no Serial Monitor
        Serial.print("Tempo decorrido: ");
        Serial.print(getTimeElapsed());
        Serial.print(" ms, Velocidade Motor A: ");
        Serial.print(velocidadeA);
        Serial.print(" RPM, Velocidade Motor B: ");
        Serial.println(velocidadeB);
    }

    delay(200); // Aguarda um pouco antes de repetir o loop
}

// read analod read


velocidade certo muda pino