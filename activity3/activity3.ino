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

// Keeping track of SD card;
bool sdEnable = true;
long sdTries = 0;

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
  while (!SD.begin(SD_CS)) {
    Serial.println("Failed to initialize SD card!");
    delay(10);
    sdTries++;
    if(sdTries > 5){
      sdEnable = false;
      Serial.println("Bypassing SD card. Writting to serial only.");
      break;
    }
  }
  if(sdEnable){
    dataFile = SD.open("log-0000.csv", FILE_WRITE);
    Serial.println("SD file initialized");
    // dataFile.println("temperature, humidity, pressure, light, Ax, Ay, Az, Gx, Gy, Gz");  // pressure sensor broken
    dataFile.println("temperature, humidity, light, Ax, Ay, Az, Gx, Gy, Gz");
    dataFile.close();
  }
  carrier.display.setTextColor(ST77XX_WHITE); //white text  
}

void loop() {
  if(sdEnable){
    dataFile = SD.open("log-0000.csv", FILE_WRITE);
    delay(100);
  }

  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();

  while(!carrier.Light.colorAvailable()){
    delay(10);
  }
  carrier.Light.readColor(r,g,b,light);

  // Read acceleration and direction
  while(!carrier.IMUmodule.accelerationAvailable()){
    delay(10);
  }
  carrier.IMUmodule.readAcceleration(Ax, Ay, Az);
  while(!carrier.IMUmodule.gyroscopeAvailable()){
    delay(10);
  }
  carrier.IMUmodule.readGyroscope(Gx, Gy, Gz);

  counter++;

  // Write data to serial and SD card
  printComma(temperature);
  printComma(humidity);
  printComma(light);
  printComma(Ax);
  printComma(Ay);
  printComma(Az);
  printComma(Gx);
  printComma(Gy);
  printComma(Gz);
  Serial.println("");
  if(sdEnable){
    dataFile.println("");
    dataFile.close();
  }

  carrier.display.fillScreen(ST77XX_BLUE);
  carrier.display.setCursor(100, 70);
  carrier.display.setTextSize(4); //medium sized text
  carrier.display.print(counter);
  carrier.display.setCursor(20, 110);
  carrier.display.setTextSize(2);
  carrier.display.print("Measures taken");
 
  // wait 1 second to print again
  delay(1000);
}

void printComma(String text){
  if(sdEnable){
    dataFile.print(text);
    dataFile.print(",");
  }
  Serial.print(text);
  Serial.print(",");
}

void printComma(float value){
  printComma(String(value));
}

void printComma(int value){
  printComma(String(value));
}
