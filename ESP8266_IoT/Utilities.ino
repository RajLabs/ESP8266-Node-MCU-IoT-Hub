
const char* getSensorValuesJSON(char* sensor, float value, char* uom){
  
  const size_t capacity = JSON_OBJECT_SIZE(20);
  DynamicJsonDocument doc(capacity);

  String timeStamp = NTP.getTimeDateString().c_str();

  doc["sensor"] = sensor;
  doc["data"] = value;
  doc["uom"] = uom;
  doc["timeStamp"] = timeStamp;
  
  String output;
  serializeJson(doc, output);
  return output.c_str();
}
