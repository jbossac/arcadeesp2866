#include <ESP8266WiFi.h>
#include <IRsend.h>
#include <ir_LG.h>

const int IRPIN = D7;

IRsend irsend(IRPIN);

const char *ssid = "ffwifiubq2";
const char *password = "hura-3613";

void setup()
{

    Serial.begin(9600);
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
}

void sendTvSwitch()
{
    uint64_t code = 0x20DF10EF;
    irsend.sendNEC(code, 32);
}

void lightNotify(){
    
}

void loop()
{
    Serial.println("Hola prueba Wemos2");

    Serial.println("Envio Code!");
    delay(1000);
}
