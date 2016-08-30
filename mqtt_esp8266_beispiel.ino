/*
  Basic ESP8266 MQTT example etwas angedeutscht
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define AKTOR   2  // hier Aktive-Low (Built-in-LED)
#define SENSOR  4  // mit Pullup ; Aktive-Low

const char* ssid              = "ssid";
const char* password          = "password";
const char* clientName        = "Name";
const char* mqtt_server       = "192.168.178.xxx";
const char* start             = "fhem/online";
const char* ziel              = "fhem/zimmer/Sensor/set";
const char* anmeldeKanal1     = "fhem/zimmer/Aktor/set";
long        verzoegern        = 0;
boolean     gedrueckt         = false;

WiFiClient ESPClient;
PubSubClient client(ESPClient);

void setup() {
  pinMode(AKTOR, OUTPUT);
  digitalWrite(AKTOR, LOW);
  pinMode(SENSOR, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* kanal, byte* nachrichtInBytes, unsigned int length) {
  String nachricht = "";
  for (int i = 0; i < length; i++) {
    nachricht += (char)nachrichtInBytes[i];
  }
  String  stringKanal         = kanal;
  String  stringAnmeldeKanal1 = anmeldeKanal1;
  if (stringKanal == stringAnmeldeKanal1) {
    if (nachricht == "on") {
      digitalWrite(AKTOR, LOW);
    } else {
      digitalWrite(AKTOR, HIGH);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Warten auf Anmeldung...");
    if (client.connect(clientName)) {
      Serial.println("angemeldet");
      client.publish(start, clientName);
      client.subscribe(anmeldeKanal1);
    } else {
      Serial.print("Fehler, rc=");
      Serial.print(client.state());
      Serial.println(" erneut versuchen in 5 Sekunden");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - verzoegern > 200) {
    if (gedrueckt && digitalRead(SENSOR) == 1 ) {
      gedrueckt = false;
      client.publish(ziel, "off");
    }
    if ( !gedrueckt && digitalRead(SENSOR) == 0) {
      gedrueckt = true;
      verzoegern = now;
      client.publish(ziel, "on");
    }
  }
}
