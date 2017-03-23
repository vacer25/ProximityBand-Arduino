void processUserInteraction() {

  unsigned long currentMillis = millis();

  getUserInputs();

  if (alarmIsActive) {
    checkToStopAlarmWithButton();
  }

  if (!bluetoothConnected) {

    if (!buttonIsPressed) {
      if (prev_buttonIsPressed) {
        EEPROM_writeAnything(1, currentLEDBrightness);
        setupWaitingConnectionLEDFlasing();
      }
      //sendLEDWave(false, WAITING_CONNECTION_LED_SCAN_SPEED);
    }
    else if(buttonIsPressed && canEnterBrightnessSettingMode) {
      if (!switchStatus) {
        currentLEDBrightness = maxLEDLEDBrightness;
      }
      else if (switchStatus == 1) {
        currentLEDBrightness = mediumLEDLEDBrightness;
      }
      else {
        currentLEDBrightness = minLEDLEDBrightness;
      }
      turnOnAllLEDs();
    }

  }

}

void getUserInputs() {

  unsigned long currentMillis = millis();

  buttonIsPressed = digitalRead(buttonPin);
  if(!buttonIsPressed) {
    canEnterBrightnessSettingMode = true;
  }

  boolean switchIsInPos1 = digitalRead(switchPos1Pin);
  boolean switchIsInPos2 = digitalRead(switchPos2Pin);

  if (switchIsInPos1) {
    temp_switchStatus = 0;
  }
  else if (switchIsInPos2) {
    temp_switchStatus = 1;
  }
  else {
    temp_switchStatus = 2;
  }

  if (!switchIsBeingSwitched && temp_switchStatus != switchStatus) {
    switchInteractionTime = currentMillis;
    switchIsBeingSwitched = true;
  }

  if (switchIsBeingSwitched) {
    if (currentMillis - switchInteractionTime > SWITCH_DEBOUNCE_TIME) {
      switchIsBeingSwitched = false;
      switchStatus = temp_switchStatus;
    }
  }

}

boolean checkToStopAlarmWithButton() {
  if (buttonIsPressed) {
    alarmIsActive = false;
  }
}
