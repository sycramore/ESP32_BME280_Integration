# ESP32_BME280_Integration
A driver to connect Bosch's BME280 temperature, humidity and pressure sensor to the ESP32 microcontroller platform

## Used files

* Espressif Basic I2C master example code from IDF development kit v. 6.0 (The original version uses the MPU9250 inertial measurement unit)
* BME280_SensorAPI files bme280.c, bme280.h and bme280_defs.h accessed at 21.09.2026. The files can be found in /components/bme280.

### Choice of used files

The first setup should deliver fast results and working files. I will integrate the official Bosch repo later. Given my own experience with dependencies that lead to code failure due to changes in the dependencies I want to have one working version before integrating dependencies whose maintainence I do not control. This allows me to have a back up version that works in case changes in the dependencies cause my code to fail.
