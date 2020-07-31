#define BAUD_RATE 115200

void initSerialPort()
{
    Serial.begin(BAUD_RATE);
    Serial.setDebugOutput(false);
    //Wait for port to be open
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB
    }
    Serial.println();
    Serial.println("Connected to serial port on baud rate 115200");
}
