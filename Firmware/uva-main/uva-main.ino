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

#define REFRESH_PERIOD 200 // OLED refresh period in ms

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

//Batery meter
int battery_percent;    // %
int bat_pin = A2;
int bat_adc;

// Define the pin which controls the Led PWM
int pmw_pin = 5;
int pwm_power = 127; // for 100mW/cm^2
// int pwm_power = 20;   // for 20mW/cm^2
//int pwm_power = 0; // para 100mW/cm^2

// Set tmer for cronometer
unsigned long start_time;

void setup(void)
{

  //Make sure the LEDs are off
  pinMode(pmw_pin, OUTPUT);
  analogWrite(pmw_pin, pwm_power);

  // OLED config
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    while (1);
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.setRotation(3);
  display.clearDisplay();

  // Screen demo to test all the functionallity of the lamp
  // OledDemo();


    // VL53L1X - Distance sensor config
    Wire.begin();
    distanceSensor.begin();
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
    start_time = millis();
}

void loop(void){


    if (distanceSensor.checkForDataReady()){
      distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
      distanceSensor.clearInterrupt();
      distanceSensor.stopRanging();

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

    // If leds overheat - severly cutdown the power.
    if (temp >= 60.0) analogWrite(pmw_pin, 13);


    // Check the battery charge
    bat_adc = analogRead(bat_pin);
    battery_percent = (int)(bat_adc - 775) * 100 / 202;
    // Safety check of the battery
   if (battery_percent < 10) {

     //Shut down leds, and stop interface
     analogWrite(pmw_pin, 0);
     distance = 0;
     start_time = millis();

   }


    if(millis()-last_refresh > REFRESH_PERIOD){
      // Display the updated measurement in the screen
      // Clear the buffer
      display.clearDisplay();

      // Display UVA text at the top left corner.
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(3, 3);
      display.println(F("UVA"));

      // Display the distance monitor
      set_distance_monitor(distance);

      // Display the battery charge monitor
      set_battery_charge_monitor(battery_percent);

      // Set timer value
      set_cronometer_monitor(millis()-start_time);

      // Show distance and temperature
      display.display();

      last_refresh = millis();
    }
}

void OledDemo() {

  //Shut down the lamp if you enter here
  analogWrite(pmw_pin, 0);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(5);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(0);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(20);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(900);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(30);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(1000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(50);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(1100);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(70);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(99000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(80);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(100000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(120);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(101000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(102000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(5);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(996000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(20);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(997000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(40);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(998000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(60);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(999000);
  display.display(); // Update screen with each newly-drawn line
  delay(1000);

  display.clearDisplay(); // Clear display buffer
  set_distance_monitor(160);
  set_battery_charge_monitor(80);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 3);
  display.println(F("UVA"));
  set_cronometer_monitor(1000000);
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
    display.fillRect(0,64,64,30, SSD1306_BLACK);

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
  }else if(percent >= 77 && percent <= 100){
    display.drawRect(41,3,20,9, SSD1306_WHITE);
    display.fillRect(56,6,3,3, SSD1306_WHITE);
    display.fillRect(52,6,3,3, SSD1306_WHITE);
    display.fillRect(48,6,3,3, SSD1306_WHITE);
    display.fillRect(44,6,3,3, SSD1306_WHITE);
  }
}

void set_cronometer_monitor(unsigned long time_since_start){
  float time_diff = time_since_start/1000.0;
  String cronom_value = "";
  if(time_diff < 10.0){
    cronom_value = String(time_diff, 1);
  }else if (time_diff >= 10.0 && time_diff < 100.0){
    cronom_value = "0" + String(floor(time_diff),0);
  }else if (time_diff >= 100.0 && time_diff < 1000.0){
    cronom_value = String(floor(time_diff),0);
  }else{
    cronom_value = "999";
  }
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 100);
  display.println(cronom_value);
}
