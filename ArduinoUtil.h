////////
// Header for ArduinoUtil
///////

#ifndef ArduinoUtil_h
#define ArduinoUtil_h

#include "Arduino.h"

#ifdef ESP8266
#include "FS.h"
#endif

// Classe

class ArduinoUtil
{
	public:

    // Utilities for NodeMCU

    boolean isNum(String str);

	int32_t convStrInt(String str);
	float convStrFloat(String str);

	float roundFloat(float value, uint8_t decimals = 2);

    String convFloatStr(float value, uint8_t decimals = 2, boolean comma = false);

	String formatNumber(uint32_t number, uint8_t size, char insert='0');
    String formatFloat(float value, uint8_t intPlaces=0, uint8_t decPlaces=2, boolean comma=false);

    String getFieldStr(String str, uint8_t fieldNum, char delimiter = ':');
    char getFieldChar(String str, uint8_t fieldNum, char delimiter = ':');
    int32_t getFieldInt(String str, uint8_t fieldNum, char delimiter = ':');
    float getFieldFloat(String str, uint8_t fieldNum, char delimiter = ':');

    // For Esp8266

#ifdef ESP8266

    File openFile(const char*  fileName, const char*  mode = "r");
    boolean removeFile(const char*  fileName);

    String readTextFile(const char*  fileName);

#endif

};

#endif // Inicial
