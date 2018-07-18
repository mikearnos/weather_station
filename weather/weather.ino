
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme;

#define ALTITUDE 1025.1  // update daily at weather.unisys.com/forecast.php?Name=43606
 #define I2C_ADDR 0x3F // <<- Add your address here.
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
float t1,t2; 


void setup()
{
  Serial.begin(9600);
  lcd.begin (16,2); // <<-- our LCD is a 20x4, change for your LCD if needed
  // LCD Backlight ON
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home (); // go home on LCD
  lcd.print("Weather Station");
  delay(5000);
  dht.begin();
  bme.begin();
}
 
void loop()
{
  float temperature, pressure, alt;
  if (bme.begin())
  {
    Serial.print("1");

    temperature = bme.readTemperature();
    temperature = (temperature * 1.8) + 32;  // convert to F
    t1 = temperature;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Baro Temperature: ");
    lcd.setCursor(0,1);
    lcd.print(temperature);
    lcd.print(" deg F");
    delay(3000);

    pressure = bme.readPressure();
    pressure = pressure / 100;  // convert Pa to mbar
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pressure: ");
    lcd.setCursor(0,1);
    lcd.print(pressure);
    lcd.print(" mbar");
    delay(3000);

    alt = bme.readAltitude(ALTITUDE);
    alt = alt * 3.2808;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Altitude: ");
    lcd.setCursor(0,1);
    lcd.print(alt);
    lcd.print(" feet");
    delay(3000);
  }
 
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  t = (t * 1.8) + 32;  // convert to F
  t2=t;
  lcd.clear();
  lcd.setCursor (0,0); // go to start of 2nd line
  lcd.print("Humidity: ");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print(" %");
  delay(3000);
  
  lcd.clear();
  lcd.setCursor (0,0); // go to start of 2nd line
  lcd.print("DHT Temperature: ");
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print(" deg F");
  delay(3000);

  lcd.clear();
  lcd.setCursor (0,0); // go to start of 2nd line
  lcd.print("Mean Temperature: ");
  lcd.setCursor(0,1);
  lcd.print((t1+t2)/2);
  lcd.print(" deg F");
  delay(3000);
}

