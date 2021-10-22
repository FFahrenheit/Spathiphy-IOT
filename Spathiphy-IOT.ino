#include <ESP8266WiFi.h>
#include "DHTStable.h"
#include <Wire.h>
#include <BH1750.h>

#define DHT22_PIN D0

const char * ssid = "TheCoolestWiFiLM";
const char * password = "LopezMurillo128";

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

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
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