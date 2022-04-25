#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <Espalexa.h>
#define D1 2 // reley light
#define D2 0 // ir transmissor

IRsend irsend(D2);
Espalexa espAlexa;

const char* WIFI_SSID = "INTERNET_DO_VIZINHO";
const char* WIFI_PASS = "tanio123";

long irValue;

void interageDoor(uint8_t state){
  if (state) {
    digitalWrite(D1, LOW);
  }
  else {
    digitalWrite(D1, HIGH);
  }
}

void interageTV(uint8_t state){
  Serial.print("TV CHANGED!");
  irValue = 0xE0E040BF;
  //irValue = 0x40BF;
  irsend.sendNEC(irValue, 32);
  delay(50);
  irsend.sendRC5(irValue, 32);
  delay(50);
  irsend.sendRC5(irValue, 32);
}

void connectWifi(){
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("");
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    IPAddress ip(192, 168, 25, 15);
    IPAddress gateway(192, 168, 25, 1);
    IPAddress subnet(255, 255, 255, 0);
    Serial.print("Configuring fixed IP to : ");
    Serial.println(ip);
  
    WiFi.config(ip, gateway, subnet);
    Serial.print("Served in: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
  digitalWrite(D1, HIGH);

  espAlexa.addDevice("TV", interageTV);
  espAlexa.begin();
}

void loop() {
  connectWifi();
  espAlexa.loop();
  delay(1);
}
