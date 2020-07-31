
int readLDRSensor(){
  int ldrValue = map(readAnalogPin(),0,1024,1,100);
  return ldrValue;
}
