#include <WiFi.h>;
#include <HTTPClient.h>;

String url = "https://ga.insoulit.com/api/data";


// wifi connection variables 
const char* ssid = "black_sky";
const char* password =  "blackmirror";
unsigned long previousMillis = 0;
unsigned long interval = 10000;
  
void setup() {
  
  Serial.begin(115200);
  initWiFi();
  Serial.println("Setup done");
  
}
  
void loop() {

  checkWifiConnection();

  WiFiClient client;
  HTTPClient http;   
  
  http.begin(client , url);
//  http.addHeader("Content-Type", "application/json");   
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Accept", "application/json");
//
//  const size_t CAPACITY = JSON_OBJECT_SIZE(1);
//  StaticJsonDocument<CAPACITY> doc ;



  String httpRequestData = "device_key=xuHUr5wJI04O0A4tRKTT";        
  httpRequestData = httpRequestData + "&T=39";
  httpRequestData = httpRequestData + "&H=39";  
  httpRequestData = httpRequestData + "&L=39";  
  httpRequestData = httpRequestData + "&M=39";  
  httpRequestData = httpRequestData + "&P=39"; 
  Serial.println(httpRequestData); 
  int httpResponseCode = http.POST(httpRequestData);
  
  Serial.print("HTTP Response code : ");
  Serial.println(httpResponseCode);
  

  // Free resources
  http.end();
  
  
  
  delay(8000);  //Send a request every 10 seconds
  
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
