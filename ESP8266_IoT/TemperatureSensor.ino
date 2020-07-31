// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
DHT dht(DIGITAL_PIN4, DHTTYPE);

float getHumidity(){
  return dht.readHumidity();
}

float getTemperature(){
  return dht.readTemperature();
}

float getTemperatureInFarenHeit(){
  // Read temperature as Fahrenheit (isFahrenheit = true)
  return dht.readTemperature(true);
}

float getHeatIndex(){
  // Compute heat index in Fahrenheit (the default)
  float h = getHumidity();
  float f = getTemperatureInFarenHeit();
  return dht.computeHeatIndex(f, h);
}

float getHeatIndexInCelsius(){
  // Compute heat index in Fahrenheit (the default)
  float h = getHumidity();
  float t = getTemperature();
  return dht.computeHeatIndex(t, h, false);
}
