#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <HTTPClient.h>
#include "time.h"

#define ss 5         // LoRa chip select pin
#define rst 14       // LoRa reset pin
#define dio0 4       // LoRa data pin

#define TFT_CS 15    // TFT chip select pin
#define TFT_DC 2     // TFT DC or data/command pin
#define TFT_RST 12   // TFT reset pin

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const char * ssid = "Accio_WiFi";
const char * password = "Accio_Ritam09";

//==================================GOOGLE SHEET CREDENTIALS=========================================
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = 19800;
//const int   daylightOffset_sec = 0;
//// Google script ID and required credentials
//String GOOGLE_SCRIPT_ID = "AKfycbwsA3rGqiojLrvoqzV2urXFGGGeJMoqYh6dXqYYfCmSyMY-BVPbkxOOH1y2rRlmG9x6";

//===================================================================================================

byte localAddress = 0xF5;
int recipient;
String incoming;
byte sender;
byte incomingMsgId;
byte incomingLength;

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

//========================================DISPLAY FUNCTION===========================================

/*void updatedisplay()
{
  tft.setTextColor(ILI9341_ORANGE);
  tft.setTextSize(2);
  tft.setCursor(60, 10);
  tft.println("WEATHER STN");

  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.print("WIND SPD:");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(data.Wind_spd);
  tft.println("kmph");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 80);
  tft.print("WIND DIR: ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(data.Wind_dir);
  tft.println(" deg");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 110);
  tft.print("TEMPERATURE:");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(data.Temp);
  tft.println("*C");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 140);
  tft.print("HUMIDITY: ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(data.Humid);
  tft.println("%");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 170);
  tft.print("LIGHT: ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(data.Lux);
  tft.println("lux");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 200);
  tft.print("UV: ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.println(data.UV);
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 230);
  tft.print("PRESSURE:");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(data.Pres);
  tft.println("hPa");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 260);
  tft.print("SENDER: ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("0x");
  tft.println(sender, HEX);
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 290);
  tft.print("RSSI: ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.println(LoRa.packetRssi());

}*/
//==============================================================================================

void setup() 
{
  Serial.begin(115200);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  
  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);  
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  Serial.print("Connecting");
//  while (WiFi.status() != WL_CONNECTED) 
//  {
//    delay(500);
//    Serial.print(".");
//  }
  
  // Init and get the time
//  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//  Serial.flush();
  //-----------------------------------------------------------------------------------------------
}

void loop() 
{
  int packetSize = LoRa.parsePacket();
  if (packetSize == 0) return;
  
  recipient = LoRa.read();
  sender = LoRa.read();
  incomingMsgId = LoRa.read();
  incomingLength = LoRa.read();
  LoRa.readBytes((byte *)&data, packetSize);
  
  incoming = String(data.Wind_spd) + String(data.Wind_dir) + String(data.Temp) + String(data.Humid) + String(data.Lux) + String(data.UV) + String(data.Pres);
  
//  if (incomingLength != incoming.length()) 
//  {
//    Serial.println(incoming.length());
//    Serial.println("error: message length does not match length");
//    return;
//  }
  if (recipient != localAddress) 
  {
    Serial.println("This message is not for me.");
    return;
  }
  
  Serial.println("**********************************************************************");
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));

  Serial.println("Wind_spd: " + String(data.Wind_spd));
  Serial.println("Wind_dir: " + String(data.Wind_dir));
  Serial.println("Temp: " + String(data.Temp));
  Serial.println("Humid: " + String(data.Humid));
  Serial.println("Lux: " + String(data.Lux));
  Serial.println("UV: " + String(data.UV));
  Serial.println("Pres: " + String(data.Pres));

  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println("");

  //===================================================================================================
  /*if (WiFi.status() == WL_CONNECTED) 
  {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) 
    {
      Serial.println("Failed to obtain time");
      return;
    }

    String wind_spd = String(data.Wind_spd);
    String wind_dir = String(data.Wind_dir);
    String temp = String(data.Temp);
    String humid = String(data.Humid);
    String lux = String(data.Lux);
    String uv = String(data.UV);
    String pres = String(data.Pres);
    String Rssi = String(LoRa.packetRssi());
    String snr = String(LoRa.packetSnr());
    
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?spd=" + wind_spd + "&dir=" + wind_dir + "&temp=" + temp +
     "&humid=" + humid + "&lux=" + lux + "&uv=" + uv + "&pres=" + pres + "&Rssi=" + Rssi + "&snr=" + snr;
    Serial.println("POST data to spreadsheet:");
    
    //Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) 
    {
        payload = http.getString();
//        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
    unsigned long currentMillis = millis();
    Serial.println(currentMillis);
  }*/
  
//  updatedisplay();
}
