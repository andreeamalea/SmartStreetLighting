#include <WiFi.h>
#include <PubSubClient.h>

// Configurații Rețea și Broker
const char* ssid = "Nume_WiFi";
const char* password = "Parola_WiFi";
const char* mqtt_server = "broker.hivemq.com"; // Ex. de broker gratuit

WiFiClient espClient;
PubSubClient client(espClient);

const int pirPin = 13; // Pinul la care este conectat senzorul PIR 

void setup() {
  pinMode(pirPin, INPUT); // Inițializare senzor
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

// Funcția reconnect(): Asigură reluarea conexiunii TCP/IP
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Senzor_Muntean")) {
      Serial.println("Conectat la Broker");
    } else {
      delay(5000);
    }
  }
}

// Funcția readSensors() și publishData()
void loop() {
  if (!client.connected()) { reconnect(); }
  client.loop();

  int miscare = digitalRead(pirPin); // Citire stare senzor

  if (miscare == HIGH) {
    // Transmite starea "DETECTATA" pe canalul dedicat
    client.publish("oras/strada1/senzor_miscare", "DETECTATA");
    Serial.println("Mișcare detectată! Mesaj trimis.");
    delay(5000); // Evită un flood de mesaje
  }
}
