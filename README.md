# UVA Firmware #

Main structure for the UVA, and examples for each sensor.

## Requirements ##

1. [LM73-Q1 Temperature Sensor](examples/LM73/README.md)

2. [VL53L1X Distance Sensor](examples/VL53L1X/README.md)

3. [SSD1306 OLED](examples/SSD1306/README.md)

## Examples ##

### [LM73-Q1 Temperature Sensor](examples/LM73/README.md) ###

- [Oneshot](examples/LM73/oneshot/oneshot.ino): this is an example from the [LM73 Library](https://github.com/zkemble/LM73) that starts a new reading, waits until the temperature measurement is ready and then shows the results in the serial monitor. Once the measurement is ready, it starts a new one and the cycle begins again.

### [VL53L1X Distance Sensor](examples/VL53L1X/README.md) ###

- [ReadDistance](examples/VL53L1X/Example1_ReadDistance/Example1_ReadDistance.ino): this is an example from the [VL53L1X Library](https://github.com/sparkfun/SparkFun_VL53L1X_Arduino_Library) that starts a new reading, waits until the distance measurement is ready and then shows the results in the serial monitor. Once the measurement is ready, it starts a new one and the cycle begins again.

### [SSD1306 OLED](examples/SSD1306/README.md) ###

- [SSD1306 128x64 I2C Demo](examples/SSD1306/ssd1306_128x64_i2c/ssd1306_128x64_i2c.ino): this is an example from the [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306) that plays a demo, it tests all the different functions in the library.

## Main ##

- [uva-main](uva-main/uva-main.ino): The code starts measurements from the temperature sensor and the distance sensor, and waits until each one finishes the measurement, once it is ready, it prints the results in the serial monitor and starts new conversions. The code also prints a text message in the screen (just to test that the library is working correctly).
