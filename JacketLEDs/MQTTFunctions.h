#ifndef MQTT_FUNCTIONS_H
#define MQTT_FUNCTIONS_H

// This holds most of the MQTT code, so it's not a million lines on
//  the main .ino file. Is it bad practice to include everything
//  in a .h file? Sure. But it's also Arduino, so whatever.

/// NOTE! You need to download these two libraries and install them
//   into Arduino for this code to compile
//    https://github.com/marvinroger/async-mqtt-client/archive/master.zip
//    https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

// What channel to listen
#define MQTT_DIST_TOPIC "IROS/hue"

// SSID that the Raspberry Pi is running
#define WIFI_SSID "NSpike"
#define WIFI_PASSWORD "IROSIROS"
//#define WIFI_SSID "Clowns"
//#define WIFI_PASSWORD "deadp00l"

// IP address for the Raspberry Pi
#define MQTT_HOST IPAddress(192, 168, 4, 1)
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  
  mqttClient.subscribe(MQTT_DIST_TOPIC, 0);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  if(String(topic) == MQTT_DIST_TOPIC) {
    gHue = atoi(payload);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setupMQTT() {

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
//  connectToMqtt();

}














#endif
