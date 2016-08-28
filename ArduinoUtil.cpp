////////
// Libraries Arduino
//
// Library: Util - Utilities for Arduino
// Author: Joao Lopes
// Versions:
//    - 0.9.0 Beta 1 - August 2016
///////

#include <Arduino.h>
#include <pins_arduino.h>

#include "ArduinoUtil.h"

// For ESP8266 - NodeMCU

#ifdef ESP8266
#include "FS.h"
#endif

///////

// String is numeric

boolean ArduinoUtil::isNum(String str) {

    boolean isNum = false;

    for (uint8_t i = 0; i < str.length(); i++) {
        isNum = isdigit(str.charAt(i)) || str.charAt(i) == '+' ||
                        str.charAt(i) == '.' || str.charAt(i) == '-';
        if (!isNum)
            return false;
    }

    return isNum;
}

// Convert string para integer

int32_t ArduinoUtil::convStrInt(String str) {

    if (isNum(str)) {

        int32_t valor = str.toInt();

        return valor;

    }

    return 0; // Numero invalido

}

// Converte string para float

float ArduinoUtil::convStrFloat(String str) {

	char floatbuf[5];

	str.toCharArray(floatbuf, sizeof(floatbuf));

	float ret = atof(floatbuf);

	return ret;
}

// Round float

float ArduinoUtil::roundFloat(float value, uint8_t decimals) {

    float factor = round(pow(10, decimals));

    int aux = round(value * factor);

    float ret = ((float) aux / factor);

    return ret;
}

// Convert float to string (dtostrf is not good)

String ArduinoUtil::convFloatStr(float value, uint8_t decimals, boolean comma) {

    // Convert

    String ret = String(roundFloat(value, decimals));

    // Comma ?

    if (comma)
        ret.replace('.', ',');

    // Return

    return ret;
}

String ArduinoUtil::formatFloat(float value, uint8_t intPlaces, uint8_t decPlaces, boolean comma)
{
    String ret = "";

    // Signal

    if (value < 0.0f) {
        value*=-1.0f;
        ret.concat('-');
    }
    // Integer

    float factor = round(pow(10, decPlaces));

    uint32_t aux = (uint32_t) roundFloat(value * factor, 0);

    uint32_t factorInt = (uint32_t) factor;

    uint32_t valInt = (uint32_t) (aux / factorInt);

    // Decimal

    uint32_t valDec = (aux - (valInt * factorInt));

    // Return

    if (intPlaces == 0) {
        ret.concat(valInt);
    } else {
        ret.concat(formatNumber(valInt, intPlaces));
    }

    ret.concat((comma)?',':'.');

    ret.concat(formatNumber(valDec, decPlaces));

    // Return

    return ret;

}

// Format numbers

String ArduinoUtil::formatNumber(uint32_t value, uint8_t size, char insert) {

    // Putting zeroes in left

    String ret = "";

    for (uint8_t i=1; i<=size; i++) {
        uint32_t max = pow(10, i);
        if (value < max) {
            for (uint8_t j=(size - i); j>0; j--) {
                ret.concat(insert);
            }
            break;
        }
    }

    ret.concat(value);

    return ret;
}

// Get field from a string with delimiters

String ArduinoUtil::getFieldStr(String str, uint8_t fieldNum, char delimiter) {

    uint16_t pos = 0;
    uint16_t lastPos = 0;
    uint16_t field = 0;
    uint16_t size = str.length() + 1;

    str.concat(delimiter);

    do {

        pos = str.indexOf(delimiter, lastPos);

        if (pos > 0) {

            if (pos == size) {

                break;

            }

            field++;

            if (field == fieldNum) {

                return (str.substring(lastPos, pos));

            }

            lastPos = pos + 1;

        }
    }
    while (pos > 0);

    return ""; // If not found, return empty

}

// Get field char

char ArduinoUtil::getFieldChar(String str, uint8_t fieldNum, char delimiter) {

    String aux = getFieldStr(str, fieldNum, delimiter);
    return aux.charAt(0);
}


// Get field integer

int32_t ArduinoUtil::getFieldInt(String str, uint8_t fieldNum, char delimiter) {

    String aux = getFieldStr(str, fieldNum, delimiter);

    if (isNum(aux)) { // OK

        return aux.toInt();

    } else { // Err

        String msgErr = "*ArduinoUtil:getFieldInt: value of field ";
        msgErr.concat(fieldNum);
        msgErr.concat(" not is a number: ");
        msgErr.concat(aux);
        Serial.println(msgErr);

        return 0;
    }
}

// Get field float

float ArduinoUtil::getFieldFloat(String str, uint8_t fieldNum, char delimiter) {

    String aux = getFieldStr(str, fieldNum, delimiter);

    if (isNum(aux)) { // Ok

        return aux.toFloat();

    } else { // Err

        String msgErr = "*ArduinoUtil:getFieldFloat: value of field ";
        msgErr.concat(fieldNum);
        msgErr.concat(" not is a float: ");
        msgErr.concat(aux);
        Serial.println(msgErr);

        return 0;
    }
}

////// For ESP8266

#ifdef ESP8266

// Open a file

File ArduinoUtil::openFile(const char* fileName, const char* mode) {

    // Open

    File file = SPIFFS.open(fileName, mode);

    if (!file) {
        Serial.println("*ArduinoUtil:openFile:file open failed");
    }

    // Return

    return file;

}

// Remove a file

boolean ArduinoUtil::removeFile(const char* fileName) {

    // Exists ?

    if (SPIFFS.exists(fileName) == false) { // Not exist - considered removed
        return true;
    } else { // Delete
        return SPIFFS.remove(fileName);
    }

}

// Read all the text of file

String ArduinoUtil::readTextFile(const char* fileName) {

    String data = "";

    // File exists ?

    if (SPIFFS.exists(fileName) == false) { // Not exist - return zero string
        return "";
    }

    // Open the file

    File file = openFile(fileName, "r");

    if (file) { // Success

        // Get all the text

        data = file.readString();

        // Close file

        file.close();

    } else {

        // Return a empty string

        data = "";
    }

    // Return

    return data;

}
#endif

/////// End
