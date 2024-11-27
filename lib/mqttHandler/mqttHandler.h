#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class MQTTHandler {
public:
  MQTTHandler();
  void setupWiFi();
  void setupMQTT();
  void loop();
  void publishMQTT(int line, uint32_t status);
  void setActionCallback(void (*actionCallback)(int line, uint32_t status));

private:
  WiFiClient espClient;
  PubSubClient client;

  void reconnect();
  void (*userActionCallback)(int line, uint32_t status); // Pointer to user-defined callback

  // Static callback function for PubSubClient
  static void mqttCallback(char* topic, unsigned char* payload, unsigned int length);

  // Static pointer to an MQTTHandler instance
  static MQTTHandler* instance;

  static const int JSON_DOC_SIZE = 200;
};

#endif
