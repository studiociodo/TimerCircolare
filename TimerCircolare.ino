#include <Arduino.h>
#include <ESP8266WiFi.h>        // https://github.com/esp8266/Arduino
#include <ESPAsyncTCP.h>        // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer/
#include <FastLED.h>            // https://github.com/FastLED/FastLED
#include <DNSServer.h>          // https://github.com/esp8266/Arduino

AsyncWebServer server(80);      // set internal web server to port 80
DNSServer dnsServer;            // set internal dns server


const char* input_parameter = "minuti";
int minuti = 0;
const String localIPURL = "http://172.0.0.1";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>TimerCircolare</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.0rem; color: #FF0000;}
  </style>
  </head><body>
  <br><br><br>
  <h2>TimerCircolare</h2> 
  <form action="/get">
    Durata in Minuti: <input type="text" name="minuti">
    <input type="submit" value="Inizia">
  </form><br>
</body></html>)rawliteral";


void setup() {
  pinMode(D2, OUTPUT);      // Mantain Mosfet powered on the entire circuit
  digitalWrite(D2, HIGH);
  pinMode(D4, OUTPUT);      // Onboard LED on Wemos D1 Mini
  digitalWrite(D4, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(172, 0, 0, 1), IPAddress(172, 0, 0, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP("TimerCircolare");    // Create Accesspoint with IP 172.0.0.1 and SSID TimerCircolare without password

  dnsServer.setTTL(3600);
  dnsServer.start(53, "*", IPAddress(172, 0, 0, 1));   // All DNS request must be routed on the IP of Wemos

  // Required
  server.on("/connecttest.txt", [](AsyncWebServerRequest * request) {
    request->redirect("http://logout.net");
  });  // windows 11 captive portal workaround
  server.on("/wpad.dat", [](AsyncWebServerRequest * request) {
    request->send(404);
  });               // Honestly don't understand what this is but a 404 stops win 10 keep calling this repeatedly and panicking the esp32 :)

  // Background responses: Probably not all are Required, but some are. Others might speed things up?
  // A Tier (commonly used by modern systems)
  server.on("/generate_204", [](AsyncWebServerRequest * request) {
    request->redirect(localIPURL);
  });       // android captive portal redirect
  server.on("/redirect", [](AsyncWebServerRequest * request) {
    request->redirect(localIPURL);
  });         // microsoft redirect
  server.on("/hotspot-detect.html", [](AsyncWebServerRequest * request) {
    request->redirect(localIPURL);
  });  // apple call home
  server.on("/canonical.html", [](AsyncWebServerRequest * request) {
    request->redirect(localIPURL);
  });     // firefox captive portal call home
  server.on("/success.txt", [](AsyncWebServerRequest * request) {
    request->send(200);
  });             // firefox captive portal call home
  server.on("/ncsi.txt", [](AsyncWebServerRequest * request) {
    request->redirect(localIPURL);
  });         // windows call home

  // return 404 to webpage icon
  server.on("/favicon.ico", [](AsyncWebServerRequest * request) {
    request->send(404);
  });  // webpage icon

  // Serve Basic HTML Page
  server.on("/", HTTP_ANY, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", index_html);
    response->addHeader("Cache-Control", "public,max-age=31536000");  // save this file to cache for 1 year (unless you refresh)
    request->send(response);
  });

  // the catch all
  server.onNotFound([](AsyncWebServerRequest * request) {
    request->redirect(localIPURL);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String input_message;

    if (request->hasParam(input_parameter)) {
      input_message = request->getParam(input_parameter)->value();
      if (input_message == "") {
        input_message = 5;
      }
    }

    request->send(200, "text/html", "<div align='center' style='font-size:2.0rem;'><br><br><br><h2>Partiti " + input_message + " minuti</h2></div>");
    minuti = input_message.toInt();



  });

  server.begin();


}

void loop() {
  dnsServer.processNextRequest();

  if ((millis() >= 120000 ) && (minuti == 0)){
    digitalWrite(D2, LOW);      // If after 2 minute from poweron anyone set timer to count it will shutdown itself
  }

  if (minuti > 0) {            // If timer is set light up the ring with 24 leds
    CRGB leds[24];
    FastLED.addLeds<WS2811, D3, GRB>(leds, 24).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  64 );

    leds[0] = 0xFF0000;   // from Red...
    FastLED.show();
    leds[1] = 0xFF0600;
    FastLED.show();
    leds[2] = 0xFF0E00;
    FastLED.show();
    leds[3] = 0xFF1000;
    FastLED.show();
    leds[4] = 0xFF2000;
    FastLED.show();
    leds[5] = 0xFF3000;
    FastLED.show();
    leds[6] = 0xFF4000;
    FastLED.show();
    leds[7] = 0xFF5000;
    FastLED.show();
    leds[8] = 0xFF6000;
    FastLED.show();
    leds[9] = 0xFF7000;
    FastLED.show();
    leds[10] = 0xFF8000;
    FastLED.show();
    leds[11] = 0xFF9000;
    FastLED.show();
    leds[12] = 0xFFA000;
    FastLED.show();
    leds[13] = 0xFFC000;
    FastLED.show();
    leds[14] = 0xFFE000;
    FastLED.show();
    leds[15] = 0xFFFF00;
    FastLED.show();
    leds[16] = 0xD0FF00;
    FastLED.show();
    leds[17] = 0xA0FF00;
    FastLED.show();
    leds[18] = 0x90FF00;
    FastLED.show();
    leds[19] = 0x80FF00;
    FastLED.show();
    leds[20] = 0x70FF00;
    FastLED.show();
    leds[21] = 0x30FF00;
    FastLED.show();
    leds[22] = 0x20FF00;
    FastLED.show();
    leds[23] = 0x00FF00;   // ...to Green
    FastLED.show();
    unsigned long millisled = ( 60000 * minuti) / 24;
    for (int i = 23; i != -1; i = i - 1) {    // blink every led for the duration requested
      CRGB colore = leds[i];
      unsigned long startblink = millis();
      unsigned long now = millis();
      while ( now - startblink <= millisled) {
        leds[i] = colore;
        FastLED.show();
        delay (1000);
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(1000);
        now = millis();
      }
      if (i == 22) {
        WiFi.mode(WIFI_OFF);    // after first led switchoff it shut down the wifi for save energy
      }
    }
    for (int i = 23; i != -1; i = i - 1) {   // at the end of time make blue circle with little sound and ...
      leds[i] = 0x0000FF;
      FastLED.show();
      tone(D8, i * 500);
      delay(100);
      noTone(D8);
    }
    while (true) {
      digitalWrite(D2, LOW);   // ... shutdown itself
    }
  }
}
