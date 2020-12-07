# TemperatureLogger

This code is based around a [ESP-WROVER-KIT v4.1](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/get-started-wrover-kit.html) with various temperature, humidity and light sensors to log environmental conditions at an aquatic facility.

## Introduction

This project leans heavily on the [ESPUI](https://github.com/s00500/ESPUI) library, which allows for a web based inferface to host the current sensor reads, graphs and settings of the device.

This device also utilises the TFT-LCD screen to display the current sensor readings, as well as the current time.

The data collected is in 15 min increments over a 12 month period, where the oldest data is discarded as new data is added.

## Required Components

The required components for this device are as follows:

* 1x [ESP-WROVER-KIT v4.1](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/get-started-wrover-kit.html)
* 2x [PCA9615 I2C Bus extender](https://learn.sparkfun.com/tutorials/qwiic-differential-i2c-bus-extender-pca9615-hookup-guide/all)
* 3x [DS18B20 1-Wire Temperature Sensor](https://www.adafruit.com/product/381)
* 1x [VEML7700 I2C Lux Sensor](https://www.adafruit.com/product/416)
* 1x [BME280 I2C Temperature & Humidity Sensor](https://www.adafruit.com/product/2652)
*1x [DS3231 I2C Precision RTC](https://au.mouser.com/ProductDetail/Adafruit/3295?qs=0X%2FVfLRQkePjC%2FDwiF4rig%3D%3D&vip=1&gclid=EAIaIQobChMIytKE5IS77QIVb8FMAh27NAZAEAQYAiABEgINjPD_BwE)
