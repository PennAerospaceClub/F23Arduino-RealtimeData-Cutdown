  void sanity() {
  //========================================
  //Sanity Declarations ====================
  static boolean sd = false;
  static boolean atm = false;
  static boolean gps_attached = false;
  static boolean lux_attached = false;
  static boolean tempHu = false;

  if (!sd) {
    if (SD.begin(chipSelect)) {
      sd = true;
    }
  }

  if (!lux_attached) {
    if (initLux()) {
      lux_attached = true;
    }
  }
  
  if (!atm) {
    if (initATM()) {
      atm = true;
    }
  }

  //GPS
  if (!gps_attached) {
    gps_attached = true;
  }

  if (!tempHu) {
    if (initTempHu()) {
      tempHu = true;
    }
  }

  //========================================
  //Data Verification ======================
  if (gps_attached) {
    readGPS();
  }
  if (atm) {
    readATM();
  }

  if(lux_attached) {
    readLux();
  }
  
  if (tempHu) {
    readTempHu();
  }
}
