#include <Wire.h>               //OLED/LUX
#include <SPI.h>                //SD/ATM
#include <SD.h>                 //SD
#include <TinyGPS++.h>          //GPS
#include <Adafruit_SSD1306.h>   //OLED
#include <Adafruit_Sensor.h>    //ATM/LUX
#include <Adafruit_BME680.h>    //ATM
#include "Adafruit_TSL2591.h"   //LUX
#include "Adafruit_SHT31.h"     // TEMP/HU

#include <IridiumSBD.h>
#define IridiumSerial Serial3
#define DIAGNOSTICS false // Change this to see diagnostics

//=====================================
//ATM =================================
#define SEALEVELPRESSURE_HPA (1013.2)
#define BME_SCK 40
#define BME_MISO 41
#define BME_MOSI 42
#define BME_CS 43
Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

//Rockblock variables =======================
IridiumSBD modem(IridiumSerial);

uint8_t buffer[200]={ 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };

int buzzerPin = 11;

int sq;

bool sendSuccess;

uint8_t bufferSend[270];
size_t bufferSizeSend = 0;

//=====================================

//ATM variables =======================
float pascals;
float altm;
float humidity;
float gas;
float tempC;

//=====================================
//LUX =================================
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

//LUX variables =======================
uint32_t lum;
int16_t ir, full;
float lux;

 TinyGPSPlus gps;
double latit;   //degrees
double longit;  //degrees
double alt;     //meters
double alto;
double velDiff;
double vel[] = {0, 0, 0, 0, 0}; //meters
long gpstime;   //HHMMSSCC
long gpsdate;   //DDMMYY
int numsats;
double course;  //degrees
double speed;   //mph

//SD Card ==================================
#define chipSelect 53

//TEMP/HU ==================================
Adafruit_SHT31 sht31 = Adafruit_SHT31();
double sht31_temp;
double sht31_humidity;

void setup() {
  //Serial
  Serial.begin(115200); //Computer
  Serial1.begin(115200); //cutter, change wired connections
  Serial2.begin(9600); //GPS

  //rockblock
  pinMode(buzzerPin, OUTPUT);
  while (!Serial);
  IridiumSerial.begin(19200);

  // Setup the Iridium modem
  Serial.println("Hello! Checking signal quality...");
  modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);
  if (modem.begin() != ISBD_SUCCESS)
  {
    Serial.println("Couldn't begin modem operations.");
    exit(0);
  }
  //rockblock

}

static bool messageSent = false;

void loop() {
  // put your main code here, to run repeatedly:
  sanity();
  String data = mkdata();
  Serial.println(data);
  writeSD(data);
  delay(100);

  String dataSend = mkdataSend();
  char outBuffer[270];
  for (int i = 0; i < dataSend.length(); i++) {
    outBuffer[i] = dataSend[i];
  }

  uint8_t rxBuffer[270] = {0};
  for (int i = 0; i < dataSend.length(); i++) {
    rxBuffer[i] = outBuffer[i];
  }

  bufferSizeSend = sizeof(bufferSend);

  int signalQuality = -1;
  // Check the signal quality
  int errS = modem.getSignalQuality(signalQuality);
  if (errS != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(errS);
    exit(1);
  }
  Serial.print("Signal quality is ");
  Serial.println(signalQuality);
  sq = signalQuality;
  
  //indentation?????
      // Control the active buzzer to beep 'x' times in a group
for (int j = 0; j < 3; j++){
  for (int i = 0; i < sq+1; i++) {
    // Beep (generate a tone) for a specific duration
    beep(1000, 500); // You can adjust the frequency and duration
    delay(100); // Delay between beeps (you can adjust this too)
  }
  // Add a longer delay between groups if needed
}

if (sq > 0){
  delay(1000); 
  int err;
  Serial.println("Waiting for messages...");
  beep(300, 2000);
  // Read/Write the first time or if there are any remaining messages
  //if (!messageSent || modem.getWaitingMessageCount() > 0)
  //{
size_t bufferSize = sizeof(buffer);

  //  if (!messageSent)
    //  err = modem.sendReceiveSBDBinary(buffer, 11, buffer, bufferSize);
    //else
 err = modem.sendReceiveSBDText(NULL, buffer, bufferSize);

    if (err != ISBD_SUCCESS)
    {
      //Serial.print("sendReceiveSBD* failed: error ");
      beep(2700, 500);
      beep(2100, 500);
      beep(1500, 500);
      beep(900, 500);
      beep(300, 500);
      Serial.print("Timeout after 30 seconds...Probably due to bad signal quality. Error code: ");
      Serial.println(err);
      Serial.println("Checking signal quality again...");
    }
    else // success!
    {
      //messageSent = true;
      Serial.println();
      Serial.print("Inbound buffer size is ");
      Serial.println(bufferSize);
      char mes[strlen(buffer) +1];
      for (int i=0; i<bufferSize; ++i)
      {
        Serial.print(buffer[i], HEX);
        if (isprint(buffer[i]))
        {
          Serial.print("(");
          Serial.write(buffer[i]);
          Serial.print(")");
        }
        mes[i] = buffer[i];

      }
      mes[strlen(buffer)] = '\0';
      Serial.println();
      Serial.print("The full message received is: ");
      Serial.println(mes);
      //change coressponding wiring
      Serial1.write(mes);
      beep(300, 500);
      beep(900, 500);
      beep(1500, 500);
      beep(2100, 500);
      beep(2700, 500);
      Serial.println();
      Serial.print("Messages sent to cutter!");
      Serial.println();
      Serial.print("Messages remaining to be retrieved: ");
      Serial.println(modem.getWaitingMessageCount());

          int senderr;
          Serial.println("Sending data to control...");
          beep(1000,2000);
          senderr = modem.sendReceiveSBDBinary(rxBuffer, dataSend.length(), bufferSend, bufferSizeSend);
          if (senderr != ISBD_SUCCESS)
          {
                beep(3700, 1000);
                beep(3100, 1000);
                beep(2500, 1000);
                Serial.print("Unable to send data to control after 30 seconds...Probably due to bad signal quality. Error code: ");
                Serial.println(senderr);
                sendSuccess = false;
          }
          else{
            Serial.print("Message successfully sent to control is: ");
            Serial.println(dataSend);
            sendSuccess = true;
          }

    }
  //}
}


}

// Function to generate a beep with a specific frequency and duration
void beep(int frequency, int duration) {
    tone(buzzerPin, frequency);
    delay(duration);
    noTone(buzzerPin);
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
#endif
