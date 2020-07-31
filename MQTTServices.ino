
void OnMessageCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
    Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
    //do connect upon WiFi connected
    connectToIoTHub();
}

void onSTADisconnected(WiFiEventStationModeDisconnected event_info) {
    Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
    Serial.printf("Reason: %d\n", event_info.reason);
}

void connectToIoTHub() {
  while (!client.connected()) {
      Serial.print("Connecting to MQTT Server ... ");
      if (client.connect(DEVICE_ID, MQTT_USER, MQTT_PASS)) {
        Serial.println("connected.");
        client.subscribe(MQTT_SUB_TOPIC);
      } else {
        Serial.print("failed, status code =");
        Serial.print(client.state());
        Serial.println(". Try again in 5 seconds.");
        /* Wait 5 seconds before retrying */
        delay(5000);
      }
    }
}

void initMQTT()
{
  tlsClient.setFingerprint(THUMBPRINT);
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(OnMessageCallback);
}
