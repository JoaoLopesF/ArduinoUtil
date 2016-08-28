/*
* Sample for the ArduinoUtil Library
*
*/

// Libraries

#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// SSID and password

const char* ssid = ".....";
const char* password = ".....";

// Arduino Util Library

#include "ArduinoUtil.h"

ArduinoUtil util;

void setup() {

    Serial.begin(115200);

    // WiFi connection

    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");

    // Initialize SPIFFS

    if (!SPIFFS.begin()) {
        Serial.println("* Error on SPIFFS.begin.");
    }

    // Examples

    Serial.println ("* Arduino Util Library");
    Serial.printf("isNum: 1 -> ");
    Serial.println(util.isNum("1"));

    Serial.print("isNum: A ->");
    Serial.println(util.isNum("A"));

    Serial.print("Field 2 A:B:C -> ");
    Serial.println(util.getFieldStr("A:B:C", 2));

    Serial.print("Field 2 1:2:3 -> ");
    Serial.println(util.getFieldInt("1:2:3", 2));

    Serial.print("Field 2 1.1:1.2:1.3 -> ");
    Serial.println(util.getFieldStr("1.1:1.2:1.3", 2));

    Serial.print("formatFloat 3 dec.: 5.0 ->");
    Serial.println(util.formatFloat(5.0f, 0, 3));

    Serial.print("formatFloat 3 dec.: 5.1 ->");
    Serial.println(util.formatFloat(5.1f, 0, 3));

    Serial.print("formatFloat 3 dec.: 5.12 ->");
    Serial.println(util.formatFloat(5.12f, 0, 3));

    Serial.print("formatFloat 3 dec.: 5.123 ->");
    Serial.print(util.formatFloat(5.123f, 0, 3));
    Serial.print(" ");
    Serial.println(util.formatFloat(5.123f, 5, 3));

    Serial.print("formatFloat 3 dec.: 5.1234 ->");
    Serial.print(util.formatFloat(5.1234f, 0, 3));
    Serial.print(" ");
    Serial.println(util.formatFloat(5.1234f, 5, 3));

    Serial.print("formatFloat 3 dec. with comma: 5.1234 ->");
    Serial.print(util.formatFloat(5.1234f, 0, 3, true));
    Serial.print(" ");
    Serial.println(util.formatFloat(5.1234f, 5, 3, true));

    Serial.print("roundFloat 2 dec.: 5.1234 ->");
    Serial.println(util.roundFloat(5.1234f,2));

    Serial.print("roundFloat 2 dec.: 5.567890 ->");
    Serial.println(util.roundFloat(5.567890f,2));

    // Open file to write

    File file = util.openFile("/test.txt", "w");

    if (file) {

        Serial.print("Write file test.txt -> test:1:1.23 -> ");

        file.print("test:1:1.23");

        file.close();

        // Read file

        Serial.print("Read file test.txt -> test:1:1.23 -> ");
        String data = util.readTextFile("/test.txt");
        Serial.println(data);

        // Get fields by type

        Serial.print("Field 1 str -> ");
        Serial.println(util.getFieldStr(data, 1));

        Serial.print("Field 2 int -> ");
        Serial.println(util.getFieldStr(data, 2));

        Serial.print("Field 3 float -> ");
        Serial.println(util.getFieldFloat(data, 3));

        // Remove file

        Serial.print("remove file test.txt -> ");
        Serial.println(util.removeFile("/test.txt"));

    }

}

void loop()
{
}
