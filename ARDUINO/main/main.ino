//Defines
#define ARDUINOJSON_USE_DOUBLE 1

// Includes
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

 
// Devices
const int lcdCols = 16; 
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdCols, lcdRows);
uint8_t euro[8] = {0x3,0x4,0x8,0x1f,0x8,0x1f,0x4,0x3}; // char to display the euro.
const int buzzer = 0; // remove this if you do not use a buzzer.

// WiFi
const char* SSID = "Enter your WiFi SSID here.";
const char* PASSWORD = "Enter your Wifi password here.";

// Timers
const long interval = 7000; // the rate in milliseconds when the crypto price should get retrieved, change this to whatever you want. CoinGecko's API limit is 100/min
unsigned long prevTime = 0;

// Vars
String payload;
float nano_eur; // change the float to the cryptocurrency of your likings.

// Class Init
WiFiClientSecure client;
ESP8266WiFiMulti WiFiMulti;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, euro); // for the euro sign to display.
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT); // remove this if you do not use a buzzer.
  connectToWifi();
}


void connectToWifi() {
  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "PASSWORD");
}

void loop() {
  unsigned long curMillis = millis();
  if ((curMillis - prevTime) >= interval) {
    getPrice();
    printData();
    prevTime = curMillis;
  }
}

void getPrice() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure); // big thanks to bruger.dread.cz for writing a blog post on how to use an HTTPS API.
    client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://api.coingecko.com/api/v3/simple/price?ids=nano&vs_currencies=eur")) {  // check the CoinGecko API on how to change this to a different Cryptocurrency, for example: ids=bitcoin works.

      Serial.print("[HTTPS] GET...\n");
      int httpCode = https.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

void printData() {
  // Price Parse
  String json = payload;
  const size_t capacity = 2 * JSON_OBJECT_SIZE(1) + 10;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, json);
  float nano_eur = doc["nano"]["eur"]; // change nano to the cryptocurrency of your likings.
  Serial.println(nano_eur, 10); // change this variable if you want to use a different crypto.
  
  // LCD
  lcd.setCursor(0, 0);
  lcd.print("NANO PRICE:"); // change nano to the cryptocurrency of your likings.
  lcd.setCursor(0, 1);
  lcd.print((char)0);
  lcd.setCursor(1, 1);
  lcd.print(nano_eur, 3); // adjust the precision of the float, should be used for altcoins due to the lower price. Also, change the variable if you want to use a different crypto.
  lcd.setCursor(15, 1);
  lcd.blink();

  // Buzzer
  if (nano_eur <= 0.33) { // can be removed completely. If the price of nano is below 33 cents, then play a sound. Also, change the variable if you want to use a different crypto.
    tone(buzzer, 1000);
  }
}
