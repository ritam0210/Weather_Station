#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "TP-Link_907E"
#define WIFI_PASSWORD "ritam@09"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDFNu7k2SNE2HcZpSbJy3cVVkkdcQHK4oY"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://weatherstn-65af6-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis1 = 0;
unsigned long sendDataPrevMillis2 = 0;
bool signupOK = false;

//---------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  float wind_spd = random(0.1,100.0);
  int wind_dir = random(0,360);
  float int_temp = random(25.1,39.9);
  float ext_temp = random(20.1,35.9);
  int hum = random(20,89);
  int light_int = random(10,65536);
  float UV = random(280.1,349.9);
  float atm_pres = random(100.1,400.1);
  float batt = random(11.1,12.6);
  delay(100);

  //--------------------------------------------------------------------------------------------------------
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis1 > 4000 || sendDataPrevMillis1 == 0))
  {
    sendDataPrevMillis1 = millis();
    
    if (Firebase.RTDB.setFloat(&fbdo, "Stn Readings/wind_spd", wind_spd))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.RTDB.setInt(&fbdo, "Stn Readings/wind_dir", wind_dir))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Stn Readings/int_temp", int_temp))
    Serial.println("PASSED");
      
    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Stn Readings/ext_temp", ext_temp))
    Serial.println("PASSED");
      
    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "Stn Readings/hum", hum))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "Stn Readings/light_int", light_int))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Stn Readings/UV", UV))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Stn Readings/atm_pres", atm_pres))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Stn Readings/batt", batt))
    Serial.println("PASSED");

    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  //--------------------------------------------------------------------------------------------------------

}
