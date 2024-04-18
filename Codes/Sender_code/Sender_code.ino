//==============================================LIBRARIES=================================================
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_LTR390.h"
#include "DHT.h"
#include <SPI.h>
#include <LoRa.h>


#define ss 5
#define rst 14
#define dio0 2

#define DHTPIN 32     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define SEALEVELPRESSURE_HPA (1013.25)
#define ALTITUDE 11.0

//============================================PIN DEFINITION==============================================
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_LTR390 ltr = Adafruit_LTR390();
Adafruit_BME280 bme;

byte msgCount = 0;
byte localAddress = 0xB5;
byte destination = 0xF5;

struct Data
{
  float Wind_spd;
  int Wind_dir;
  float Temp;
  float Humid;
  int Lux;
  int UV;
  float Pres;
}data;

void setup() 
{
  Serial.begin(115200);
  //*********************************************************************************************************  
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);  
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");

  //*********************************************************************************************************
  // Initialize the I2C bus
  Wire.begin();

  lightMeter.begin();
  dht.begin();

  //*********************************************************************************************************
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
//  if (ltr.getMode() == LTR390_MODE_ALS) 
//  {
//    Serial.println("In ALS mode");
//  } 
//  else 
//  {
//    Serial.println("In UVS mode");
//  }

  ltr.setGain(LTR390_GAIN_3);
  ltr.setResolution(LTR390_RESOLUTION_16BIT);

  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);
  //***************************************************************************************************************************
  
}

float getDHT()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
//  if (isnan(h) || isnan(t)) 
//  {
//    Serial.println(F("Failed to read from DHT sensor!"));
////    break;
//  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.println(h);
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  Serial.print(hic);
  Serial.println(F("°C "));

  return h;
}

float getlux()
{
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  return lux;
}

float getBME()
{
  Serial.print("Pressure = ");
  float pressure = bme.readPressure();
//  pressure = bme.seaLevelForAltitude(ALTITUDE, pressure);
  Serial.print(pressure/100.0);
  Serial.println(" hPa");

//  Serial.print("Approx. Altitude = ");
//  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//  Serial.println(" m");

  return pressure/100.0;
}

float getLTR()
{
  if (ltr.newDataAvailable()) 
  {
      Serial.print("UV data: "); 
      Serial.println(ltr.readUVS());
  }
  return ltr.readUVS();
}

void loop() 
{
//  getDHT();
//  getlux();
//  getBME();
//  getLTR();

  Serial.print("Sending packet: ");
  Serial.println(msgCount);

  data.Wind_spd = 0;
  data.Wind_dir = 0;
  data.Temp = 24.8;
  data.Humid = getDHT(); //62.4
  data.Lux = getlux(); //1570.0
  data.UV = getLTR(); //10.0
  data.Pres = getBME(); //1012.8
  
  String outgoing = String(data.Wind_spd) + String(data.Wind_dir) + String(data.Temp) + String(data.Humid) + String(data.Lux) + String(data.UV) + String(data.Pres);
  
  // send packet
  LoRa.beginPacket();
  LoRa.write(destination);
  LoRa.write(localAddress);
  LoRa.write(msgCount);
  LoRa.write(outgoing.length());
  LoRa.write((byte *)&data, sizeof(data));

  LoRa.endPacket();
  msgCount++;

  Serial.println(outgoing.length());
  delay(1500);
}
