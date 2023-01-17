
#include <iostream>
#include <string>
using namespace std;


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>



// wifi connection variables 
const char* ssid = "black_sky";
const char* password =  "blackmirror";
unsigned long previousMillis = 0;
unsigned long interval = 30000; // wifi connection checking interval

String url = "https://gassistant.insoulit.com/api/data";



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


// Relay Module Pins
//#define LIGHT_PIN 5 


// Display
// connect SDA to 21 and SCL 22
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WIRE Wire

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);




// ***************************************************************//
// ****************One Time Setup Function************************//
// ***************************************************************//
void setup() {
//  Serial.begin(115200);
  Serial.begin(9600);
  
  dht.begin();

  // light sensor
  Wire.begin();
  lightMeter.begin();

  // Relay Module 
//  pinMode(LIGHT_PIN, OUTPUT);

  // display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);

  initWiFi();
  Serial.println("Setup done");

}



// ***************************************************************//
// ****************Continuous Loop Function***********************//
// ***************************************************************//
void loop() {
  
  Serial.println();
  Serial.println("*************************************************");

  double temperature = getTemperature();
  double humidity = getHumidity();
  double light = getLight();
  double soilMoisture = getSoilMoisture();
  
  Serial.println("*************************************************");
  Serial.println();

//  digitalWrite(LIGHT_PIN, HIGH);

  displayData(temperature ,humidity , light , soilMoisture);

  checkWifiConnection();
  sendDataToServer( temperature , humidity , light , soilMoisture , 0.0 );

  delay(9000);
//  digitalWrite(LIGHT_PIN, LOW);
  
}



// ***************************************************************//
// ****************Display Data on LCD****************************//
// ***************************************************************//
void displayData( double temperature ,double humidity, double light , double soilMoisture ){
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  
  display.clearDisplay();


  display.println("Temperature : " + String(temperature));
  display.println("");
  display.println("Humidity : " + String(humidity) );
  display.println("");
  display.println("Light : " + String(light));
  display.println("");
  display.println("Soil Moisture : " + String(soilMoisture));
  
  yield();
  display.display();
  
}



// ***************************************************************//
// ****************Send Data to Server****************************//
// ***************************************************************//
void sendDataToServer(double t ,double h ,double l ,double m ,double p ){
  
  Serial.println("--------------------------------------------------------");
  
  HTTPClient http;   
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");   
  http.addHeader("Accept", "application/json");

  StaticJsonDocument<256> doc;
  
  doc["device_key"] = "PY5WGkoHwu4azEYOKmL51S3g6WEUKKKPAw6JCs3V";
  doc["T"] = t;
  doc["H"] = h;
  doc["L"] = l;
  doc["M"] = m;
  doc["P"] = p;

  char output[256];
  serializeJson(doc, output);
  Serial.println(output);
  
  int httpResponseCode = http.POST(output);
  
  Serial.print(" HTTP Response code : ");
  Serial.println(httpResponseCode);

  Serial.println(http.getString());
  
  http.end();
  
  Serial.println("--------------------------------------------------------");
}



// ***************************************************************//
// ************************Temperature****************************//
// ***************************************************************//
double getTemperature(){
  
  float temperature = dht.readTemperature();

  if ( isnan(temperature) ) {
    Serial.println("Temperature sensor Error");
    return -100000;
  }
  
  Serial.println("Temperature:" + String(temperature) + "'C");
  
  return temperature;
}



// ***************************************************************//
// ***************************Humidity****************************//
// ***************************************************************//
double getHumidity(){
  
  double humidity = dht.readHumidity();

  if ( isnan(humidity) ) {
    Serial.println("Humidity sensor Error");
    return -100000;
  }

  Serial.println("Humidity:" + String(humidity) + "%");
  
  return humidity;
}



// ***************************************************************//
// ******************************Light****************************//
// ***************************************************************//
double getLight(){
  
  double lux = lightMeter.readLightLevel();
  
  if ( isnan(lux) ) {
    Serial.println("Light Sensor Error");
    return -100000;
  }
  
  Serial.println("Light:" + String(lux) + " lux");
  
  return lux ;
}



// ***************************************************************//
// *************************Soil Moisture*************************//
// ***************************************************************//
double getSoilMoisture(){
  
  double soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  if ( isnan(soilMoisture) ) {
    Serial.println("Soil moisture sensor Error");
    return -100000;
  }
  
  Serial.println("Soil Moisture:" + String(soilMoisture));
  
  return soilMoisture;
}



// ***************************************************************//
// *************************Wifi Setup****************************//
// ***************************************************************//
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("");
  Serial.println("Congratulations! Connected to WiFi.");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
}



// ***************************************************************//
// *******************Wifi Connection Check***********************//
// ***************************************************************//
void checkWifiConnection(){
  unsigned long currentMillis = millis();
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println(" : Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}

  
