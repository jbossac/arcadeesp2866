#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRsend.h>
#include <ir_LG.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_VL53L0X.h"


const int POWERSW = D8;
const int LedsPin = D7;
const int IRPIN = D6;


const int led = D1; //Led de la placa.


IRsend irsend(IRPIN);

ESP8266WebServer server(80);
const char *ssid = "ffwifiubq2";
const char *password = "hura-3613";


#define DELAYVAL 100
#define NUMPIXELS 15 // Neopixel strip
Adafruit_NeoPixel pixels(NUMPIXELS, LedsPin, NEO_GRB + NEO_KHZ800);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup()
{

 pinMode(POWERSW, INPUT_PULLUP);
    Serial.begin(115200);
    Serial.println("hola");
    if (!lox.begin())
    {
        Serial.println(F("Failed to boot VL53L0X"));
        while (1)
            ;
    }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    irsend.begin();
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)



  server.on("/", handleRoot); // what happens in the root
//  server.on("/on",led_on); // turn led on from webpage /on
//  server.on("/off", led_off); // turn led off from webpage /off

server.on("/pcon",prendePc); // turn led on from webpage /on
server.on("/pcoff",apagaPc); // turn led on from webpage /on


server.begin();


}

void checkDistance()
{
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4)
    { // phase failures have incorrect data
        Serial.print("Distance (mm): ");
        Serial.println(measure.RangeMilliMeter);
    }
    else
    {
        Serial.println(" out of range ");
    }
}
void sendTvSwitch()
{
    uint64_t code = 0x20DF10EF;
    irsend.sendNEC(code, 32);
}

void prendePc(){
    server.send(200, "text/plain", "ApretoBoton");
  pinMode(POWERSW,OUTPUT);
  digitalWrite(POWERSW, LOW);
  delay(400);
  pinMode(POWERSW,INPUT_PULLUP);
}


void apagaPc(){
   server.send(200, "text/plain", "ApagandoPC!!");
  pinMode(POWERSW,OUTPUT);
  digitalWrite(POWERSW,LOW);
  delay(4000); //tenerlo presionado 3 segs.
  pinMode(POWERSW,INPUT_PULLUP);
}



void prendeLuz()
{
    pixels.clear(); // Set all pixel colors to 'off'

    // The first NeoPixel in a strand is #0, second is 1,< all the way up
    // to the count of pixels minus one.
    for (int i = 0; i < NUMPIXELS; i++)
    { // For each pixel...

        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        // Here we're using a moderately bright green color:
        pixels.setPixelColor(i, pixels.Color(0, 0, 150));

        pixels.show(); // Send the updated pixel colors to the hardware.

        delay(DELAYVAL); // Pause before next pass through loop
        if (i == NUMPIXELS - 1)
        {
            //delay(DELAYVAL*5);
        }
    }
}

void handleRoot() {
    
  server.send(200, "text/plain", "hello from esp8266! (blinking LED)");
  int led_state = 0;
  for (int ii=0;ii<10;ii++){
    digitalWrite(led,led_state);
    led_state = !led_state;
    delay(100);
  }
}


void loop()
{
      server.handleClient();
    Serial.println("Hola prueba Wemos2");

    Serial.println("Envio Code!");
    sendTvSwitch();
    delay(1000);

    prendeLuz();
    checkDistance();
}
