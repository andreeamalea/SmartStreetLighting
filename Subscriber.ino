#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Nume_WiFi";
const char* password = "Parola_WiFi";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2; // LED-ul integrat sau extern 

// Funcția callback(): Procesează mesajul primit de la Broker 
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) { message += (char)payload[i]; }

  // Dacă mesajul este "DETECTATA", activează becul 
  if (message == "DETECTATA") {
    digitalWrite(ledPin, HIGH);
    delay(10000); // Ține lumina aprinsă 10 secunde
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Înregistrare funcție prelucrare
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

// Funcția subscribe(): Înregistrează dispozitivul la canal
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Receptor_Muntean")) {
      client.subscribe("oras/strada1/senzor_miscare"); // Abonare canal
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) { reconnect(); }
  client.loop();
}