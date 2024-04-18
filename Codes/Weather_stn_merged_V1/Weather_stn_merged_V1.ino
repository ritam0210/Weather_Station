#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_LTR390.h"
#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define SEALEVELPRESSURE_HPA (1013.25)
#define ALTITUDE 11.0

BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_LTR390 ltr = Adafruit_LTR390();
Adafruit_BME280 bme;



void setup() 
{
  Serial.begin(115200);
  
  // Initialize the I2C bus
  Wire.begin();

  lightMeter.begin();
  dht.begin();

  //***************************************************************************************************************************
  bool status;
  status = bme.begin(0x76);  
  if (!status) 
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  //***************************************************************************************************************************
  if ( ! ltr.begin() ) 
  {
    Serial.println("Couldn't find LTR sensor!");
    while (1) delay(10);
  }
  Serial.println("Found LTR sensor!");

  ltr.setMode(LTR390_MODE_UVS);
  if (ltr.getMode() == LTR390_MODE_ALS) 
  {
    Serial.println("In ALS mode");
  } 
  else 
  {
    Serial.println("In UVS mode");
  }

  ltr.setGain(LTR390_GAIN_3);
  Serial.print("Gain : ");
  switch (ltr.getGain()) 
  {
    case LTR390_GAIN_1: Serial.println(1); break;
    case LTR390_GAIN_3: Serial.println(3); break;
    case LTR390_GAIN_6: Serial.println(6); break;
    case LTR390_GAIN_9: Serial.println(9); break;
    case LTR390_GAIN_18: Serial.println(18); break;
  }

  ltr.setResolution(LTR390_RESOLUTION_16BIT);
  Serial.print("Resolution : ");
  switch (ltr.getResolution()) 
  {
    case LTR390_RESOLUTION_13BIT: Serial.println(13); break;
    case LTR390_RESOLUTION_16BIT: Serial.println(16); break;
    case LTR390_RESOLUTION_17BIT: Serial.println(17); break;
    case LTR390_RESOLUTION_18BIT: Serial.println(18); break;
    case LTR390_RESOLUTION_19BIT: Serial.println(19); break;
    case LTR390_RESOLUTION_20BIT: Serial.println(20); break;
  }

  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);
  //***************************************************************************************************************************
  
}

float getDHT()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
//    break;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

float getlux()
{
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
}

float getBME()
{
  Serial.print("Pressure = ");
  float pressure = bme.readPressure();
//  pressure = bme.seaLevelForAltitude(ALTITUDE, pressure);
  Serial.print(pressure/100.0);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
}

float getLTR()
{
  if (ltr.newDataAvailable()) 
  {
      Serial.println("UV data: "); 
      Serial.println(ltr.readUVS());
  }
}

void loop() 
{
  getDHT();
  getlux();
  getBME();
  getLTR();
  delay(1000);
}
