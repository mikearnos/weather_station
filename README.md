# Ten Dollar portable Arduino Weather Station

original idea and code from
http://www.instructables.com/id/10-Portable-Arduino-Weather-Station-AWS/

---

#### (1) Download and Save the New LCD library .zip file and SdFat below, they are not in the Arduino Library Manager:

  * [fmalpartida I2C LCD library version 1.3.4](https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/NewliquidCrystal_1.3.4.zip)

  * [SdFat - Arduino FAT16/FAT32 Library](https://github.com/greiman/SdFat/archive/master.zip)
   by Bill Greiman, updated 21 Mar 2015 


#### (2) Use the **Arduino Library Manager** to install OR download and Save the 4 libraries below:

  * [DHT sensor library](https://github.com/adafruit/DHT-sensor-library/archive/master.zip)
    by Adafruit Version 1.2.3

  * [Adafruit BMP280 Library](https://github.com/adafruit/Adafruit_BMP280_Library/archive/master.zip)
    by Adafruit Version 1.0.2

  * [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor/archive/master.zip)
    by Adafruit Version 1.0.2

  * [Sodaq_DS3231](https://github.com/SodaqMoja/Sodaq_DS3231/archive/master.zip)
    by SODAQ Version 1.2.1


#### (3) Install the libraries:

 In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library...  
 Select the .zip files one by one to install. No need to do this if using the Arduino Library Manager.  
 For SdFat you need to open the .zip file, open the SdFat-master folder, and extract the SdFat folder
 to your library folder.

 More information on library installation https://www.arduino.cc/en/Guide/Libraries


#### (4) RTC and SD card

 cut trace on RTC board to prevent charging of CR2032 battery!  
 make sure SD card board has a 3.3V voltage regulator, SD cards are 3.3V


#### (5) Upload AWS_tester.ino.

 Uncomment the #define for the DHT11 or DHT22, pick the one you have. LCD should blink once and
 display LCD I2C address if connected properly. Adjust LCD contrast on back if nothing shows on
 screen. Check the Arduino Serial Monitor to get sensor data. If all hardware is working, upload
 weather.ino to complete the project.


## LCD
```
 Gnd (1) to Gnd
 Vcc (2) to 5V
 SDA (3) to A4 of Arduino
 SCL (4) to A5 of Arduino
```

## DHT11/DHT22
```
  Vcc (1) to 5V
 Data (2) to D2 of arduino (add 4.7k to 10k pullup resistor to 5V)
  n/a (3)
  Gnd (4) to Gnd

* 10k pullup on data line
* 3.3 - 5.5 volts
* stand-by 40 - 50 ua
* measuring 1.3 - 2.1 ma
* collecting period 2 seconds
```

## Barometer And Pressure sensor BMP280
```
 Vcc (1) to 3.3V of Arduino (do not connect to 5V!)
 Gnd (2) to Gnd
 SCL (3) to A5 of Arduino
 SDA (4) to A4 of Arduino
 CSB (5) to 3.3V for I2C mode
 SD0 (6) to 3.3V for I2C address 0x77 (connect to Gnd for I2C address 0x76)

* protect from ambient light, 3.3V only, not 5V tolerant
* I2C Address is specified in library file Adafruit_BMP280.h as 0x77
* 2.7 ua @ 1hz sampling
* 0.3 ua sleep current
```

## RTC 3231
```
 SCL (3) to A5 of Arduino
 SDA (4) to A4 of Arduino
 Vcc (5) to 5V
 Gnd (6) to Gnd

* Note for I2C address 0x57
* cut the trace to the resistor on the right of the board above SCL
* be sure to change example library code Serial.begin to 9600
```

## SD Card Reader
```
 CS   (1) to D4 of Arduino
 SCK  (2) to D13 of Arduino
 MOSI (3) to D11 of Arduino
 MISO (4) to D12 of Arduino
 Vcc  (5) to 5V
 Gnd  (6) to Gnd
```
