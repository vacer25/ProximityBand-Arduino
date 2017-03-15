void processUserInteraction() {

  getUserInputs();

  if (!bluetoothConnected) {

    if (!buttonIsPressed) {
      if (prev_buttonIsPressed) {
        turnOffAllLEDs();
        EEPROM_writeAnything(1, currentLEDBrightness);
      }
      sendLEDWave(false, WAITING_CONNECTION_LED_SCAN_SPEED);
    }
    else {
      if (!switchStatus) {
        currentLEDBrightness = minLEDLEDBrightness;
      }
      else if (switchStatus == 1) {
        currentLEDBrightness = mediumLEDLEDBrightness;
      }
      else {
        currentLEDBrightness = maxLEDLEDBrightness;
      }
      turnOnAllLEDs();
    }

  }

}

void getUserInputs() {

  unsigned long currentMillis = millis();

  buttonIsPressed = digitalRead(buttonPin);

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

void updateStatus() {

  if (bluetoothConnected && millis() - lastBluetoothDataInTime > bluetoothConnectionTimeout) {
    bluetoothConnected = false;
    activateOutOfRangeAlarm();
  }

  prev_switchStatus = switchStatus;
  prev_buttonIsPressed =  buttonIsPressed;

  prev_motorIsOn = motorIsOn;
  for (int index = 0; index < 3; index++) {
    prev_rgbLEDStatus[index] = rgbLEDStatus[index];
    prev_rgbLEDFlashAciveStatus[index] = rgbLEDFlashAciveStatus[index];
  }

}

boolean stopAlarmWithButton() {
  return digitalRead(buttonPin);
}
