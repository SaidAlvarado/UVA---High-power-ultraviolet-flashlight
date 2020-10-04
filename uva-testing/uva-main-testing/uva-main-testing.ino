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

//Temperature Sensor Object
LM73 lm73 = LM73();
byte lm73_start_time;
double temp;
unsigned long last_refresh;   // ms

int battery_percent = 100;    // %

// Define the pin which controls the Led PWM
int pmw_pin = 5;


void setup(void)
{

  //Make sure the LEDs are off
  pinMode(pmw_pin, OUTPUT);
  digitalWrite(pmw_pin, LOW);

  // OLED config

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    while (1);
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.setRotation(3);
  display.clearDisplay();
  display.drawRect(0,0,50,50, SSD1306_WHITE);
  display.display(); // Update screen with each newly-drawn line

  // Clear the buffer
  // Put the display in portrait
  delay(2000);   // Pause for 2 seconds
  display.clearDisplay();

  // Screen demo
  // OledDemo();


    // VL53L1X - Distance sensor config
    Wire.begin();
    distanceSensor.begin();
  //
  //  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  //  {
  //    // Clear the buffer
  //    display.clearDisplay();
  //    // Show distance and temperature
  //    display.setTextSize(2); // Draw 2X-scale text
  //    display.setTextColor(SSD1306_WHITE);
  //    display.setCursor(2, 2);
  //    display.print(F("VL53L1X failed to begin"));
  //    display.display();
  //    while (1);
  //  }

    // Write configuration bytes to initiate measurement
    distanceSensor.startRanging();

  //  LM73 - Temperature sensor config

    lm73.begin(LM73_0_I2C_GND);
    lm73.setResolution(LM73_RESOLUTION_14BIT); // 14 bit
    lm73.power(LM73_POWER_OFF); // Turn off sensor (one shot temperature conversion)


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


      float distanceInches = distance * 0.0393701;
      float distanceFeet = distanceInches / 12.0;




      distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    }

    // Wait for lm73 conversion completion
    if(lm73.ready()){
      // Workout conversion time
      byte conversion_time = ((byte)millis()) - lm73_start_time;

      // Get the temperature
      temp = lm73.temperature();
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
      // set_distance_monitor(distance);

      // Display the battery charge monitor
      // set_battery_charge_monitor(battery_percent);

      // Show distance and temperature
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print(F("D(mm):"));
      display.println(distance);
      display.print(F("T(C):"));
      display.println(temp);
      display.display();

      last_refresh = millis();
    }
}

void OledDemo() {

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(5); 
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.1"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(20);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.2"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(30);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.3"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(50);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.4"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(70);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.5"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(80);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.6"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(120);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.7"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.8"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(5);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("0.9"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(20);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("1.0"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(40);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("1.1"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(60);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("1.2"));
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(80);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(F("1.3"));
  display.display(); // Update screen with each newly-drawn line
  delay(5000);
}

void set_distance_monitor(int distance){
  if(distance < 10){
    display.fillCircle(32,64,3, SSD1306_WHITE);
  }else if(distance >= 10 && distance < 25){
    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);
  }else if(distance >= 25 && distance < 40){
    display.drawCircle(32,64,15, SSD1306_WHITE);
    display.drawCircle(32,64,16, SSD1306_WHITE);
    display.fillRect(0,64,64,20, SSD1306_BLACK);

    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);
  }else if(distance >= 40 && distance < 65){
    display.drawCircle(32,64,21, SSD1306_WHITE);
    display.drawCircle(32,64,20, SSD1306_WHITE);
    display.drawCircle(32,64,19, SSD1306_WHITE);
    display.fillRect(0,64,64,25, SSD1306_BLACK);
    display.drawCircle(32,64,15, SSD1306_WHITE);
    display.drawCircle(32,64,16, SSD1306_WHITE);
    display.fillRect(0,64,64,20, SSD1306_BLACK);

    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);
  }else if(distance >= 60 && distance < 75){
    display.drawCircle(32,64,21, SSD1306_WHITE);
    display.drawCircle(32,64,20, SSD1306_WHITE);
    display.drawCircle(32,64,19, SSD1306_WHITE);
    display.fillRect(0,64,64,25, SSD1306_BLACK);
    display.drawCircle(32,64,15, SSD1306_WHITE);
    display.drawCircle(32,64,16, SSD1306_WHITE);
    display.fillRect(0,64,64,20, SSD1306_BLACK);

    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);

    display.drawLine(32,64+9+8,32,64+9+8+9, SSD1306_WHITE);
    display.drawLine(32+8+8,64+5+5,32+8+8+9,64+5+5+9, SSD1306_WHITE);
    display.drawLine(32-8-8,64+5+5,32-8-8-9,64+5+5+9, SSD1306_WHITE);

  }else if(distance >= 75 && distance < 100){
    display.drawCircle(32,64,25, SSD1306_WHITE);
    display.drawCircle(32,64,24, SSD1306_WHITE);
    display.fillRect(0,64,64,27, SSD1306_BLACK);
    display.drawCircle(32,64,21, SSD1306_WHITE);
    display.drawCircle(32,64,20, SSD1306_WHITE);
    display.drawCircle(32,64,19, SSD1306_WHITE);
    display.fillRect(0,64,64,25, SSD1306_BLACK);
    display.drawCircle(32,64,15, SSD1306_WHITE);
    display.drawCircle(32,64,16, SSD1306_WHITE);
    display.fillRect(0,64,64,20, SSD1306_BLACK);

    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);
  }else if(distance >= 100 && distance < 150){
    display.drawCircle(32,64,30, SSD1306_WHITE);
    display.drawCircle(32,64,29, SSD1306_WHITE);
    display.drawCircle(32,64,28, SSD1306_WHITE);
    display.fillRect(0,64,64,30, SSD1306_BLACK);
    display.drawCircle(32,64,25, SSD1306_WHITE);
    display.drawCircle(32,64,24, SSD1306_WHITE);
    display.fillRect(0,64,64,27, SSD1306_BLACK);
    display.drawCircle(32,64,21, SSD1306_WHITE);
    display.drawCircle(32,64,20, SSD1306_WHITE);
    display.drawCircle(32,64,19, SSD1306_WHITE);
    display.fillRect(0,64,64,25, SSD1306_BLACK);
    display.drawCircle(32,64,15, SSD1306_WHITE);
    display.drawCircle(32,64,16, SSD1306_WHITE);
    display.fillRect(0,64,64,20, SSD1306_BLACK);

    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);
  }else{
    display.drawCircle(32,64,29, SSD1306_WHITE);
    display.drawCircle(32,64,28, SSD1306_WHITE);
    display.drawCircle(32,64,27, SSD1306_WHITE);
    display.fillRect(0,64,64,30, SSD1306_BLACK);

    display.fillCircle(32  ,64  ,3, SSD1306_WHITE);
    display.fillCircle(32  ,64-9,3, SSD1306_WHITE);
    display.fillCircle(32  ,64+9,3, SSD1306_WHITE);
    display.fillCircle(32+8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32+8,64-5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64+5,3, SSD1306_WHITE);
    display.fillCircle(32-8,64-5,3, SSD1306_WHITE);
  }
}

void set_battery_charge_monitor(int percent){
  if(percent < 10){
    display.drawRect(41,3,20,9, SSD1306_WHITE);
  }else if(percent < 32){
    display.drawRect(41,3,20,9, SSD1306_WHITE);
    display.fillRect(56,6,3,3, SSD1306_WHITE);
  }else if(percent >= 32 && percent < 55){
    display.drawRect(41,3,20,9, SSD1306_WHITE);
    display.fillRect(56,6,3,3, SSD1306_WHITE);
    display.fillRect(52,6,3,3, SSD1306_WHITE);
  }else if(percent >= 55 && percent < 77){
    display.drawRect(41,3,20,9, SSD1306_WHITE);
    display.fillRect(56,6,3,3, SSD1306_WHITE);
    display.fillRect(52,6,3,3, SSD1306_WHITE);
    display.fillRect(48,6,3,3, SSD1306_WHITE);
  }else if(percent >= 77 && percent < 100){
    display.drawRect(41,3,20,9, SSD1306_WHITE);
    display.fillRect(56,6,3,3, SSD1306_WHITE);
    display.fillRect(52,6,3,3, SSD1306_WHITE);
    display.fillRect(48,6,3,3, SSD1306_WHITE);
    display.fillRect(44,6,3,3, SSD1306_WHITE);
  }
}
