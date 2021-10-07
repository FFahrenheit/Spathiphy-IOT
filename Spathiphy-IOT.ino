#include "DHTStable.h"

DHTStable DHT;

#define DHT22_PIN D0

void setup()
{
  Serial.begin(9600);
  Serial.println("Sensors starting");
}

void loop()
{
  DHT.read22(DHT22_PIN);
  float hum = DHT.getHumidity();
  float temp = DHT.getTemperature();
  Serial.println("Temperatura: " + String(temp) + "\tHumedad: " + String(hum) + "%");
  delay(2000);
}