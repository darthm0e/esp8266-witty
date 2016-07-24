#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>
#include <Adafruit_NeoPixel.h>
#define pixelCount 1
#define pixelPin 5
//NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang400KbpsMethod> strip(pixelCount, pixelPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);
//NeoPixelBus strip = NeoPixelBus(pixelCount, 4, NEO_GRB);

bool rainbow = false;
const char *ssid = "ESP-net";
const char *password = "35p-r0cks-iot";

MDNSResponder mdns;
ESP8266WebServer server ( 80 );
const int led = 13;
void handleRoot() {
    digitalWrite ( led, 1 );
  String out = "<html><head><title>Wifi light</title></head>";
  out += "<body style='background-color:gray'>";
  out += "<span style='display:block; width:100%; font-size:2em; font-family:Verdana; text-align:center'>Choose color</span><br/>";
  out += "<a href='white'><span style='display:block; text-align:center; background-color:white; width:100%; height:2em;'>WHITE</span></a><br/>";
  out += "<a href='red'><span style='display:block; text-align:center; background-color:red; width:100%; height:2em;'>RED</span></a><br/>";
  out += "<a href='green'><span style='display:block; text-align:center; background-color:green; width:100%; height:2em;'>GREEN</span></a><br/>";
  out += "<a href='blue'><span style='display:block; text-align:center; background-color:blue; width:100%; height:2em;'>BLUE</span></a><br/>";
  out += "<a href='black'><span style='display:block; text-align:center; text-color:white; background-color:black; width:100%; height:2em;'>OFF</span></a><br/>";
  out += "<a href='yellow'><span style='display:block; text-align:center; background-color:yellow; width:100%; height:2em;'>YELLOW</span></a><br/>";
  out += "<a href='rainbow'><span style='display:block; text-align:center; background-color:white; width:100%; height:2em;'>RAINBOW</span></a>";
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
  server.on ( "/white", []() {colorWipe(strip.Color(127, 127, 127), 50); strip.show(); rainbow = false; handleRoot();} );
  server.on ( "/red", []() {colorWipe(strip.Color(255, 0, 0), 50); strip.show(); rainbow = false; handleRoot();} );
  server.on ( "/green", []() {colorWipe(strip.Color(0, 255, 0), 50); strip.show(); rainbow = false; handleRoot();} );
  server.on ( "/blue", []() {colorWipe(strip.Color(0, 0, 255), 50); strip.show(); rainbow = false; handleRoot();} );
  server.on ( "/black", []() {colorWipe(strip.Color(0, 0, 0), 50); strip.show(); rainbow = false; handleRoot();} );
  server.on ( "/yellow", []() {colorWipe(strip.Color(127, 127, 0), 50); strip.show(); rainbow = false; handleRoot();} );
  server.on ( "/rainbow", []() {rainbow = true; handleRoot(); rainbowCycle(200); handleRoot();} );
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

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
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
