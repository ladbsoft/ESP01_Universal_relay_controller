// +---------------------------------------------------------------------------+
// |                            C O N S T A N T S                              |
// +---------------------------------------------------------------------------+

// MQTT
const char* mqttServer            = "<YourMQTTServerIP>";
const int   mqttPort              = 1883;
const char* mqttUser              = "<YourMQTTUser>";
const char* mqttPassword          = "<YourMQTTPassword>";
const char* mqttClientId          = "<ANewMQTTClientID>";
const char* mqttStatusTopic       = "<ANewMQTTTopic>";
const char* mqttPowerStateTopic   = "<ANewMQTTTopic2>";
const char* mqttPowerCommandTopic = "<ANewMQTTTopic3>";

// GPIO
const byte  powerPin              = 0;

// WI-FI CONFIGURATION NETWORK
const char* wifiSsid              = "<NewSSID>";
const char* wifiPassword          = "<NewPassword>";
