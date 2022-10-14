#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String url = "https://ga.insoulit.com/api/data";


// wifi connection variables 
const char* ssid = "black_sky";
const char* password =  "blackmirror";
unsigned long previousMillis = 0;
unsigned long interval = 30000;
  
void setup() {
  
  Serial.begin(115200);
  initWiFi();
  Serial.println("Setup done");
  
}
  
void loop() {

  checkWifiConnection();
  sendDataToServer( 10.56 , 10.55 , 10.55 , 10.55 , 10.55 );
  delay(5000);
  
}

void sendDataToServer(double t ,double h ,double l ,double m ,double p ){
  
  Serial.println("--------------------------------------------------------");
  
  HTTPClient http;   
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");   
  http.addHeader("Accept", "application/json");

  StaticJsonDocument<256> doc;
  
  doc["device_key"] = "xuHUr5wJI04O0A4tRKTT";
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
