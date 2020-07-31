
void initSSL()
{
   tlsClient.setFingerprint(THUMBPRINT);
}

void initWiFi(const char* ssid, const char* pwd)
{
  // Mark the WiFi to be on station mode
  WiFi.mode(WIFI_STA);

  // Mark device auto connect to false during debug
  WiFi.setAutoConnect(autoConnect);

  // Mark device to reconnect automatically when the Wifi access point is available
  WiFi.setAutoReconnect(autoReconnect);

  initMQTT();

  // Establish WiFi connection to the given access point
  WiFi.begin(ssid,pwd);


  //Handle WiFi events
//  wifiStationConnectedEvent = WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start the Client
//  wifiStationDisconnectedEvent = WiFi.onStationModeDisconnected(onSTADisconnected);

  Serial.println("Please wait while you are connecting to wifi...");
  int connectionStatus = WiFi.waitForConnectResult();

  if (connectionStatus == WL_CONNECTED)
  {
    //Print Connection Info
    Serial.println("Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.printf("Mac address: %s\n", WiFi.macAddress().c_str());
  }
  else
  {
    Serial.printf("Failed connecting to WiFi: %s\n", ssid);
    Serial.printf("Connection status: %s\n", getWiFiStatus().c_str());
  }
}

void reConnectWiFi()
{
    Serial.printf("Attempting to connect : %s\n", WIFI_USER);
    WiFi.reconnect();
    int connectionStatus = WiFi.waitForConnectResult();
    Serial.printf("Connection status: %s\n", getWiFiStatus().c_str());  
}

String getWiFiStatus(){
  switch(WiFi.status())
  {
    case 0:
      // WL_IDLE_STATUS
      return "Idle"; 
      break;  
    case 1:
      // WL_NO_SSID_AVAILABLE
      return "SSID Not Available"; 
      break;  
    case 2:
      // WL_SCAN_COMPLETED
      return "Scan Completed"; 
      break;  
    case 3:
      // WL_CONNECTED
      return "Connected"; 
      break;  
    case 4:
      // WL_CONNECT_FAILED
      return "Connection Failed"; 
      break;  
    case 5:
      // WL_CONNECTION_LOST
      return "Connection Lost"; 
      break;  
    case 6:
      // WL_DISCONNECTED
      return "Disconnected"; 
      break;     
    case 255:
      // WL_NO_SHIELD
      return "No Sheild"; 
      break;           
    default:
      return "Unknown";
    break;
  }
}
