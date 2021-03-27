// #===========================================================================#
// ‖ Author: Luis Alejandro Domínguez Bueno                http://ladbsoft.com ‖
// ‖ Date: 2019-03-20                                             Version: 1.1 ‖
// #===========================================================================#
// ‖ Name: ESP8266 MQTT Universal Relay Controller                             ‖
// ‖ Description: A small sketch for the ESP8266 (ESP-01 to be exact) for      ‖
// ‖ implementing a simple relay controller using MQTT as the communication    ‖
// ‖ protocol. This will allow you to turn on and off the power of any device. ‖
// #===========================================================================#
// ‖ Version history:                                                          ‖
// #===========================================================================#
// ‖ 1.1: Added timeout to the WiFi configuration mode.                        ‖
// ‖ 1.0: Added watchdog.                                                      ‖
// ‖ 0.2a: WifiManager implemented. Maintaining the WiFi password without      ‖
// ‖ reprogramming is now possible. Power state is saved in EEPROM, in order   ‖
// ‖ resume the status if power is restarted.                                  ‖
// ‖ 0.1a: Start of development. Connection to the MQTT server and             ‖
// ‖ communication tests.                                                      ‖
// #===========================================================================#

#define DEBUG false

// +---------------------------------------------------------------------------+
// |                             I N C L U D E S                               |
// +---------------------------------------------------------------------------+
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Configuration.h"

// +---------------------------------------------------------------------------+
// |                              G L O B A L S                                |
// +---------------------------------------------------------------------------+

WiFiClient espClient;
PubSubClient client(espClient);
long lastCheck   = 0;
bool isPoweredOn = false;

// +---------------------------------------------------------------------------+
// |                                S E T U P                                  |
// +---------------------------------------------------------------------------+

void setup() {
  pinMode(powerPin, OUTPUT);   // For controlling the relay

  #if DEBUG
  Serial.begin(115200);
  #endif

  initPowerState();

  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  //Reenable watchdog
  ESP.wdtEnable(5000);
}

// +---------------------------------------------------------------------------+
// |                                 L O O P                                   |
// +---------------------------------------------------------------------------+

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  //Send status every 5 seconds
  if (now - lastCheck > 5000) {
    client.publish(mqttStatusTopic, "ON");

    if (isPoweredOn) {
      client.publish(mqttPowerStateTopic, "ON");
    } else {
      client.publish(mqttPowerStateTopic, "OFF");
    }
    
    #if DEBUG
    Serial.println("Status: ON");
    #endif
    
    lastCheck = now;
  }

  //Feed the watchdog
  ESP.wdtFeed();
}

// +---------------------------------------------------------------------------+
// |                          S U B R O U T I N E S                            |
// +---------------------------------------------------------------------------+

void setup_wifi() {
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);

  if(!wifiManager.autoConnect(wifiSsid, wifiPassword)) {
    #if DEBUG
    Serial.println("Failed to connect to WiFi. Resetting...");
    delay(3000);
    #endif

    //Retry after 3 minutes with no WiFi connection
    ESP.reset();
    delay(5000);
  } 
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Add \0 to payload, to convert it to string later
  byte* command = (byte*)malloc(length+1);
  memcpy(command, payload, length);
  command[length] = '\0';

  #if DEBUG
  Serial.print("Topic ");
  Serial.print(topic);
  Serial.print(" received \"");
  Serial.print(String((char*)command));
  Serial.println("\"");
  #endif

  if(String(topic).equals(String(mqttPowerCommandTopic))) {
    if (String((char*)command).equals("ON")) {
      powerOn();
    } else if (String((char*)command).equals("OFF")) {
      powerOff();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    #if DEBUG
    Serial.println("Trying to connect...");
    #endif
    if (client.connect(mqttClientId, mqttUser, mqttPassword)) {
      // Once connected, resubscribe
      client.subscribe(mqttPowerCommandTopic);
      #if DEBUG
      Serial.print("Subscribed to ");
      Serial.println(mqttPowerCommandTopic);
      #endif
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
      #if DEBUG
      Serial.println("Couldn't connect. Retrying in 5 seconds");
      #endif
    }
  }
}

void initPowerState() {
  EEPROM.begin(1);

  if (readPowerState()) {
    isPoweredOn = true;
    digitalWrite(powerPin, LOW); //LOW = ON
  } else {
    isPoweredOn = false;
    digitalWrite(powerPin, HIGH); //LOW = OFF
  }
}

bool readPowerState() {
  return EEPROM.read(0) == 1;
}

void writePowerState(bool state) {
  EEPROM.write(0, state);
  EEPROM.commit();
}

void powerOn() {
  isPoweredOn = true;
  
  digitalWrite(powerPin, LOW); //LOW = ON
  writePowerState(true);

  client.publish(mqttPowerStateTopic, "ON");
  #if DEBUG
  Serial.println("Power set to ON");
  #endif
}

void powerOff() {
  isPoweredOn = false;
  
  digitalWrite(powerPin, HIGH); //HIGH = OFF
  writePowerState(false);

  client.publish(mqttPowerStateTopic, "OFF");
  #if DEBUG
  Serial.println("Power set to OFF");
  #endif
}
