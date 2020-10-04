/*
  VL53L1X:
  Reading distance from the laser based VL53L1X
  This example prints the distance to an object.
  Are you getting weird readings? Be sure the vacuum tape has been removed from the sensor.

  LM73 Temperature Sensor
  Low power one shot temperature conversion
  A4 -> SDA
  A5 -> SCL
  2 -> ALM

  SSD1306 Oled text prints
*/
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <LM73.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels

#define REFRESH_PERIOD 500 // OLED refresh period in ms

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);


SFEVL53L1X distanceSensor;
int distance;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

// Temperature Sensor Object
LM73 lm73 = LM73();
byte lm73_start_time;
double temp;

unsigned long last_refresh;   // ms

int battery_percent = 100;    // %

void setup(void)
{
  Wire.begin();
  Serial.begin(115200);

  // OLED config

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Screen demo
  OledDemo();

  // VL53L1X - Distance sensor config
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    // Clear the buffer
    display.clearDisplay();
    // Show distance and temperature
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 2);
    display.print(F("VL53L1X failed to begin"));
    display.display();
    while (1);
  }
  Serial.println("Sensor online!");

  // LM73 - Temperature sensor config

  lm73.begin(LM73_0_I2C_GND);
  lm73.setResolution(LM73_RESOLUTION_14BIT); // 14 bit
  lm73.power(LM73_POWER_OFF); // Turn off sensor (one shot temperature conversion)

  // Write configuration bytes to initiate measurement
  distanceSensor.startRanging();

  Serial.println("LM73 Starting one shot conversion...");
  // Begin first one shot conversion
  // Don't turn on sensor, that's done automatically
  lm73.startOneShot();

  // lm73 Start time
  lm73_start_time = millis();
}

void loop(void){

  if (distanceSensor.checkForDataReady()){
    distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();

    Serial.print("VL53L1X - Distance(mm): ");
    Serial.print(distance);

    float distanceInches = distance * 0.0393701;
    float distanceFeet = distanceInches / 12.0;

    Serial.print("\tDistance(ft): ");
    Serial.print(distanceFeet, 2);

    Serial.println();

    Serial.println("VL53L1X - Initiate measurement...");

    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  }

  // Wait for lm73 conversion completion
  if(lm73.ready()){
    // Workout conversion time
    byte conversion_time = ((byte)millis()) - lm73_start_time;

    // Get the temperature
    temp = lm73.temperature();

    Serial.print("LM73 - Conversion time(ms): ");
    Serial.println(conversion_time);

    Serial.print("LM73 - Temperature (°C): ");
    Serial.println(temp, 5);

    Serial.println();

    Serial.println("LM73 Starting one shot conversion...");
    // Begin one shot conversion
    // Don't turn on sensor, that's done automatically
    lm73.startOneShot();

    // lm73 Start time
    lm73_start_time = millis();
  }

  if(millis()-last_refresh > REFRESH_PERIOD){
    // Display the updated measurement in the screen
    // Clear the buffer
    display.clearDisplay();

    // Display the distance monitor
    set_distance_monitor(distance);

    // Display the battery charge monitor
    set_battery_charge_monitor(battery_percent);

    // Show distance and temperature
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F("D(mm):"));
    display.println(distance);
    display.print(F("T(°C):"));
    display.println(temp);
    display.display();

    last_refresh = millis();
  }
}

void OledDemo() {

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(5);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(20);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(30);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(50);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(70);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(80);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(120);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(5);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(20);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(40);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(60);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(80);
  display.display(); // Update screen with each newly-drawn line
  delay(5000);
}

void set_distance_monitor(int distance){
  if(distance < 10){
    display.fillCircle(73,32,3, SSD1306_WHITE);
  }else if(distance < 25){
    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);
  }else if(distance < 40){
    display.fillCircle(73,32,15, SSD1306_WHITE);
    display.fillCircle(73,32,13, SSD1306_BLACK);
    display.fillRect(56,15,73,49, SSD1306_BLACK);

    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);
  }else if(distance < 65){
    display.fillCircle(73,32,22, SSD1306_WHITE);
    display.fillCircle(73,32,19, SSD1306_BLACK);
    display.fillRect(51,10,73,54, SSD1306_BLACK);
    display.fillCircle(73,32,15, SSD1306_WHITE);
    display.fillCircle(73,32,13, SSD1306_BLACK);
    display.fillRect(56,15,73,49, SSD1306_BLACK);

    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);
  }else if(distance < 75){
    display.fillCircle(73,32,22, SSD1306_WHITE);
    display.fillCircle(73,32,19, SSD1306_BLACK);
    display.fillRect(51,10,73,54, SSD1306_BLACK);
    display.fillCircle(73,32,15, SSD1306_WHITE);
    display.fillCircle(73,32,13, SSD1306_BLACK);
    display.fillRect(56,15,73,49, SSD1306_BLACK);

    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);

    display.drawLine(50,32,59,32, SSD1306_WHITE);
    display.drawLine(64,42,58,48, SSD1306_WHITE);
    display.drawLine(64,42,58,48, SSD1306_WHITE);
  }else if(distance < 100){
    display.fillCircle(73,32,28, SSD1306_WHITE);
    display.fillCircle(73,32,26, SSD1306_BLACK);
    display.fillRect(45,4,73,60, SSD1306_BLACK);
    display.fillCircle(73,32,22, SSD1306_WHITE);
    display.fillCircle(73,32,19, SSD1306_BLACK);
    display.fillRect(51,10,73,54, SSD1306_BLACK);
    display.fillCircle(73,32,15, SSD1306_WHITE);
    display.fillCircle(73,32,13, SSD1306_BLACK);
    display.fillRect(56,15,73,49, SSD1306_BLACK);

    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);
  }else if(distance < 150){
    display.fillCircle(73,32,32, SSD1306_WHITE);
    display.fillCircle(73,32,29, SSD1306_BLACK);
    display.fillRect(73-32,32-32,73,32+32, SSD1306_BLACK);
    display.fillCircle(73,32,28, SSD1306_WHITE);
    display.fillCircle(73,32,26, SSD1306_BLACK);
    display.fillRect(45,4,73,60, SSD1306_BLACK);
    display.fillCircle(73,32,22, SSD1306_WHITE);
    display.fillCircle(73,32,19, SSD1306_BLACK);
    display.fillRect(51,10,73,54, SSD1306_BLACK);
    display.fillCircle(73,32,15, SSD1306_WHITE);
    display.fillCircle(73,32,13, SSD1306_BLACK);
    display.fillRect(56,15,73,49, SSD1306_BLACK);

    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);
  }else{
    display.fillCircle(73,32,32, SSD1306_WHITE);
    display.fillCircle(73,32,29, SSD1306_BLACK);
    display.fillRect(73-32,32-32,73,32+32, SSD1306_BLACK);

    display.fillCircle(73,32,3, SSD1306_WHITE);
    display.fillCircle(65,32,3, SSD1306_WHITE);
    display.fillCircle(81,32,3, SSD1306_WHITE);
    display.fillCircle(69,24,3, SSD1306_WHITE);
    display.fillCircle(69,40,3, SSD1306_WHITE);
    display.fillCircle(77,24,3, SSD1306_WHITE);
    display.fillCircle(77,40,3, SSD1306_WHITE);
  }
}

void set_battery_charge_monitor(int percent){
  if(percent < 10){
    display.drawRect(114,23,121,1, SSD1306_WHITE);
  }else if(percent < 32){
    display.drawRect(114,23,121,1, SSD1306_WHITE);
    display.fillRect(117,20,118,19, SSD1306_WHITE);
  }else if(percent < 55){
    display.drawRect(114,23,121,1, SSD1306_WHITE);
    display.fillRect(117,20,118,19, SSD1306_WHITE);
    display.fillRect(117,15,118,14, SSD1306_WHITE);
  }else if(percent < 77){
    display.drawRect(114,23,121,1, SSD1306_WHITE);
    display.fillRect(117,20,118,19, SSD1306_WHITE);
    display.fillRect(117,15,118,14, SSD1306_WHITE);
    display.fillRect(117,5,118,4, SSD1306_WHITE);
  }else if(percent < 100){
    display.drawRect(114,23,121,1, SSD1306_WHITE);
    display.fillRect(117,20,118,19, SSD1306_WHITE);
    display.fillRect(117,15,118,14, SSD1306_WHITE);
    display.fillRect(117,5,118,4, SSD1306_WHITE);
    display.fillRect(117,20,118,19, SSD1306_WHITE);
  }
}
