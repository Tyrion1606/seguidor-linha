#include <MX1508.h>
#include <ACS712.h>
#include <SPI.h>
#include <SdFat.h>

#define PINA1 6 // Pino de controle PWM para motor A (direita)
#define PINA2 7 // Pino de controle que será configurado como LOW (GND) para motor A (direita)
#define PINB1 5 // Pino de controle PWM para motor B (esquerda)
#define PINB2 4 // Pino de controle que será configurado como LOW (GND) para motor B (esquerda)
#define NUMPWM 1
#define PWM 105 // Velocidade PWM desejada

#define IR_PIN_RIGHT A1
#define IR_PIN_LEFT A0
#define ENCODER_PPR 1024 // Pulsos por Rotação do Encoder
#define INTERVALO_CALCULO 1000 // Intervalo de tempo em milissegundos para cálculo da velocidade

MX1508 motorA(PINA1, PINA2, FAST_DECAY, NUMPWM);
MX1508 motorB(PINB1, PINB2, FAST_DECAY, NUMPWM);

// Variáveis globais para armazenar leituras dos sensores IR
int leitura_pino_r = 0;
int leitura_pino_l = 0;

// Variáveis globais para contagem de pulsos
volatile unsigned long pulsosMotorA = 0; // Contador de pulsos do motor A
volatile unsigned long pulsosMotorB = 0; // Contador de pulsos do motor B

// Variável global para armazenar o tempo de início
unsigned long startTime = 0;

// Objeto da biblioteca SdFat para controle do cartão SD
SdFat sd;
// Objeto para o arquivo no cartão SD
SdFile file;

// Nome do arquivo no cartão SD
const char *fileName = "dados_motor.txt";

void startTimer() {
    startTime = millis(); // Salva o tempo atual em milissegundos
}

unsigned long getTimeElapsed() {
    return millis() - startTime; // Calcula e retorna o tempo decorrido
}

// Interrupções para contagem de pulsos
void contaPulsosMotorA() {
    pulsosMotorA++; // Incrementa o contador de pulsos do motor A
}

void contaPulsosMotorB() {
    pulsosMotorB++; // Incrementa o contador de pulsos do motor B
}

void setup() {
    pinMode(PINA2, OUTPUT);
    pinMode(PINB2, OUTPUT);
    digitalWrite(PINA2, LOW); // Configura PINA2 como GND para motor A
    digitalWrite(PINB2, LOW); // Configura PINB2 como GND para motor B

    Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps

    pinMode(IR_PIN_RIGHT, INPUT);
    pinMode(IR_PIN_LEFT, INPUT);

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

    // Inicia a contagem de tempo no início do programa
    startTimer();

    // Configuração das interrupções do encoder
    attachInterrupt(digitalPinToInterrupt(2), contaPulsosMotorA, RISING); // Interrupção no pino do encoder A, borda de subida
    attachInterrupt(digitalPinToInterrupt(3), contaPulsosMotorB, RISING); // Interrupção no pino do encoder B, borda de subida
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

    // Leituras dos sensores IR
    leitura_pino_r = analogRead(IR_PIN_RIGHT);
    leitura_pino_l = analogRead(IR_PIN_LEFT);

    Serial.print("sensor A: ");
    Serial.print(leitura_pino_r);
    Serial.print("   sensor B: ");
    Serial.println(leitura_pino_l);

    // Controle dos motores baseado nas leituras dos sensores IR
    if (leitura_pino_r > 150) {
        motorA.motorGo(-(2 * PWM));
        
    } else {
        motorA.motorGo(PWM);
    }

    if (leitura_pino_l > 150) {
        motorB.motorGo(-(2 * PWM));
        
    } else {
        motorB.motorGo(PWM);
    }

    // Cálculo da velocidade dos motores e gravação dos dados no cartão SD
    unsigned long tempoAtual = millis();
    static unsigned long ultimoTempo = 0;
    if (tempoAtual - ultimoTempo >= INTERVALO_CALCULO) {
        // Calcular a frequência dos pulsos
        unsigned long freqMotorA = pulsosMotorA / (INTERVALO_CALCULO / 1000); // pulsos por segundo
        unsigned long freqMotorB = pulsosMotorB / (INTERVALO_CALCULO / 1000); // pulsos por segundo

        // Calcular a velocidade em RPM
        unsigned long velocidadeA = (freqMotorA * 60) / ENCODER_PPR;
        unsigned long velocidadeB = (freqMotorB * 60) / ENCODER_PPR;

        // Resetar contadores de pulsos
        pulsosMotorA = 0;
        pulsosMotorB = 0;
        ultimoTempo = tempoAtual;

        // Formata os dados para escrever no arquivo no cartão SD
        char buffer[150];
        sprintf(buffer, "Tempo: %lu ms, Velocidade Motor A: %lu RPM, Velocidade Motor B: %lu RPM, sensor A: %d, sensor B: %d\n", millis(), velocidadeA, velocidadeB, leitura_pino_r, leitura_pino_l);

        // Escreve os dados no arquivo no cartão SD
        if (!file.println(buffer)) {
            Serial.println("Erro ao escrever no arquivo no cartão SD.");
        } else {
            Serial.println("Dados salvos no cartão SD.");
        }

        // Imprime os dados no Serial Monitor
        Serial.print("Tempo decorrido: ");
        Serial.print(millis());
        Serial.print(" ms, Velocidade Motor A: ");
        Serial.print(velocidadeA);
        Serial.print(" RPM, Velocidade Motor B: ");
        Serial.print(velocidadeB);
        Serial.print(" RPM, sensor A: ");
        Serial.print(leitura_pino_r);
        Serial.print(" , sensor B: ");
        Serial.println(leitura_pino_l);
    }

    delay(200); // Aguarda um pouco antes de repetir o loop
}
