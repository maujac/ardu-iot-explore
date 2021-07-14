#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

float temperature = 0;
float humidity = 0;
// float pressure = 0; // pressure sensor broken
int light = 0;
int r,g,b;
 
// gyroscope
float Gx, Gy, Gz;
//accelerometer
float Ax, Ay, Az;

//track how many measurements have been made
int counter = 0;

// file object
File dataFile;

void setup() {

  Serial.begin(9600);
  Serial.println("Initializing carrier");
  while(!Serial){
    delay(100);
  }
  CARRIER_CASE = false;
  if(!carrier.begin()){  //It will see any sensor failure
   Serial.println("Failure on init");
   while(1);
  }

  // init SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("Failed to initialize SD card!");
    while (1);
  }
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
  Serial.println("SD file initialized");
  // dataFile.println("temperature, humidity, pressure, light, Ax, Ay, Az, Gx, Gy, Gz");  // pressure sensor broken
  dataFile.println("temperature, humidity, light, Ax, Ay, Az, Gx, Gy, Gz");
  dataFile.close();
}

void loop() {
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
  delay(100);
 
  temperature = 

}
