#include <ESP8266WiFi.h>

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
}

void loop()
{
    Serial.println("Hola prueba Wemos2");
    delay(1000);
}