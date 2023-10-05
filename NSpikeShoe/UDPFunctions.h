#ifndef UDP_FUNCTIONS_H
#define UDP_FUNCTIONS_H

//packet structure, 3 characters long, check that our data starts with the start character, comma splits
// start character
// S
// Mood (0) or distance (1)
// 0
// Value (0-3 for mood, 0-1 for distance)
// 0

// distance value 0 is no person, 1 is person

// So example:
// S00
// S01
// S10


#include <ESP8266WiFi.h>
#include <Ticker.h>
//#include <AsyncMqttClient.h>
#include <WiFiUdp.h>

// SSID that the Raspberry Pi is running
#define WIFI_SSID "NSpike"
#define WIFI_PASSWORD "IROSIROS"

// IP address for the Raspberry Pi
#define MQTT_HOST IPAddress(192, 168, 4, 1)
#define MQTT_PORT 1883

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[4];
//char replyPacket[] = "Hi there! Got the message :-)";

char distance = '0';
char pot_state = '0';

void connectToWifi() {
  //Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  //Serial.println("Connected to Wi-Fi.");
  Udp.begin(localUdpPort);
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  //Serial.println("Disconnected from Wi-Fi.");
  //mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void udpSetup()
{
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  connectToWifi();
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   //Serial.print(".");
  // }
  //Serial.println(" connected");

  //Udp.begin(localUdpPort);
  //Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void checkPacketContent()
{
  if( WiFi.status() != WL_CONNECTED) { return; }
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 3);
    if (len > 0)
    {
      // bad packet? maybe just return?
      incomingPacket[len] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if (incomingPacket[0] != 'S')
    {
      // not a good packet, ignore it
      return;
    }
    char which_var = incomingPacket[1];
    char which_state = incomingPacket[2];

    // if it's not zero, we're gonna assume it's distance
    if (which_var == '1')
    {
      distance = which_state;
    }
    // Otherwise it's for the potentiometer
    else
    {
      pot_state = which_state;
    }


    //if(incomingPacket[1] )


    // send back a reply, to the IP address and port we got the packet from
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(replyPacket);
    //Udp.endPacket();
}
}

#endif
