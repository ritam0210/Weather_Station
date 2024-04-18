// Lora receive a structure

#include <SPI.h>
#include <LoRa.h>

#define ss 5
#define rst 14
#define dio0 2

struct Data 
{
  int x;
  float y;
  char z[10];
} data;


void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);  
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() 
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
    Serial.print("\nReceived packet size ");
    Serial.print(packetSize);
    Serial.print(" data ");
    LoRa.readBytes((byte *)&data, packetSize);
    // read packet
//    while (LoRa.available())
//    {
//      for (int i = 0; i < packetSize; i++) 
//      {
//        ((byte *) &data)[i] = LoRa.read();
//        Serial.print(' ');
//        Serial.print(((byte *) &data)[i]);
//      }
//    }
    
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print("x = "); Serial.print(data.x);
    Serial.print(" y = "); Serial.print(data.y);
    Serial.print(" z = "); Serial.print(data.z);

  }
}
