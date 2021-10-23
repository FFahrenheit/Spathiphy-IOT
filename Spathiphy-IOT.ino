#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "DHTStable.h"
#include <Wire.h>
#include <BH1750.h>

#define DHT22_PIN D0

char *redes[2][2] = {
  {"TheCoolestWiFiLM", "LopezMurillo128"},
  {"WifiLM", "LopezMurillo128"}
};

ESP8266WiFiMulti wifiMulti;

DHTStable DHT;
BH1750 LUX(0x1D);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  delay(500);
  Wire.begin(D2, D1);
  Serial.begin(9600);
  Serial.println("Sensors starting");

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
}

void loop()
{
  DHT.read22(DHT22_PIN);
  float hum = DHT.getHumidity();
  float temp = DHT.getTemperature();
  float light = LUX.readLightLevel();//Realizamos una lectura del sensor
  Serial.println("Temperatura: " + String(temp) + "°C\t\tHumedad: " + String(hum) + "%\t\tLuminosidad: " + String(light) + "lx");  
  blink();
}

void blink()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1850);
}