// Pinos do Sensor Ultrassônico HC-SR04
const int pinTrig = 6;
const int pinEcho = 7;

// Pinos de controle da Ponte H (Motor DC)
const int motorPin1 = 10;
const int motorPin2 = 9;

// Variável de controle do sentido da rotação
// false = Anti-horário | true = Horário
bool sentidoHorario = false; 

void setup() {
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Garante que o motor inicie desligado
  pararMotor();
  
  Serial.begin(9600);
}

void loop() {
  float distancia = medirDistancia();

  // Se detectar um objeto a 4 cm ou menos (e maior que zero para evitar leituras erráticas)
  if (distancia > 0 && distancia <= 4.0) {
    
    if (!sentidoHorario) {
      // 1ª Leitura: Gira no sentido ANTI-HORÁRIO por 6 segundos
      Serial.println("Girando no sentido ANTI-HORÁRIO...");
      rotacionarAntiHorario();
      delay(6000);
      pararMotor();
      
      // Alterna a flag para a próxima acionamento
      sentidoHorario = true;
    } 
    else {
      // 2ª Leitura: Gira no sentido HORÁRIO por 6 segundos
      Serial.println("Girando no sentido HORÁRIO...");
      rotacionarHorario();
      delay(6000);
      pararMotor();
      
      // Alterna a flag para a próxima acionamento
      sentidoHorario = false;
    }

    // Trava de segurança: aguarda o objeto ser removido da frente do sensor
    // para evitar múltiplos acionamentos seguidos pelo mesmo objeto.
    delay(500);
    while (medirDistancia() <= 4.0) {
      delay(100);
    }
  }

  delay(60); // Pequeno intervalo padrão entre leituras
}

// Função para calcular a distância em cm
float medirDistancia() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  long duracao = pulseIn(pinEcho, HIGH, 30000); // Timeout de 30ms
  if (duracao == 0) return 999.0;               // Fora de alcance

  return (duracao * 0.0343) / 2.0;
}

// Funções de acionamento do motor via Ponte H
void rotacionarHorario() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void rotacionarAntiHorario() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void pararMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}