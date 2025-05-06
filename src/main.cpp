#include <Arduino.h>
// Pinos do sensor ultrassônico
const int trigPin = 9;
const int echoPin = 10;

// Pino do relé
const int relayPin = 3;

// LED indicador de presença
const int ledPin = 2;

// Botão para desligar manualmente (modo economia)
const int buttonPin = 7;

bool economia = false;  // Estado do modo economia

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Botão com resistor pull-up interno

  digitalWrite(relayPin, LOW);  // Relé começa desligado
  digitalWrite(ledPin, LOW);    // LED apagado

  Serial.begin(9600);  // Para monitorar a distância (opcional)
}

void loop() {
  // Leitura do botão (ativo em nível BAIXO)
  if (digitalRead(buttonPin) == LOW) {
    economia = !economia;  // Alterna o modo economia
    delay(200);  // Debounce do botão
  }

  if (!economia) {
    // Mede distância com o sensor ultrassônico
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2;  // Conversão para cm

    Serial.print("Distância: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);  // Aguarda meio segundo entre leituras

    // Verifica se há presença
    if (distance > 0 && distance < 90) {
      digitalWrite(relayPin, HIGH);  // Liga luz (relé)
      digitalWrite(ledPin, HIGH);    // Liga LED indicador
    } else {
      digitalWrite(relayPin, LOW);   // Desliga luz
      digitalWrite(ledPin, LOW);     // Desliga LED
    }
  } else {
    // Modo economia: tudo desligado
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, LOW);
  }
}
