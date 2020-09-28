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
*/
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <LM73.h>

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

// Temperature Sensor Object
LM73 lm73 = LM73();
byte lm73_start_time;

void setup(void)
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");

  lm73.begin(LM73_0_I2C_FLOAT);
  lm73.setResolution(LM73_RESOLUTION_14BIT); // 14 bit
  lm73.power(LM73_POWER_OFF); // Turn off sensor (one shot temperature conversion)

  Serial.println("VL53L1X - Initiate measurement...");
  
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
    int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
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
    byte time = ((byte)millis()) - lm73_start_time;

    // Get the temperature
    double temp = lm73.temperature();

    Serial.print("LM73 - Conversion time: ");
    Serial.println(time);

    Serial.print("LM73 - Temperature: ");
    Serial.println(temp, 5);

    Serial.println();

    Serial.println("LM73 Starting one shot conversion...");
    // Begin one shot conversion 
    // Don't turn on sensor, that's done automatically
    lm73.startOneShot();

    // lm73 Start time
    lm73_start_time = millis();
  }
}
