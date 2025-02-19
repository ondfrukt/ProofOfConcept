#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "lineStatuses.h"

class MQTTHandler {
public:
  MQTTHandler(int wifiPIN, int mqttPIN);
  void setupWiFi();
  void setupMQTT();
  void loop();
  void publishMQTT(int line, uint8_t status);
  void setActionCallback(void (*actionCallback)(int line, uint8_t status));
  void reconnect();
  bool isConnected();

private:
  WiFiClient espClient;
  PubSubClient client;
  int wifiPIN;
  int mqttPIN;

  void (*userActionCallback)(int line, uint8_t status); // Pointer to user-defined callback

  // Static callback function for PubSubClient
  static void mqttCallback(char* topic, unsigned char* payload, unsigned int length);

  // Static pointer to an MQTTHandler instance
  static MQTTHandler* instance;

  static const int JSON_DOC_SIZE = 256;
};

#endif
