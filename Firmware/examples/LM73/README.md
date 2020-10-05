# LM73-Q1 Temperature Sensor #

* [Library](https://github.com/zkemble/LM73)

## Installation ##

1. Download the [latest release archive from GitHub](https://github.com/zkemble/LM73/archive/master.zip) and decompress it.
2. Move the "LM73-master/arduino/LM73" folder into the "libraries" directory inside your Arduino sketchbook directory.
3. If you are using Arduino 1.5 or higher: create the folder LM73/src, move the .cpp and .h files into the src folder.
4. After installing the library, restart the Arduino IDE.

## Examples ##

* [Oneshot](./oneshot/oneshot.ino): this is an example from the [LM73 Library](https://github.com/zkemble/LM73) that periodically starts a new reading, waits until the measurement is ready and then shows the results in the serial monitor.

- [Continuous Reading](./continuous_reading/continuous_reading.ino): this example takes a continuous temperature readings and displays the results each 1000 mseg in the terminal monitor (9600 Baud).
