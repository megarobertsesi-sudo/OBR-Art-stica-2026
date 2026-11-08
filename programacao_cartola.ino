#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Inicializa a placa no endereço padrão I2C (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150 
#define SERVOMAX  600 

// Definição dos pinos do Sensor Ultrassônico HC-SR04
const int trigPin = 9;

const int echoPin = 8;

// Variável para contar em qual etapa/vez o sistema está (0 = aguardando 1ª vez)
int etapa = 0; 

// Variável para evitar leituras consecutivas muito rápidas do mesmo objeto
unsigned long ultimaLeitura = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60); // Frequência de 60Hz para servos

  // Configuração dos pinos do ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  delay(500);

  // Pré-calculando os pulsos dos ângulos iniciais
  int p60  = map(60, 0, 180, SERVOMIN, SERVOMAX);
  int p90  = map(90, 0, 180, SERVOMIN, SERVOMAX);
  int p120 = map(120, 0, 180, SERVOMIN, SERVOMAX);

  // Inicialização: Servos 0 e 2 em 90°, Servo 4 em 120°
  pwm.setPWM(0, 0, p60);
  pwm.setPWM(2, 0, p90);
  pwm.setPWM(4, 0, p120);
  
  Serial.println("Sistema pronto. Servos 0 e 2 em 90°, Servo 4 em 120°. Aguardando sensor...");
}

void loop() {
  // Faz a leitura da distância com o sensor ultrassônico
  long duracao;
  float distancia;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao * 0.034 / 2; // Converte para centímetros

  // Verifica se detectou algo a 8 cm ou menos e respeita um intervalo de 2 segundos entre ativações
  if (distancia > 0 && distancia <= 8 && (millis() - ultimaLeitura > 2000)) {
    ultimaLeitura = millis(); // Atualiza o tempo da última ativação
    etapa++; // Avança para a próxima etapa

    // Pré-calculando os pulsos dos ângulos utilizados nas rotinas
    int p_menos10 = map(0, 0, 180, SERVOMIN, SERVOMAX); // Limitado a 0° por segurança mecânica
    int p180      = map(180, 0, 180, SERVOMIN, SERVOMAX);
    int p100      = map(100, 0, 180, SERVOMIN, SERVOMAX);
    int p90       = map(90, 0, 180, SERVOMIN, SERVOMAX);
    int p65       = map(65, 0, 180, SERVOMIN, SERVOMAX);
    int p0        = map(0, 0, 180, SERVOMIN, SERVOMAX);

    // ==========================================
    // 1º PASSE: Servo 0 e Servo 1
    // ==========================================
    if (etapa == 1) {
      Serial.println("--- 1ª Detecção: Acionando Servo 0 e 1 ---");
      
      // Servo 0 vai para 180° (estava em 90°)
      pwm.setPWM(0, 0, p180);
      delay(500);

      // Servo 1 inicia em 0°
      pwm.setPWM(1, 0, p0);
      delay(300);

      // Servo 1 oscila entre -10° (0°) e 100° por 8 segundos
      unsigned long tempoInicio = millis();
      while (millis() - tempoInicio < 8000) {
        pwm.setPWM(1, 0, p_menos10);
        delay(300);
        pwm.setPWM(1, 0, p100);
        delay(300);
      }

      // Servo 1 para em 0°
      pwm.setPWM(1, 0, p0);

      // Servo 0 retorna para o seu início que é 90°
      pwm.setPWM(0, 0, p65);
      Serial.println("1ª Etapa concluída. Aguardando 2ª detecção...");
    }

    // ==========================================
    // 2º PASSE: Servo 4 e Servo 5
    // ==========================================
    else if (etapa == 2) {
      Serial.println("--- 2ª Detecção: Acionando Servo 4 e 5 ---");
      
      // Servo 4 vai para 0°
      pwm.setPWM(4, 0, p0);
      delay(500);

      // Servo 5 inicia em 0°
      pwm.setPWM(5, 0, p0);
      delay(300);

      // Servo 5 oscila entre -10° (0°) e 100° por 8 segundos
      unsigned long tempoInicio = millis();
      while (millis() - tempoInicio < 8000) {
        pwm.setPWM(5, 0, p_menos10);
        delay(300);
        pwm.setPWM(5, 0, p100);
        delay(300);
      }

      // Servo 5 para em 0°
      pwm.setPWM(5, 0, p0);

      // Servo 4 retorna para o valor original de inicialização (120°)
      int p120 = map(120, 0, 180, SERVOMIN, SERVOMAX);
      pwm.setPWM(4, 0, p120);
      Serial.println("2ª Etapa concluída. Aguardando 3ª detecção...");
    }

    // ==========================================
    // 3º PASSE: Servo 2 e Servo 3
    // ==========================================
    else if (etapa == 3) {
      Serial.println("--- 3ª Detecção: Acionando Servo 2 e 3 ---");
      
      // Servo 2 vai para 0°
      pwm.setPWM(2, 0, p0);
      delay(500);

      // Servo 3 inicia em 0°
      pwm.setPWM(3, 0, p0);
      delay(300);

      // Servo 3 oscila entre -10° (0°) e 100° por 8 segundos
      unsigned long tempoInicio = millis();
      while (millis() - tempoInicio < 8000) {
        pwm.setPWM(3, 0, p_menos10);
        delay(300);
        pwm.setPWM(3, 0, p100);
        delay(300);
      }

      // Servo 3 para em 0°
      pwm.setPWM(3, 0, p0);

      // Servo 2 retorna para 90°
      pwm.setPWM(2, 0, p90);
      Serial.println("3ª Etapa concluída. Reiniciando o ciclo para a 1ª etapa.");

      // Retorna a etapa para 0, fazendo o ciclo reiniciar na próxima detecção
      etapa = 0; 
    }
  }

  // Pequeno intervalo entre as leituras do sensor
  delay(100);
}