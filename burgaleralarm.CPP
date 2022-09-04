#include <Arduino.h>
#include <ArduinoJson.hpp>
#include <ESP8266WiFi.h>
#include <UniversalTelegrambot.h>
#include <WiFiClientSecure.h>
#define Chat  "-603470556" 
#define name "your network name"
#define pass "your ntwork password"
X509List cert (TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client ;
UniversalTelegramBot burgb( "your bot token" , secured_client);  //bot API from the bot father
int D;
int F;
void setup() {
Serial.begin(115200);
secured_client.setTrustAnchors(&cert);
WiFi.mode(WIFI_STA);
configTime(0,0,"pool.ntp.org");
delay(1000);
WiFi.begin(name,pass);
while (WiFi.status() != WL_CONNECTED){  // checks WiFi is connected 
Serial.println("...");
delay(2000);
 

}
Serial.println("connected");
pinMode(5,OUTPUT);
pinMode(16,INPUT);
burgb.longPoll = 60 ; // amount of time request to send message stays at telegram not needed but speeds up the process
}
int dist() {                           //function to measure distance
digitalWrite(5,LOW); 
delayMicroseconds(5);
digitalWrite(5,HIGH);
delayMicroseconds(10);
digitalWrite(5,LOW);
int x = pulseIn(16,HIGH);
return x ;
}
void loop() {
bool c = 0 ;
D = dist();
burgb.sendMessage( Chat , "Start" , "");
while (D > 0 ) {
F = dist();
if(F > D+70 || F < D-70 ){
    if (c == 1) {
burgb.sendMessage( Chat , "Door open" , "");
c = 0 ; 
    } // I use a large range and double cchecks because 3V VCC makes readings innacurate
   while( F > D+70 || F < D-70 ){
   F = dist();
   if (F < D+70 && F > D-70){ //check
    F = dist();
   }
   }
}
else {
    if (c == 0) {
burgb.sendMessage( Chat , "Door Closed" , "");
c = 1 ;
    }
while (F < D+70 && F > D-70) {
F = dist() ;    
if (F > D+70 || F < D-70) { // check
    F = dist();
}
}
}
}
}
