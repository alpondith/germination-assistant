
#include <iostream>
#include <string>
using namespace std;


// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"



#define DHT_PIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHT_PIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHT_PIN, DHTTYPE);

// Light Sensor
// connect SDA to 21 and SCL 22
#include <BH1750.h>
#include <Wire.h>
BH1750 lightMeter(0x23);



// Soil Moisture Pin
#define SOIL_MOISTURE_PIN 2



// Display
// connect SDA to 21 and SCL 22
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WIRE Wire

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);





void setup() {
//  Serial.begin(115200);
  Serial.begin(9600);
  
  dht.begin();

  // light sensor
  Wire.begin();
  lightMeter.begin();

  // display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);



  
}

void loop() {
  
  
  Serial.println();
  Serial.println("***************************************************************************");
  
  String humidity = getHumidity();
  String temperature = getTemperature();
  String light = getLight();
  String soilMoisture = getSoilMoisture();
  
  Serial.println("***************************************************************************");
  Serial.println();

  displayData(humidity ,temperature , light , soilMoisture);
  
  delay(5000);
  
}

void displayData(String humidity,  String temperature , String light , String soilMoisture ){
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  
  display.clearDisplay();

  display.println(humidity);
  display.println("");
  display.println(temperature);
  display.println("");
  display.println(light);
  display.println("");
  display.println(soilMoisture);
  
  yield();
  display.display();
  
}



String getHumidity(){
  
  float humidity = dht.readHumidity();

  if ( isnan(humidity) ) {
    String error = "Humidity sensor Error" ;
    Serial.println(error);
    return error;
  }

  String output = "Humidity:" + String(humidity) + "%";
  Serial.println(output);
  return output;
  
}

String getTemperature(){
  
  float temperature = dht.readTemperature();

  if ( isnan(temperature) ) {
    String error = "Temperature sensor Error" ;
    Serial.println(error);
    return error;
  }

  String output = "Temperature:" + String(temperature) + "'C";
  Serial.println(output);
  return output;
  
}

String getSoilMoisture(){
  
  float soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  if ( isnan(soilMoisture) ) {
    String error = "Soil moisture sensor Error" ;
    Serial.println(error);
    return error;
  }

  String output = "Soil Moisture:" + String(soilMoisture);
  Serial.println(output);
  return output;
  
}

String getLight(){
  
  float lux = lightMeter.readLightLevel();
  
  if ( isnan(lux) ) {
    String error = "Light Sensor Error" ;
    Serial.println(error);
    return error;
  }
  
  String output = "Light:" + String(lux) + " lux";
  Serial.println(output);
  return output ;
  
}









// DHT Functionalities
// String dhtSensor(){
//
//  // Sensor readings
//  // Read humidity
//  float humidity = dht.readHumidity();
//  // Read temperature as Celsius (the default)
//  float temperatureInCelsius = dht.readTemperature();
//  // Read temperature as Fahrenheit (isFahrenheit = true)
//  float temperatureInFahrenheit = dht.readTemperature(true);
//
//  // Check if any reads failed and exit early (to try again).
//  if ( isnan(humidity) || isnan(temperatureInCelsius) || isnan(temperatureInFahrenheit) ) {
//    Serial.println("Failed to read from DHT sensor!");
//    return "DHT Sensor not found";
//  }
//
//  // Compute heat index in Fahrenheit (the default)
//  float heatIndexInFahrenheit = dht.computeHeatIndex(temperatureInFahrenheit , humidity);
//  // Compute heat index in Celsius (isFahreheit = false)
//  float heatIndexInCelsius = dht.computeHeatIndex(temperatureInCelsius, humidity, false);
//
//  Serial.print("Humidity: ");
//  Serial.print(humidity);
//  Serial.print("%  Temperature: ");
//  Serial.print(temperatureInCelsius);
//  Serial.print("°C ");
//  Serial.print(temperatureInFahrenheit);
//  Serial.print("°F  Heat index: ");
//  Serial.print(heatIndexInCelsius);
//  Serial.print("°C ");
//  Serial.print(heatIndexInFahrenheit);
//  Serial.println("°F");
//
//  String data = "Humidity : " + String(humidity);
//
//  return data;
//
//}
  
