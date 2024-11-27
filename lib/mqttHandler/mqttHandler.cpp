#include "mqttHandler.h"
#include "secrets.h"

// Initialize the static pointer
MQTTHandler* MQTTHandler::instance = nullptr;

// Constructor
MQTTHandler::MQTTHandler() : client(espClient), userActionCallback(nullptr) {
  instance = this; // Save the pointer to this instance
}

// WiFi connection
void MQTTHandler::setupWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi.");
}

// MQTT connection
void MQTTHandler::setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);

  // Set the static callback function
  client.setCallback(mqttCallback);
}

// Main loop for MQTT
void MQTTHandler::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// Reconnect to MQTT
void MQTTHandler::reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe("phoneExchange"); // Subscribe to topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Publish data
void MQTTHandler::publishMQTT(int line, uint32_t status) {
  StaticJsonDocument<JSON_DOC_SIZE> jsonDoc;
  jsonDoc["line"] = line;
  jsonDoc["status"] = status;

  char jsonBuffer[JSON_DOC_SIZE];
  serializeJson(jsonDoc, jsonBuffer);

  client.publish("phoneExchange", jsonBuffer); // Publish to phoneExchange
}

// Set external callback function for decoded data
void MQTTHandler::setActionCallback(void (*actionCallback)(int line, uint32_t status)) {
  userActionCallback = actionCallback;
}

// Static callback for PubSubClient
void MQTTHandler::mqttCallback(char* topic, unsigned char* payload, unsigned int length) {
  // Check that the instance is set
  if (!instance) {
    Serial.println("No instance available for handling MQTT messages.");
    return;
  }

  // Convert payload to a null-terminated string
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  // Parse JSON
  StaticJsonDocument<JSON_DOC_SIZE> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, message);

  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Read JSON content
  int line = jsonDoc["line"];
  uint32_t status = jsonDoc["status"];

  // Call the user's callback
  if (instance->userActionCallback) {
    instance->userActionCallback(line, status);
  } else {
    Serial.println("No action callback defined.");
  }
}
