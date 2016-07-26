#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>
#include <Adafruit_NeoPixel.h>
#define pixelCount 59
#define pixelPin 5
//NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang400KbpsMethod> strip(pixelCount, pixelPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);
//NeoPixelBus strip = NeoPixelBus(pixelCount, 4, NEO_GRB);

bool rainbow = false;
bool loops = false;
bool theater = false;

const char *ssid = "ESP-net";
const char *password = "35p-r0cks-iot";

MDNSResponder mdns;
ESP8266WebServer server ( 80 );
const int led = 13;
void handleRoot() {
    digitalWrite ( led, 1 );
  String out = "<html><head><title>OcknigmaLabs Wifi light</title></head>";
  out += "<body style='background-color:#323232'>";
  out += "<span style='display:block; width:100%; font-size:2em; font-family:Verdana; color:white; text-align:center'>Choose color</span><br/>";
  out += "<div id='colors' style='padding: 3% 0%; display:block; padding: 1% 1%; margin:auto; position:relative;'>";
  out += "<a href='white'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:white; width:21%; height:4em;'>WHITE</span></a>";
  out += "<a href='red'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:red; width:21%; height:4em;'>RED</span></a>";
  out += "<a href='green'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:green; width:21%; height:4em;'>GREEN</span></a><br/>";
  out += "</div>";
  out += "<div id='colors' style='padding: 3% 0%; display:block; padding: 1% 1%; margin:auto; position:relative;'>";
  out += "<a href='blue'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:blue; width:21%; height:4em;'>BLUE</span></a>";
  out += "<a href='pink'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#f100f1; width:21%; height:4em;'>PINK</span></a>";   
  out += "<a href='yellow'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:yellow; width:21%; height:4em;'>YELLOW</span></a><br/>";
  out += "</div>";  
  out += "<div id='effect' style='padding: 3% 0%; display:block; padding: 1% 1%; margin:auto; position:relative;'>";
  out += "<a href='looper'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#f1f1f1; width:21%; height:4em;'>ColorLoop</span></a>";
  out += "<a href='chaser'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#e9e9e9; width:21%; height:4em;'>ColorChaser</span></a>";
  out += "<a href='rainbow'><span style='display:inline-table; padding: 1% 6%; margin:auto; text-align:center; background-color:#f1f1f1; width:21%; height:4em;'>RAINBOW</span></a>";
  out += "</div>";  
  out += "<div id='off' style='padding: 3% 0%; display:block; padding: 1% 1%; margin:auto; position:relative;'>";
  out += "<a href='black'><span style='display:inline-table; padding: 2% 6%; text-align:center; color:white; background-color:black; width:99%; height:5em;'>OFF</span></a><br/>";
  out += "<span style='display:block; padding: 1% 6%; margin:auto; text-align:center;'>OcknigmaLabs-2016 # Sourcecode: <a href='#'>https://github.com/darthm0e/esp8266-witty</a></span>";
  out += "</div>";
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
  server.on ( "/black", []() {colorWipe(strip.Color(0, 0, 0), 10); strip.show(); rainbow = false; theater = false; loops = false; handleRoot();} );
  server.on ( "/yellow", []() {colorWipe(strip.Color(200, 200, 0), 20); theater = false; loops = false; rainbow = false; strip.show(); handleRoot();} );
  server.on ( "/rainbow", []() {rainbow = true; theater = false; loops = false; handleRoot(); rainbowCycle(200); handleRoot();} );
  server.on ( "/chaser", []() {loops = false; theater = true; rainbow = false; handleRoot(); theaterChaseRainbow(1000); handleRoot();} );
  server.on ( "/looper", []() {theater = false; loops = true; rainbow = false; handleRoot(); looper(6000); handleRoot();} );
    server.onNotFound ( handleNotFound );
    server.begin();
    Serial.println ( "HTTP server started" );
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

void looper(uint8_t wait) {
       while ( loops == true ) {
        theater = false;
        rainbow = false;
          colorWipe(strip.Color(127, 0, 0), 20);
          delay(100);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(0, 127, 0), 20);
          delay(100);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(0, 0, 127), 20);
          delay(100);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(127, 127, 0), 20);
          delay(100);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(127, 0, 127), 20);
          delay(100);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
          colorWipe(strip.Color(0, 127, 127), 20);
          delay(100);
          colorWipe(strip.Color(0, 0, 0), 15);
          server.handleClient();
          if (loops == false) {
            break;
          }
      }
  }

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  theater = false;
  loops = false;
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
    
    Serial.println ( "LOOP");
    delay(250);
    mdns.update();
    server.handleClient();
    delay(500);
  } while ( rainbow == true );
   Serial.println ( "rainbow end" );
}
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
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
