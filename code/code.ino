
#include <iostream>
#include <string>
using namespace std;



// Temperature & Humidity
#include "DHT.h"
#define DHT_PIN 4     
#define DHTTYPE DHT22
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
  
  delay(2000);
  
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

  
