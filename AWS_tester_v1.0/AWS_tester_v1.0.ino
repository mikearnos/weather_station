/*-----------------------------------\
|  10 Dollar Arduino Weather Station |
|  hardware test code v1.0           |
\-----------------------------------*/

/****LCD display****/
#include <Wire.h>                 // Needed for I2C
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
Adafruit_BMP280 bme; // I2C
/***************************/


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

  Serial.println("Done.\n");

  delay(5000);                    // Wait 5 seconds for next scan
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

  i2c_LCD_addr = 0, i2c_BMP_addr = 0;

  Serial.println("-Scanning for I2C devices...");

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
      else
        i2c_LCD_addr = address;                 // Another assumption

      nDevices++;
    }
    else if (error == 4) 
    {
      Serial.print("**Unknown ERROR* at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("**No I2C devices found**");
  else
  {
    if(i2c_LCD_addr != 0)
    {
      Serial.print("LCD device found at address 0x");
      if (i2c_LCD_addr<16) 
        Serial.print("0");
      Serial.println(i2c_LCD_addr,HEX);
    }
    else
      Serial.println("**No LCD device found**");

    if(i2c_BMP_addr != 0)
    {
      Serial.print("BMP device found at address 0x");
      if (i2c_BMP_addr<16) 
        Serial.print("0");
      Serial.println(i2c_BMP_addr,HEX);
    }
    else
      Serial.println("**No BMP device found**");
  }
}

void test_LCD()
{
  if(i2c_LCD_addr != 0)
  {
    LiquidCrystal_I2C lcd(i2c_LCD_addr,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
    lcd.begin (16,2);
    // LCD Backlight ON
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home ();                  // go home on LCD
    lcd.print("LCD Screen Works");
    lcd.setCursor(2,1);           // go to third column, second line
    lcd.print("Address 0x");
    lcd.print(i2c_LCD_addr,HEX);
  }
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
  float f = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("**Failed to read from DHT sensor!**");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.print(" *C\t");
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
  if (!bme.begin()) {  
    Serial.println("**Could not test BMP280 sensor**");
    return;
  }

  Serial.println("-BMP280 test...");

  Serial.print("Temperature: ");
  Serial.print(bme.readTemperature());
  Serial.print(" *C\t");

  Serial.print("Pressure: ");
  Serial.print(bme.readPressure());
  Serial.print(" Pa\t");

  Serial.print("Approx altitude = ");
  Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcast
  Serial.println(" m");
}

