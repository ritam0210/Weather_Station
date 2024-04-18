// Lora transmit a structure

#include <SPI.h>
#include <LoRa.h>

#define ss 5
#define rst 14
#define dio0 2

struct Data 
{
  int x = 458;
  float y = 1014.87;
  char z[10] = "hello";
}data;

//Data data={1 , 4.5, "hello"};

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

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
  Serial.print("\nSending packet: ");
  Serial.print("x = ");Serial.print(data.x);
  Serial.print(" y = ");Serial.print(data.y);
  Serial.print(" z = ");Serial.print(data.z);  
  
  // send packet
  LoRa.beginPacket();
  LoRa.write((byte *)&data, sizeof(data));
//  for (unsigned int i = 0; i < sizeof(Data);i++) 
//  {
//    Serial.print(' ');
//    LoRa.write(((byte *) &data)[i]);
//    Serial.print(((byte *) &data)[i]);
//  }
  LoRa.endPacket();
  
  data.x += 1;
  data.y += 1;
  data.z[0] += 1;

  delay(2000);
}
