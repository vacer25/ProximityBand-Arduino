void getBluetoothData() {

  //attemptBluetoothConnection();

  unsigned long currentMillis = millis();

  while (ble.available() /* || Serial.available() */)  {

    if (!bluetoothConnected) {
      alarmIsISuppressedState = false;
      stopWaitingConnectionLEDFlasing();
    }

    lastBluetoothDataInTime = currentMillis;
    bluetoothConnected = true;
    int currentReadChar = ble.read();
    /*
    if (currentReadChar == 0 || currentReadChar == -1) {
      currentReadChar = Serial.read();
    }
    */
    Serial.print((char)currentReadChar);

    // -------------------- LEDs ON --------------------

    if (currentReadChar == redLEDOnCommand) {
      setLED(true, 0);
    }
    else if (currentReadChar == greenLEDOnCommand) {
      setLED(true, 1);
    }
    else if (currentReadChar == blueLEDOnCommand) {
      setLED(true, 2);
    }

    // -------------------- LEDs OFF --------------------

    else if (currentReadChar == redLEDOffCommand) {
      setLED(false, 0);
    }
    else if (currentReadChar == greenLEDOffCommand) {
      setLED(false, 1);
    }
    else if (currentReadChar == blueLEDOffCommand) {
      setLED(false, 2);
    }

    // -------------------- LEDs Flash ON --------------------

    if (currentReadChar == redLEDFlashOnCommand) {
      rgbLEDFlashAciveStatus[0] = true;
      rgbLEDFlashTimes[0] = currentMillis;
    }
    else if (currentReadChar == greenLEDFlashOnCommand) {
      rgbLEDFlashAciveStatus[1] = true;
      rgbLEDFlashTimes[1] = currentMillis;
    }
    else if (currentReadChar == blueLEDFlashOnCommand) {
      rgbLEDFlashAciveStatus[2] = true;
      rgbLEDFlashTimes[2] = currentMillis;
    }

    // -------------------- LEDs Flash OFF --------------------

    if (currentReadChar == redLEDFlashOffCommand) {
      rgbLEDFlashAciveStatus[0] = false;
    }
    else if (currentReadChar == greenLEDFlashOffCommand) {
      rgbLEDFlashAciveStatus[1] = false;
    }
    else if (currentReadChar == blueLEDFlashOffCommand) {
      rgbLEDFlashAciveStatus[2] = false;
    }

    // -------------------- MOTOR --------------------

    else if (currentReadChar == motorShortCommand) {
      pulseMotor(MOTOR_SHORT_PULSE);
    }
    else if (currentReadChar == motorMediumCommand) {
      pulseMotor(MOTOR_MEDIUM_PULSE);
    }
    else if (currentReadChar == motorLongCommand) {
      pulseMotor(MOTOR_LONG_PULSE);
    }

    // -------------------- ALARM --------------------

    else if (currentReadChar == alarmOnCommand) {
      alarmIsActive = true;
    }
    else if (currentReadChar == alarmOffCommand) {
      alarmIsActive = false;
    }
    else if (currentReadChar == alarmSuppressCommand) {
      alarmIsISuppressedState = true;
      alarmSuppressStartTime = currentMillis;
    }

    // -------------------- ACKNOLEGDEMENT --------------------

    else if (currentReadChar == ackCommand) {
      Serial.println(F("ACK!"));
      didReceiveAck = true;
      lastCommandSend = -1;
    }

  }

}

void sendBluetoothData() {

  if (!bluetoothConnected) {
    return;
  }

  if ((!switchIsBeingSwitched && prev_switchStatus != switchStatus)  || (!didReceiveAck && lastCommandSend < 3)) {

    if (!switchStatus || (!didReceiveAck && lastCommandSend == 0)) {
      ble.println("" switchPosition1Command);
      Serial.println("" switchPosition1Command);
      didReceiveAck = false;
      lastCommandSend = 0;
    }
    else if (switchStatus == 1 || (!didReceiveAck && lastCommandSend == 1)) {
      ble.println(""switchPosition2Command);
      Serial.println("" switchPosition2Command);
      didReceiveAck = false;
      lastCommandSend = 1;
    }
    else if (switchStatus == 2 || (!didReceiveAck && lastCommandSend == 2)) {
      ble.println("" switchPosition3Command);
      Serial.println("" switchPosition3Command);
      didReceiveAck = false;
      lastCommandSend = 2;
    }

  }

  if (buttonIsPressed != prev_buttonIsPressed || (!didReceiveAck && lastCommandSend >= 3)) {
    if (buttonIsPressed || (!didReceiveAck &&  lastCommandSend == 3)) {
      ble.println("" buttonPressedCommand);
      Serial.println("" buttonPressedCommand);
      didReceiveAck = false;
      lastCommandSend = 3;
    }
    else if (!buttonIsPressed || (!didReceiveAck &&  lastCommandSend == 4)) {
      ble.println("" buttonUnpressedCommand);
      Serial.println("" buttonUnpressedCommand);
      didReceiveAck = false;
      lastCommandSend = 4;
    }
  }

}

/*
void attemptBluetoothConnection() {

  if (!bluetoothConnected) {
    unsigned long currentMillis = millis();

    if (currentMillis - prev_bluetoothConnectionAttemptTime > bluetoothConnectionAttemptInterval) {
      prev_bluetoothConnectionAttemptTime = currentMillis;

      sendLEDWave();

      ble.isConnected();

    }
  }

}
*/

void initilizeBluetooth() {

  /* Initialise the module */
  sendLEDWave(true, STARTUP_LED_SCAN_SPEED);
  //Serial.print(F("Initialising the Bluetooth LE module: "));
  if ( !ble.begin(VERBOSE_MODE) ) {
    initilizeBluetooth();
    //error(F("Couldn't find module!"));
  }
  //Serial.println( F("OK!") );

  ble.echo(false);     // Disable command echo from Bluetooth module
  ble.verbose(false);  // Disable debug info from Bluetooth module

  // Change Mode LED Activity
  //Serial.println(F("Change LED activity to MODE"));
  ble.sendCommandCheckOK("AT+HWModeLED=MODE");
  ble.sendCommandCheckOK("AT+GAPDEVNAME=Proximity Band");

  // Set module to DATA mode
  //Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  setupWaitingConnectionLEDFlasing();


}
