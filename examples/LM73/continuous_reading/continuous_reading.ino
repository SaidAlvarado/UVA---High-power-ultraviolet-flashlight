// Takes a continuous temperature readings
// and displays the results each 1000 mseg
// in the terminal monitor
#include <LM73.h>
#include <Wire.h>

LM73 lm73 = LM73();

void setup()
{
  Serial.begin(9600);

  lm73.begin(LM73_0_I2C_GND);
  lm73.setResolution(LM73_RESOLUTION_14BIT); // 14 bit
  lm73.power(LM73_POWER_ON); // Turn on sensor (continuous temperature conversion)
}

void loop()
{
  delay(1000);

  // Get the temperature
  double temp = lm73.temperature();

  Serial.print("Temperature: ");
  Serial.println(temp, 5);

  Serial.println();
}
