#include <ArduinoJson.h>
#include<ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include "ESP8266Helpers.h"

#define NTP_TIMEOUT 1500

//NTP Settings
int8_t timeZone = 5;
int8_t minutesTimeZone = 30;
const PROGMEM char *ntpServer = "pool.ntp.org";
boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

//WiFi Credentials
const char* WIFI_USER = "xxxxx";
const char* WIFI_PWD = "xxxxx";

//WiFi Connection Settings
bool autoConnect = false;
bool autoReconnect = true;

//Azure IoT Hub Credentials
const char* THUMBPRINT = "xxxxx";
const char* DEVICE_ID = "xxxxx";
const char* MQTT_HOST = "xxxxx.azure-devices.net";
const char* MQTT_USER = "xxxxx.azure-devices.net/<device-name>/?api-version=2018-06-30";
const char* MQTT_PASS = "SharedAccessSignature sr=xxxxx.azure-devices.net%2Fdevices%2F<device-name>&sig=xxxxx&se=xxxxx";
const char* MQTT_SUB_TOPIC = "devices/<device-name>/messages/devicebound/#";
const char* MQTT_PUB_TOPIC = "devices/<device-name>/messages/events/";

BearSSL::WiFiClientSecure tlsClient;
PubSubClient client(tlsClient);

WiFiEventHandler  wifiStationConnectedEvent, wifiStationDisconnectedEvent;

void setup() {
 
  //Initilaize Built In LED
  initGPIO();

  //Initilaize Serial Port
  initSerialPort();
  
  //Initialize WiFi
  initWiFi(WIFI_USER, WIFI_PWD);

  //Initialize NTP
  NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
      ntpEvent = event;
      syncEventTriggered = true;
  });

  NTP.setInterval (63);
  NTP.setNTPTimeout (NTP_TIMEOUT);
  NTP.begin (ntpServer, timeZone, true, minutesTimeZone);

  //Initialize MQTT Settings
  initMQTT();

  //Connect to Azure IoT Hub
  connectToIoTHub();
}

void loop() {
    
  if(WiFi.isConnected())
  {
    if (syncEventTriggered) {
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }
    
    // Display connection state via LED
    turnOnDigitalPin(DIGITAL_PIN2);
    
    if (!client.connected()) {
     connectToIoTHub();  
    }

    client.loop();
    
    if (client.connected()) 
    {    
    
      //Read the LDR value from sensor
      int ldr =  readLDRSensor();
      const char* ldrPayload = getSensorValuesJSON("ldr",ldr,"percentage");
      Serial.println(ldrPayload);
      client.publish(MQTT_PUB_TOPIC,ldrPayload);
      
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = getHumidity();
      
      // Read temperature as Celsius (the default)
      float t = getTemperature();
      
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }

      const char* humidityPayload = getSensorValuesJSON("humidity",h,"percentage");
      Serial.println(humidityPayload);
      client.publish(MQTT_PUB_TOPIC,humidityPayload);

      const char* tempPayload = getSensorValuesJSON("temperature",t,"celcius");    
      Serial.println(tempPayload);  
      client.publish(MQTT_PUB_TOPIC,tempPayload);
    }
  }
  else
  {
    turnOffDigitalPin(DIGITAL_PIN2);
    reConnectWiFi();
  }
  
  delay(10000);
}
