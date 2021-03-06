#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>
#include <Adafruit_NeoPixel.h>
#define pixelCount 15
#define pixelPin 5
//NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang400KbpsMethod> strip(pixelCount, pixelPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);
//NeoPixelBus strip = NeoPixelBus(pixelCount, 4, NEO_GRB);

bool rainbow = false;
bool loops = false;
bool theater = false;
bool faders = false;

// wenn auskommentiert: delay()
// wenn aktiv: deep sleep nutzen (Hardwaremodifikation notwendig)
//#define DEEP


const char *ssid = "ESP-net"; //wlan ssid
const char *password = "35p-r0cks-iot"; //wlan password

// Update these with values suitable for your network.
IPAddress ip(192,168,2,244);  //Node static IP
IPAddress gateway(192,168,2,112);
IPAddress subnet(255,255,255,0);

MDNSResponder mdns;
ESP8266WebServer server ( 80 );
const int led = 13;
void handleRoot() {
    digitalWrite ( led, 1 );
  String out = "<html><head><title>OcknigmaLabs Immerlicht</title></head>";
  out += "<body style='background-color:#323232'>";
  out += "<span style='display:block; width:100%; font-size:1.5em; font-family:Verdana; color:white; text-align:center'>-# OcknigmaLabs Wifi light  --#--  IP:192.168.2.244 - Node:2 #-</span><br/>";
  out += "<div id='colors' style='padding: 0% 0%; display:block; margin:auto; position:relative;'>";
  out += "<a href='blue'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:blue; width:21%; height:4em;'>BLUE</span></a>";
  out += "<a href='red'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:red; width:21%; height:4em;'>RED</span></a>";
  out += "<a href='green'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:green; width:21%; height:4em;'>GREEN</span></a><br/>";
  out += "</div>";
  out += "<div id='colors' style='padding: 1% 0%; display:block; margin:auto; position:relative;'>";
  out += "<a href='emerald'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#00ffff; width:21%; height:4em;'>EMERALD</span></a>";
  out += "<a href='pink'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#ff00ff; width:21%; height:4em;'>PINK</span></a>";   
  out += "<a href='yellow'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:yellow; width:21%; height:4em;'>YELLOW</span></a><br/>";
  out += "</div>";  
  out += "<div id='effect' style='padding: 0% 0%; display:block; margin:auto; position:relative;'>";
  out += "<a href='looper'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#f1f1f1; width:21%; height:4em;'>ColorLoop</span></a>";
  out += "<a href='chaser'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#d9d9d9; width:21%; height:4em;'>ColorChaser</span></a>";
  out += "<a href='rainbow'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#f1f1f1; width:21%; height:4em;'>RAINBOW</span></a>";
  out += "</div>";  
  out += "<div id='off' style='padding: 1% 0%; display:block; margin:auto; position:relative;'>";
  out += "<a href='fader'><span style='display:inline-table; padding: 2% 6%; text-align:center; color:black; background-color:#d9d9d9; width:87%; height:3em;'>FADER</span></a><br/>";
  out += "<a href='white'><span style='display:inline-table; padding: 2% 6%; text-align:center; color:black; background-color:white; width:38%; height:3em;'>WHITE</span></a>";
  out += "<a href='off'><span style='display:inline-table; padding: 2% 6%; text-align:center; color:white; background-color:black; width:38%; height:3em;'>OFF / FADER OFF</span></a>";
  out += "</div>";
  out += "<span style='display:block; padding: 1% 6%; margin:auto; color: white; text-align:center;'>OcknigmaLabs-2016 # Sourcecode: <a href='#'>https://github.com/darthm0e/esp8266-witty</a></span>";
  out += "</body>";
  out += "</html>";
    server.send ( 200, "text/html", out );
    digitalWrite ( led, 0 );
}
void handleNotFound() {
    digitalWrite ( led, 1 );
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
        message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }
    server.send ( 404, "text/plain", message );
    digitalWrite ( led, 0 );
}
void setup ( void ) {
    pinMode ( led, OUTPUT );
    digitalWrite ( led, 0 );
  strip.begin();
  strip.show();

    Serial.begin ( 115200 );
    WiFi.begin ( ssid, password );
    WiFi.config(ip, gateway, subnet);
    Serial.println ( "" );
    // Wait for connection
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        Serial.print ( "." );
    }
    Serial.println ( "" );
    Serial.print ( "Connected to " );
    Serial.println ( ssid );
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP() );
    if ( mdns.begin ( "esp8266", WiFi.localIP() ) ) {
        Serial.println ( "MDNS responder started" );
    }
    server.on ( "/", []() {handleRoot();} );
  server.on ( "/white", []() {colorWipe(strip.Color(127, 127, 127), 20); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/red", []() {colorWipe(strip.Color(255, 0, 0), 20); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/green", []() {colorWipe(strip.Color(0, 255, 0), 20); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/blue", []() {colorWipe(strip.Color(0, 0, 255), 20); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/pink", []() {colorWipe(strip.Color(200, 0, 200), 20); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/emerald", []() {colorWipe(strip.Color(0, 200, 200), 20); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/yellow", []() {colorWipe(strip.Color(200, 200, 0), 20); theater = false; loops = false; rainbow = false; strip.show(); handleRoot();} );
  server.on ( "/off", []() {lightoff(20); rainbow = false; theater = false; faders = false; loops = false; strip.show(); handleRoot();} );
  server.on ( "/rainbow", []() {rainbow = true; theater = false; loops = false; handleRoot(); rainbowCycle(200); handleRoot();} );
  server.on ( "/chaser", []() {loops = false; theater = true; rainbow = false; faders = false; handleRoot(); theaterChaseRainbow(1000); handleRoot();} );
  server.on ( "/looper", []() {theater = false; loops = true; rainbow = false; faders = false; handleRoot(); looper(6000); handleRoot();} );
  server.on ( "/fader", []() {rainbow = false; theater = false; loops = false; handleRoot(); fader(50);} );
    server.onNotFound ( handleNotFound );
    server.begin();
    Serial.println ( "HTTP server started" );

    Serial.print ( "Auto Rainbow Mode" );
    rainbow = true;
    rainbowCycle(200); 
    handleRoot(); 
}
void loop ( void ) {
    mdns.update();
    server.handleClient();
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
//LightsOff
void lightoff(uint8_t wait) {
  colorWipe(strip.Color(0, 0, 0), 10);
  strip.setBrightness(250); 
  delay(50);
  for(uint16_t i=0; i<4; i++) {
    delay(200);
    server.handleClient();
  }
  strip.show();    
  Serial.println ( "Lights off" );
  #ifdef DEEP
    Serial.println ( "Sleep" );
    ESP.deepSleep(60000000, WAKE_RF_DEFAULT);
    delay(100);
  #else
    //@todo disconnect WiFi to save power?
    delay(100 * wait);
  #endif
}
//Fader Test
void fader(uint8_t wait) {
  if (faders == true) {
    faders=false;
    Serial.println ( "fader off" );
    strip.setBrightness(250);
    strip.show();
  }
  else {
    faders=true;
    Serial.println ( "fader started" );
  }
  while ( faders == true ) {    
  strip.Color(0, 0, 250);
  strip.setBrightness(10);
  strip.show();  
  for(uint16_t i=10; i<255; i=i+15) {     
      delay(wait);      
      strip.setBrightness(i);
      strip.show();      
  }
  delay(200);
  for(uint16_t i=255; i>0; i=i-15) {             
      delay(wait);      
      strip.setBrightness(i);
      strip.show();      
  }
  server.handleClient();
  delay(200);
  }  
}


void looper(uint8_t wait) {
      strip.setBrightness(250);
       while ( loops == true ) {
        theater = false;
        rainbow = false;
        Serial.println ( "looper started" );
          colorWipe(strip.Color(127, 0, 0), 20);
          delay(300);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(0, 127, 0), 20);
          delay(300);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(0, 0, 127), 20);
          delay(300);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(127, 127, 0), 20);
          delay(300);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(127, 0, 127), 20);
          delay(300);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(0, 127, 127), 20);
          delay(300);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
      }
      Serial.println ( "looper end" );
  }

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  theater = false;
  loops = false;
  Serial.println ( "rainbow started" );
  strip.setBrightness(255);
  do
  {
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }      
      strip.show();
      server.handleClient();
      delay(wait);
      if (rainbow == false) {
        break;
      }
    }    
    delay(250);
    mdns.update();
    server.handleClient();
    delay(500);
  } while ( rainbow == true );
   Serial.println ( "rainbow end" );
}
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  Serial.println ( "chaser started" );
  strip.setBrightness(255);
  while (theater == true)  {   
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on            
      }
      strip.show();      
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      server.handleClient();      
    } 
    if (theater == false){
      break;   
    }
  }
  }
  Serial.println ( "chaser end" );
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
