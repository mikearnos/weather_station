/*-----------------------------------\
|  10 Dollar Arduino Weather Station |
|  hardware test code v1.2           |
\-----------------------------------*/

#include <Wire.h>                 // Needed for I2C devices

/****LCD display****/
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
byte i2c_LCD_addr;                // We scan for unknown address.
//#define I2C_ADDR 0x27           // Add your address here if
#define Rs_pin 0                  // not using an i2c_scan routine.
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
/*******************/


/****DHT temperature and humidity sensor****/
#include "DHT.h"
#define DHTPIN 2
/* Uncomment whatever type you're using! */
//#define DHTTYPE DHT11           // DHT 11 (blue)
#define DHTTYPE DHT22           // DHT 22  (AM2302), AM2321 (white)

DHT dht(DHTPIN, DHTTYPE);
/*******************************************/


/****BMP pressure sensor****/
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>      // includes wire.h
#define ALTITUDE 1012.2           // update daily at weather.unisys.com/forecast.php?Name=43606
Adafruit_BMP280 bme; // I2C
/***************************/


/****Real Time Clock 3231****/
#include "Sodaq_DS3231.h"         // has added datetime for SD card
#define SECONDS_ADJUST 18         // seconds to add to compile time when setting RTC
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
byte i2c_RTC_addr;
uint32_t old_ts;
/***************************/


/****SD Card****/
#include <SdFat.h>
SdFat sd;
SdFile file;
// Default SD chip select is SS pin
const uint8_t chipSelect = SS;    // D10 on Arduino Nano
uint8_t day, second;
/***************/


/**Desparate for dynamic memory, storing strings on FLASH**/
/** The DHT is crashing when less than 200bytes are left **/
/** Use the F() macro with larger strings to save memory **/
/**********************************************************/


void setup()
{
  Wire.begin();                   // Needed for i2c

  dht.begin();                    // Needed for DHT

  Serial.begin(9600);
}

void loop()
{
  scan_for_i2c();
  test_LCD();
  test_DHT();
  test_BMP();
  test_RTC();
  test_SD();

  Serial.println("Done.\n");

  delay(10000);                   // Wait 10 seconds for next scan
}


void scan_for_i2c()
{
  // i2c_scanner
  //
  // This program (or code that looks like it)
  // can be found in many places.
  // For example on the Arduino.cc forum.
  // The original author is not know.

  byte error, address;
  byte i2c_BMP_addr;
  int nDevices;

  i2c_LCD_addr = 0, i2c_BMP_addr = 0, i2c_RTC_addr = 0;

  Serial.println(F("-Scanning for I2C devices..."));

  nDevices = 0;
  for(address = 127; address > 0; address-- ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    // Test error checking
    //if(address == 42)
    //  error = 4;

    if (error == 0)
    {
      if(address == 0x76 || address == 0x77)    // Assume BMP
        i2c_BMP_addr = address;

      if (address == 0x68)
        i2c_RTC_addr = address;                 // Another assumption

      if(address >= 0x21 && address <= 0x3F)
        i2c_LCD_addr = address;                 // Another assumption, usually 0x21 - 0x3F

      nDevices++;
    }
    else if (error == 4) 
    {
      Serial.print(F("**Unknown ERROR** at address 0x"));

      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }

  if (nDevices == 0)
    Serial.println(F("**No I2C devices found**"));

  else
  {
    // Liquid Crystal Display
    if(i2c_LCD_addr != 0)
    {
      Serial.print(F(" LCD device found at address 0x"));

      if (i2c_LCD_addr<16) 
        Serial.print("0");
      Serial.println(i2c_LCD_addr,HEX);
    }
    else
      Serial.println(F("**No LCD device found**"));

    // Barometric Pressure Sensor
    if(i2c_BMP_addr != 0)
    {
      Serial.print(F(" BMP device found at address 0x"));

      if (i2c_BMP_addr<16) 
        Serial.print("0");
      Serial.println(i2c_BMP_addr,HEX);
    }
    else
      Serial.println(F("**No BMP device found**"));

    // Real Time Clock
    if(i2c_RTC_addr != 0)
    {
      Serial.print(F(" RTC device found at address 0x"));
      if (i2c_RTC_addr<16) 
        Serial.print("0");
      Serial.println(i2c_RTC_addr,HEX);
    }
    else
      Serial.println(F("**No RTC device found**"));
  }
}

void test_LCD()
{
  if(!i2c_LCD_addr)
    return;

  LiquidCrystal_I2C lcd(i2c_LCD_addr,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
  lcd.begin (16,2);
  // LCD Backlight ON
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();                    // go home on LCD
  lcd.print("LCD Screen Works");
  lcd.setCursor(2,1);             // go to third column, second line
  lcd.print("Address 0x");
  lcd.print(i2c_LCD_addr,HEX);
}

void test_DHT()
{
  // DHT testing code by ladyada

  Serial.print("-DHT");
  if(DHTTYPE == 11)
    Serial.print("11 (blue sensor)");
  if(DHTTYPE == 22)
    Serial.print("22 (white sensor)");
  Serial.print(" test");

  // Wait 2 seconds between measurements.
  delay(500);
  Serial.print(".");                            // Waiting animation
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.println("");

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("**Failed to read from DHT sensor!**"));
    return;
  }

  Serial.print(" Temperature: ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}

void test_BMP()
{
  /***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMEP280 Breakout 
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
  ***************************************************************************/
  float temperature, pressure, alt;

  if (!bme.begin())
    return;

  Serial.println("-BMP280 test...");

  temperature = bme.readTemperature();
  temperature = (temperature * 1.8) + 32;  // convert to F
  Serial.print(" Temperature: ");
  Serial.print(temperature);
  Serial.print(" *F\t");

  pressure = bme.readPressure();
  pressure = pressure / 100;      // convert Pa to mbar
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print(" mbar\t");

  alt = bme.readAltitude(ALTITUDE);   // ALTITUDE should be adjusted to your local forcast
  alt = alt * 3.2808;
  Serial.print("Approx altitude = ");
  Serial.print(alt);
  Serial.println(" feet");
}


void test_RTC()
{
  if (!i2c_RTC_addr)
    return;

  rtc.begin();

  DateTime now = rtc.now();       //get the current date-time in the now variable

  if (now.getEpoch() < DateTime(__DATE__, __TIME__).getEpoch() + SECONDS_ADJUST)
  {
    Serial.println(F("**Updating RTC time with compile time**"));
    rtc.setEpoch(DateTime(__DATE__, __TIME__).getEpoch() + SECONDS_ADJUST);
    delay(100);
    now = rtc.now();              //update the now variable with the current date-time
  }

  uint32_t ts = now.getEpoch();

  Serial.println("-System date and time:");

  if (old_ts == 0 || old_ts != ts)
  {
    old_ts = ts;
    Serial.print(' ');
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.print(' ');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');

    //Serial.print(now.hour(), DEC);  // 24 hour format
    // 12 hour conversion below
    int hr_24, hr_12;
    hr_24=now.hour();

    if (hr_24 == 0 || hr_24 == 12)
      hr_12 = 12;
    else
      hr_12 = hr_24%12;

    Serial.print(hr_12, DEC);
    //-------------------------
    
    Serial.print(':');
    if (now.minute() < 10)
      Serial.print('0');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if (now.second() < 10)
      Serial.print('0');
    Serial.print(now.second(), DEC);

    if (hr_24<12)
      Serial.println(" AM");
    else
      Serial.println(" PM");
  }
}


void test_SD()
{
  // Requires dateTime() callback function (below) to include the date with files
  // most of this is from the SdFat example Timestamp.ino
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorPrint();
    return;
  }

  // set dateTime callback function (puts date onto the file when accessed)
  SdFile::dateTimeCallback(dateTime);

  // create a new file with callback timestamps
  if (!file.open("data.txt", O_CREAT | O_WRITE)) {
    sd.errorPrint("open data.txt failed");
  }

  // change call back date
  day += 1;

  // must add two to see change since FAT second field is 5-bits
  second += 2;

  // modify file by writing a byte
  file.write('t');

  // force dir update (write all data to the SD)
  file.sync();

  dir_t dir;  // holds file data like timestamp

  if (!file.dirEntry(&dir)) {
    file.printName();
    Serial.println(" dirEntry failed");
    return;
  }
  
  file.printName();
  Serial.print(" - fileSize: ");
  Serial.println(file.fileSize());

  Serial.print(" Created:  ");
  file.printFatDate(dir.creationDate);
  Serial.print(" ");
  file.printFatTime(dir.creationTime);
  Serial.println();

  Serial.print(" Modified: ");
  file.printFatDate(dir.lastWriteDate);
  Serial.print(" ");
  file.printFatTime(dir.lastWriteTime);
  Serial.println();

  Serial.print(" Accessed: ");
  file.printFatDate(dir.lastAccessDate);
  Serial.println();

  // close file
  file.close();
}


void dateTime(uint16_t* date, uint16_t* time) {
 /*
  * dateTimeCallback() sets this function
  * that is called when a file is created
  * or when a file's directory entry is
  * modified by sync().
  */

  DateTime now = rtc.now();

  uint16_t year = now.year();
  uint8_t month = now.month();
  day = now.date();

  uint8_t hour = now.hour();
  uint8_t minute = now.minute();
  second = now.second();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(year, month, day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(hour, minute, second);
}

