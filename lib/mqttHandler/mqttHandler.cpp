#include "mqttHandler.h"
#include "secrets.h"

// Initialize the static pointer
MQTTHandler* MQTTHandler::instance = nullptr;

// Constructor
MQTTHandler::MQTTHandler(int wifiPIN, int mqttPIN) : client(espClient), wifiPIN(wifiPIN), mqttPIN(mqttPIN), userActionCallback(nullptr) {
  instance = this; // Save the pointer to this instance
  pinMode(wifiPIN, OUTPUT);
  pinMode(mqttPIN, OUTPUT);
  digitalWrite(wifiPIN, LOW);
  digitalWrite(mqttPIN, LOW);
}

// WiFi connection
void MQTTHandler::setupWiFi() {
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(wifiPIN, HIGH);
  Serial.print("Connected");
  Serial.print("   IP:"); Serial.println(WiFi.localIP());
}

// MQTT connection
void MQTTHandler::setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  // Set the static callback function
  client.setCallback(mqttCallback);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient", MQTT_USER, MQTT_PASSWORD)) {
      digitalWrite(mqttPIN, HIGH);
      Serial.print("connected  ");
      client.subscribe("phoneExchange"); // Subscribe to topic
      Serial.println("Subscribed to phoneExchange");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      }
  }
}

// Main loop for MQTT
void MQTTHandler::loop() {
  if (!client.connected()) {
    Serial.println("MQTT not connected. Reconnecting...");
    reconnect();
  }
  if (WiFi.status() == !WL_CONNECTED) {
    setupWiFi();
  }
  client.loop();
}

// Reconnect to MQTT
void MQTTHandler::reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe("phoneExchange/setLineStatus"); // Subscribe to topic
      Serial.println("Subscribed to phoneExchangesetLineStatus");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Publish MQTT message
void MQTTHandler::publishMQTT(int line, uint8_t status) {
  DynamicJsonDocument jsonDoc(JSON_DOC_SIZE);
  jsonDoc["line"] = line;
  jsonDoc["status"] = statusNames[status];

  char jsonBuffer[JSON_DOC_SIZE];
  serializeJson(jsonDoc, jsonBuffer);

  client.publish("phoneExchange/newLineStatus", jsonBuffer); // Publish to phoneExchange
  //Serial.println("Published to phoneExchange");
}

// Set external callback function for decoded data
void MQTTHandler::setActionCallback(void (*actionCallback)(int line, uint8_t status)) {
  userActionCallback = actionCallback;
  Serial.println("Action callback set");
}

// Static callback function for PubSubClient to call
void MQTTHandler::mqttCallback(char* topic, unsigned char* payload, unsigned int length) {
  // Check that the instance is set
  Serial.println("MQTT callback called");
  if (!instance) {
    Serial.println("No instance available for handling MQTT messages.");
    return;
  }

  // Convert payload to a null-terminated string
  char message[length + 1] = {0};
  memcpy(message, payload, length);
  message[length] = '\0';

  // Parse JSON
  DynamicJsonDocument jsonDoc(JSON_DOC_SIZE);
  DeserializationError error = deserializeJson(jsonDoc, message);

  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Read JSON content
  int line = jsonDoc["line"];
  uint8_t status = jsonDoc["status"];

  // Call the user's callback
  if (instance->userActionCallback) {
    instance->userActionCallback(line, status);
  } else {
    Serial.println("No action callback defined.");
  }
}

bool MQTTHandler::isConnected() {
    return client.connected(); // Kontrollera om MQTT är ansluten
}