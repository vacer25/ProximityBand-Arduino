void loadEEPROMData() {

  // Load previous Settings
  byte read_settings_version;
  EEPROM_readAnything(0, read_settings_version);

/*
  Serial.println(F("   SETTINGS VERSION"));
  Serial.println(F("-----------------------"));
  Serial.print(F("Read:   "));
  Serial.println(read_settings_version, BIN);
  Serial.print(F("Stored: "));
  Serial.println(settings_version, BIN);
  Serial.println();
*/

  if (settings_version == read_settings_version) { // If versions matchs, load variables from EEPROM
    //Serial.println(F("Did read EEPROM"));
    EEPROM_readAnything(1, currentLEDBrightness);
  }
  else {                                         // If not, initialize with defaults
    //Serial.println(F("Did write EEPROM"));
    EEPROM_writeAnything(0, settings_version);
    EEPROM_writeAnything(1, currentLEDBrightness);
  }
  
  //Serial.print(F("Current LED Brightness: "));
  //Serial.println(currentLEDBrightness);

}
