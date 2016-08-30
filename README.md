# MQTT_ESP_FHEM
eingedeutsches PubSubClient Programm, basierend auf diversen Quellen.

Dieses Programm sendet einen MQTT Befehl auf dem Kanal "fhem/zimmer/Sensor/set" an den FHEM-Server, sofern ein Schalter (PIN 2) gedrückt wird (Aktiv-LOW).

Es reagiert weiterhin auf die Nachricht "on" auf dem Kanal "fhem/zimmer/Aktor/set" mit dem Einschalten der auf dem ESP8266 aufgelöteten LED (built in led). Alle anderen Nachrichten schalten die LED aus.

Folgender Code-Ausschnitt müsste in der fhem.cfg hinterlegt werden.

```
# Zimmer ------------------------------------------------------------------

define Aktor dummy
attr   Aktor room Zimmer
attr   Aktor webCmd on:off

define Sensor dummy
attr   Sensor room Zimmer

define mqtt_Aktor  MQTT_BRIDGE Aktor
attr   mqtt_Aktor  IODev mqtt
attr   mqtt_Aktor  publishState fhem/zimmer/Aktor/set
attr   mqtt_Aktor  room Zimmer

define mqtt_Sensor  MQTT_BRIDGE Sensor
attr   mqtt_Sensor  IODev mqtt
attr   mqtt_Sensor  room Zimmer
attr   mqtt_Sensor  subscribeSet fhem/zimmer/Sensor/set
```
