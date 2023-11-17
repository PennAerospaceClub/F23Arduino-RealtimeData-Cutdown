
boolean initTempHu() {
  // Start sht31, returns true if connection was successful.
  return sht31.begin(0x44);
}


void readTempHu() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t)) {  // check if 'is not a number'
    sht31_temp = t;
  }
  
  if (!isnan(h)) {  // check if 'is not a number'
    sht31_humidity = h;
  }
  delay(10);
}
