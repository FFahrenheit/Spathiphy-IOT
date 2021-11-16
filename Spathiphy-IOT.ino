#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "DHTStable.h"
#include <Wire.h>
#include <BH1750.h>

String MY_PRIVATE_ID = "1";
String SERVER_ADDRESS = "http://142.93.117.12/api/planta/" + MY_PRIVATE_ID;

#define DHT22_PIN D0
#define FOCO D8
#define VENTILADOR D7
#define BOMBA D6

char *redes[2][2] = {
  {"TheCoolestWiFiLM", "LopezMurillo128"},
  {"WifiLM", "LopezMurillo128"}
};

ESP8266WiFiMulti wifiMulti;
WiFiClient wifiClient;

DHTStable DHT;
BH1750 LUX(0x1D);

float hum=0.0f, temp=0.0f, light=0.0f;
double max_hum = 0.0f, min_hum = 0.0f, max_temp = 0.0f, min_temp = 0.0f;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FOCO, OUTPUT);
  pinMode(VENTILADOR, OUTPUT);
  pinMode(BOMBA, OUTPUT);

  delay(500);
  Wire.begin(D2, D1);
  Serial.begin(115200);
  Serial.println("Sensors starting");

  digitalWrite(FOCO, LOW);
  digitalWrite(VENTILADOR, LOW);
  digitalWrite(BOMBA, LOW);

  while(!LUX.begin())
  {
    Serial.println("Error initialising BH1750");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  Serial.println("BH1750 initialised");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  for(auto red : redes){
    Serial.print("SSID: ");
    Serial.print(red[0]);
    Serial.print("\t\tPassword: ");
    Serial.println(red[1]);

    wifiMulti.addAP(red[0], red[1]);
  }

  Serial.println("Connecting to WiFi...");
  int i = 0;

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1250);
    Serial.print(++i); Serial.print(' ');
  }
  
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  config();
}

void loop()
{
  DHT.read22(DHT22_PIN);
  hum = DHT.getHumidity();
  temp = DHT.getTemperature();
  light = LUX.readLightLevel();
  Serial.println("Temperatura: " + String(temp) + "°C\t\tHumedad: " + String(hum) + "%\t\tLuminosidad: " + String(light) + "lx");  
  unsigned long timeBegin = millis();
  
  sendData();
  
  unsigned long timeFinish = millis();
  int httpTime = timeFinish - timeBegin;
  Serial.println(httpTime);
  blink(2000 - httpTime);
}

void blink(int remainingDelay)
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  if(remainingDelay > 0){
    delay(remainingDelay);
  }
}

void sendData(){
  HTTPClient http;
  String body = "temperatura=" + String(temp) + "&humedad=" + String(hum) + "&luminosidad=" + String(light);
  Serial.println(body); 

  http.begin(wifiClient, SERVER_ADDRESS);  //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.PUT(body);               //Send the request
  Serial.println("Response: " + String(httpCode));


  if (httpCode > 0) { //Check the returning code
 
    String payload = http.getString();   //Get the request response payload
    JSONVar response = JSON.parse(payload);

    if (JSON.typeof(response) != "undefined" && response.hasOwnProperty("title"))
    {
      Serial.println(response["title"]);
    }
    else
    {
      Serial.println(response);
    }
  }
 
  http.end();   //Close connection
}

void config(){
  HTTPClient http;
  http.begin(wifiClient, SERVER_ADDRESS + "/config");  //Specify request destination
  int httpCode;
  do{
    httpCode = http.GET();                  //Send the request
    Serial.println("Response: " + String(httpCode));

  if (httpCode > 0) { //Check the returning code
 
    String payload = http.getString();   //Get the request response payload
    JSONVar response = JSON.parse(payload);

    if (JSON.typeof(response) != "undefined" && response.hasOwnProperty("max_temp"))
    {
      max_temp = (double) response["max_temp"];
      min_temp = (double) response["min_temp"];
      max_hum = (double) response["max_hum"];
      min_hum = (double) response["min_hum"];

      Serial.println("Maxima temperatura: " + String(max_temp));
      Serial.println("Minima temperatura: " + String(min_temp));
      Serial.println("Maxima humedad: " + String(max_hum));
      Serial.println("Minima humedad: " + String(min_hum));
    }
    else
    {
      Serial.println(response);
    }
  }
 
  }while(httpCode < 0);
  
  http.end();   //Close connection
}