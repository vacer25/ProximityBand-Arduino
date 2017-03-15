void activateOutOfRangeAlarm() {

  for (int repeat1Counter = 0; repeat1Counter < 3; repeat1Counter++) {

    for (int repeat2Counter = 0; repeat2Counter < 5; repeat2Counter++) {

      if (stopAlarmWithButton()) {
        break;
      }

      digitalWrite(motorPin, HIGH);
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

}

void updateRgbLEDFlashing() {

  unsigned long currentMillis = millis();
  //prev_rgbLEDFlashTime = currentMillis;

  for (int index = 0; index < 3; index++) {

    if (!rgbLEDFlashAciveStatus[index] && prev_rgbLEDFlashAciveStatus[index]) {
      setLED(false, index);
      rgbLEDStatus[index] = 0;
    }

    if (rgbLEDFlashAciveStatus[index]) {
      boolean flashLEDNow = false;
      if (rgbLEDStatus[index]) {
        if (currentMillis - rgbLEDFlashTimes[index] > LED_FLASH_ON_INTERVAL) {
          flashLEDNow = true;
        }
      }
      else {
        if (currentMillis - rgbLEDFlashTimes[index] > LED_FLASH_OFF_INTERVAL) {
          flashLEDNow = true;
        }
      }
      if (flashLEDNow) {
        rgbLEDFlashTimes[index] = currentMillis;
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
