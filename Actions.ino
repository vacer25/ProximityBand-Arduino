void checkAlarm() {

  unsigned long currentMillis = millis();

  if (alarmIsActive) {

    //Serial.print("Cheking active alarm at: ");
    //Serial.println(currentMillis);

    if (!prev_alarmIsActive) {
      prev_alarmSwitchTime = currentMillis;
      alarmIsInHighState = true;
      digitalWrite(motorPin, HIGH);
      turnOnAllLEDs();
      //Serial.print("Initial alarm started at: ");
      //Serial.println(prev_alarmSwitchTime);
    }
    else {

      if (alarmIsInHighState) {
        if (currentMillis - prev_alarmSwitchTime > ALARM_ON_INTERVAL) {
          prev_alarmSwitchTime = currentMillis;
          alarmIsInHighState = false;
          digitalWrite(motorPin, LOW);
          turnOffAllLEDs();
          //Serial.print("Alarm was ON, turning OFF at: ");
          //Serial.println(prev_alarmSwitchTime);
        }
      }
      else {
        if (currentMillis - prev_alarmSwitchTime > ALARM_OFF_INTERVAL) {
          prev_alarmSwitchTime = currentMillis;
          alarmIsInHighState = true;
          digitalWrite(motorPin, HIGH);
          turnOnAllLEDs();
          //Serial.print("Alarm was OFF, turning ON at: ");
          //Serial.println(prev_alarmSwitchTime);
        }
      }

    }

  }
  else if (!alarmIsActive && prev_alarmIsActive) {
    digitalWrite(motorPin, LOW);
    turnOffAllLEDs();
    //Serial.println("Turning alarm OFF");
  }

  /*
    for (int repeat1Counter = 0; repeat1Counter < 3; repeat1Counter++) {

      for (int repeat2Counter = 0; repeat2Counter < 5; repeat2Counter++) {

        if (stopAlarmWithButton()) {
          break;
        }

        //digitalWrite(motorPin, HIGH);
        turnOnAllLEDs();

        if (stopAlarmWithButton()) {
          break;
        }
        delay(MOTOR_LONG_PULSE);
        if (stopAlarmWithButton()) {
          break;
        }

        digitalWrite(motorPin, LOW);
        turnOffAllLEDs();

        if (stopAlarmWithButton()) {
          break;
        }
        delay(MOTOR_SHORT_PULSE);

      }

      if (stopAlarmWithButton()) {
        break;
      }
      delay(1000);
      if (stopAlarmWithButton()) {
        break;
      }

    }

    digitalWrite(motorPin, LOW);
    turnOffAllLEDs();
    */

}

void updateRgbLEDFlashing() {

  unsigned long currentMillis = millis();
  //prev_rgbLEDFlashTime = currentMillis;

  for (int index = 0; index < 3; index++) {

    if (!rgbLEDFlashAciveStatus[index] && prev_rgbLEDFlashAciveStatus[index]) {
      setLED(false, index);
    }
    else if (rgbLEDFlashAciveStatus[index]) {
      if (currentMillis >= rgbLEDFlashTimes[index] || currentMillis < STARTUP_DELAY_TIME) {

        if (rgbLEDStatus[index]) {
          rgbLEDFlashTimes[index] = currentMillis + LED_FLASH_OFF_INTERVAL;
        }
        else {
          rgbLEDFlashTimes[index] = currentMillis + LED_FLASH_ON_INTERVAL;
        }

        setLED(!rgbLEDStatus[index], index);
        //rgbLEDStatus[index] = !rgbLEDStatus[index];
      }
    }

  }

}

void sendLEDWave(boolean activateMotor, int delayTime)  {

  for (int count = 0; count < 3; count++) {
    for (int index = 0; index < 3; index++) {
      setLED(count == index ? HIGH : LOW, index);
    }
    if (activateMotor) {
      digitalWrite(motorPin, HIGH);
      delay(MOTOR_SHORT_PULSE);
      digitalWrite(motorPin, LOW);
      delay(delayTime - MOTOR_SHORT_PULSE);
    }
    else {
      delay(delayTime);
    }
    setLED(0, count);
  }

  delay(3000);

}

void setupWaitingConnectionLEDFlasing() {

  unsigned long currentMillis = millis();

  turnOffAllLEDs();

  rgbLEDFlashAciveStatus[0] = true;
  rgbLEDFlashAciveStatus[1] = true;
  rgbLEDFlashAciveStatus[2] = true;

  rgbLEDFlashTimes[0] = currentMillis;
  rgbLEDFlashTimes[1] = currentMillis + (LED_FLASH_ON_INTERVAL + LED_FLASH_OFF_INTERVAL) - WAITING_CONNECTION_LED_SCAN_INTERVAL;
  rgbLEDFlashTimes[2] = currentMillis + (2 * LED_FLASH_ON_INTERVAL + 2 * LED_FLASH_OFF_INTERVAL) - 2 * WAITING_CONNECTION_LED_SCAN_INTERVAL;

}

void stopWaitingConnectionLEDFlasing() {

  turnOffAllLEDs();

  rgbLEDFlashAciveStatus[0] = false;
  rgbLEDFlashAciveStatus[1] = false;
  rgbLEDFlashAciveStatus[2] = false;
  
}

void turnOffAllLEDs() {
  for (int index = 0; index < 3; index++) {
    setLED(false, index);
  }
}

void turnOnAllLEDs() {
  for (int index = 0; index < 3; index++) {
    setLED(true, index);
  }
}

void setLED(boolean turnOn, byte index) {
  if (turnOn) {
    analogWrite(rgbLEDPins[index], currentLEDBrightness * rgbLEDBrightnessAdjustments[index]);
  }
  else {
    digitalWrite(rgbLEDPins[index], LOW);
  }
  rgbLEDStatus[index] = turnOn;
}
