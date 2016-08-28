/*
* Sample for the ArduinoUtil Library
*
*/

// Libraries

#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "ArduinoUtil.h"

ArduinoUtil util;

void setup() {

    Serial.begin(115200);

    // Initialize SPIFFS

    if (!SPIFFS.begin()) {
        Serial.println("* Error on SPIFFS.begin.");
    }

    // Exec WifiManager

    execWifiManager();

    ////// Update over air (OTA)

    initializeOTA();

    // Tests

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

    ////// Update over air (OTA)

    ArduinoOTA.handle();

}

// Execute the wifimanager for auto connection (and captiva portal)

void execWifiManager() {

    ////// WiFiManager

    WiFiManager wifiManager;

    // Timeout

    wifiManager.setTimeout(600);

    // Auto connect Wifi

    if(!wifiManager.autoConnect("AutoConnectAP")) {

        // Reset

        delay(3000);

        ESP.reset();

        delay(5000);
    }

}

// Initialize o Esp8266 OTA

void initializeOTA() {

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);
    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");
    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
        Serial.println("* OTA: Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\n*OTA: End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("*OTA: Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("*OTA: Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();

}
