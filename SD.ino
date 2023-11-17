void writeSD(String dataString){
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
}

String mkdata(){
  String data = 
  //Times
   "gpstime:" + String(gpstime) + "," + 
  "gpsdate:" + String(gpsdate) + "," + 
  "gps:" + String(millis()) + "," + 
  //GPS
  "latit:" + String(latit, 6) + "," + 
  "longit:" + String(longit, 6) + "," + 
  "alt:" + String(alt, 6) + "," + 
  "course:" + String(course, 2) + "," + 
  "speed:" + String(speed, 2) + "," + 
  "numsats:" + String(numsats) + "," + 
  "velDiff:" + String(velDiff, 3) + "," + 
  //ATM data
  "pascals:" + String(pascals, 4) + "," +
  "humidity:" + String(humidity, 2) + "," +
  "gas:" + String(gas, 2) + "," +
  "altm:" + String(altm, 2) + "," +
  "tempC:" + String(tempC, 2) + "," +
  //LUX data
  "lum:" + String(lum, 4) + "," +
  "ir:" + String(ir, 4) + "," +
  "lux:" + String(lux, 4) + "," +
  
  // TEMP_HU data
  "temp_new:" + String(sht31_temp, 4) + "," +
  "hum_new:" + String(sht31_humidity, 4);
  
  return data;
}


String mkdataSend(){
  String dataSend = 
  String(latit, 6) + "," + 
  String(longit, 6) + "," + 
  String(alt, 6) + "," + 
  String(tempC, 2) + "," +
  String(speed, 2) + "," + 
  String(numsats);
  return dataSend;
}