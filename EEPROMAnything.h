//---------------FONT + GRAPHIC-----------------------------//
// Yanked from:
// http://arduino.cc/playground/Code/EEPROMWriteAnything
//==========================================================//

#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
  //Serial.print(F("Saving "));
  //Serial.print(sizeof(value));
  //Serial.print(F(" bytes to index: "));
  //Serial.println(ee);
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}


