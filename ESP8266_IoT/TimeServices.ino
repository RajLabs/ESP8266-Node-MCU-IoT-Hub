void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
        Serial.printf ("Time Sync error: %d\n", ntpEvent);
        if (ntpEvent == noResponse)
            Serial.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println ("Invalid NTP server address");
        else if (ntpEvent == errorSending)
            Serial.println ("Error sending request");
        else if (ntpEvent == responseError)
            Serial.println ("NTP response error");
    } else {
        if (ntpEvent == timeSyncd) {
            Serial.print ("Got NTP time: ");
            Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
        }
    }
}
