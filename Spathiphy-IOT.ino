#include "DHTStable.h"
#include <Wire.h>
#include <BH1750.h>

#define DHT22_PIN D0

DHTStable DHT;
BH1750 LUX(0x23);

void setup()
{
  Wire.begin(D2, D1);
  Serial.begin(9600);
  Serial.println("Sensors starting");
  if (LUX.begin()) 
  {
    Serial.println("BH1750 initialised");
  }
  else
  {
    Serial.println("Error initialising BH1750");
  }

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