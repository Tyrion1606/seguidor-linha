

// Variáveis globais
volatile unsigned long contadorPulsos = 0; // Volatile porque será alterado dentro da ISR
unsigned long tempoAnterior = 0;
unsigned long tempoAtual = 0;
unsigned long intervalo = 0;
float velocidade = 0.0;

// Configurações do encoder
const int pinoEncoder = 3; // Pino digital onde a saída do encoder está conectada

void setup() {
  // Inicia a comunicação serial com uma taxa de 9600 bps
  Serial.begin(9600);

  // Configura o pino do encoder como entrada
  pinMode(pinoEncoder, INPUT);

  // Configura a interrupção na borda de subida
  attachInterrupt(digitalPinToInterrupt(pinoEncoder), contadorPulsoISR, RISING);
}

void loop() {
  // Calcula o tempo atual
  tempoAtual = millis();

  // Calcula o intervalo de tempo entre leituras
  intervalo = tempoAtual - tempoAnterior;

  // Calcular a velocidade a cada segundo
  if (intervalo >= 1000) {
    // Calcula a velocidade em rotações por segundo (RPS)
    velocidade = contadorPulsos;

    // Reinicia o contador de pulsos
    contadorPulsos = 0;

    // Atualiza o tempo anterior
    tempoAnterior = tempoAtual;

    // Imprime a velocidade no Monitor Serial
    Serial.print("Velocidade: ");
    Serial.print(velocidade);
    Serial.print(" RPS - pino:");
    Serial.println(digitalRead(pinoEncoder));
  }
}

// Função de Interrupção para contar os pulsos
void contadorPulsoISR() {
  contadorPulsos++;
}