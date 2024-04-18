#include <SPI.h>
#include <LoRa.h>

#define ss 5
#define rst 14
#define dio0 2

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
  
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);  
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
}

void loop() 
{
  Serial.print("Sending packet: ");
  Serial.println(msgCount);

  data.Wind_spd = 0;
  data.Wind_dir = 0;
  data.Temp = 24.8;
  data.Humid = 62.4;
  data.Lux = 1570.0;
  data.UV = 10.0;
  data.Pres = 1012.8;
  
  String outgoing = String(data.Wind_spd) + String(data.Wind_dir) + String(data.Temp) + String(data.Humid) + String(data.Lux) + String(data.UV) + String(data.Pres);
  
  // send packet
  LoRa.beginPacket();
  LoRa.write(destination);
  LoRa.write(localAddress);
  LoRa.write(msgCount);
  LoRa.write(outgoing.length());
  LoRa.write((byte *)&data, sizeof(data));
  
//  LoRa.write(Wind_spd);
//  LoRa.write(Wind_dir);
//  LoRa.write(Temp);
//  LoRa.write(Humid);
//  LoRa.write(Lux);
//  LoRa.write(UV);
//  LoRa.write(Pres);
  
  LoRa.endPacket();
  msgCount++;

  Serial.println(outgoing.length());
  delay(2500);
}
