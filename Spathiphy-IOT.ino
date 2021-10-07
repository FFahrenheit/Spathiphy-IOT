#include "DHTStable.h"
#include <Wire.h>
#include <BH1750.h>

#define DHT22_PIN D0

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
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  DHT.read22(DHT22_PIN);
  float hum = DHT.getHumidity();
  float temp = DHT.getTemperature();
  float light = LUX.readLightLevel();//Realizamos una lectura del sensor
  Serial.println("Temperatura: " + String(temp) + "\tHumedad: " + String(hum) + "%\t\tLuminosidad: " + String(light));  
  delay(2000);
}