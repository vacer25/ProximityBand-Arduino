// -------------------- LIB'S --------------------

#include <EEPROM.h> // Required to use EEPROM
#include "EEPROMAnything.h" // Easy EEPROM read & write
#include <SPI.h>
#include <SoftwareSerial.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

// -------------------- BLUETOOTH --------------------

// Create the software serial bluefruit object
SoftwareSerial bluetoothSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
Adafruit_BluefruitLE_UART ble(bluetoothSS, BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

// -------------------- BLUETOOTH CONNECTION --------------------

unsigned long lastBluetoothDataInTime = 0;
boolean bluetoothConnected = false;
//boolean prev_bluetoothConnected = false;

// -------------------- EEPROM --------------------

static const byte settings_version = B00000010;

// =================== Saved EEPROM Data Map ===================
// Bytes    Size    Purpose
// ····························································
// 0        1       Settings Version
// 1        1       Saved currentLEDBrightness

// -------------------- TIMINGS --------------------

#define BLUETOOTH_CONNECTION_TIMEOUT 1000

#define STARTUP_DELAY_TIME 1000

#define SWITCH_DEBOUNCE_TIME 500

#define MOTOR_SHORT_PULSE 50
#define MOTOR_MEDIUM_PULSE 200
#define MOTOR_LONG_PULSE 500

#define STARTUP_LED_SCAN_SPEED 500
#define WAITING_CONNECTION_LED_SCAN_INTERVAL 1000

#define LED_FLASH_ON_INTERVAL 100
#define LED_FLASH_OFF_INTERVAL 1000

#define ALARM_ON_INTERVAL 500
#define ALARM_OFF_INTERVAL 250

// -------------------- ALARM --------------------

boolean alarmIsActive = false;
boolean prev_alarmIsActive = false;
boolean alarmIsInHighState = false;

unsigned long prev_alarmSwitchTime = 0;
unsigned long prev_alarmRepeatTime = 0;

// -------------------- PINS --------------------

#define buttonPin      7    // The number of the pushbutton pin

#define switchPos1Pin  10   // The number of the switch position 1 pin
#define switchPos2Pin  8    // The number of the switch position 2 pin

#define motorPin       9    // The number of the motor pin
#define statusLedPin  13    // The number of the status pin

byte rgbLEDPins[3] = {6, 5, 3}; // The number of the red, green, blue LED pins
float rgbLEDBrightnessAdjustments[3] = {1, 1, 0.5};

// -------------------- BUTTON --------------------

boolean buttonIsPressed = 0;
boolean prev_buttonIsPressed = 0;

// -------------------- SWITCH --------------------

byte switchStatus = 0;
byte temp_switchStatus = 0;
byte prev_switchStatus = 0;

unsigned long switchInteractionTime = 0;
boolean switchIsBeingSwitched = false;

// -------------------- MOTOR --------------------

boolean motorIsOn = 0;
boolean prev_motorIsOn = 0;
unsigned long motorOffTime = 0;

// -------------------- LEDs --------------------

#define minLEDLEDBrightness 16
#define mediumLEDLEDBrightness 64
#define maxLEDLEDBrightness 255
byte currentLEDBrightness = mediumLEDLEDBrightness;

boolean rgbLEDStatus[] = {false, false, false};
boolean prev_rgbLEDStatus[] = {false, false, false};

//#define SINGLE_LED_FLASH_MODE

boolean rgbLEDFlashAciveStatus[] = {false, false, false};
boolean prev_rgbLEDFlashAciveStatus[] = {false, false, false};
unsigned long rgbLEDFlashTimes[] = {0, 0, 0};
unsigned long prev_rgbLEDFlashTime = 0;

// -------------------- COMMANDS --------------------

boolean didReceiveAck = false;
int lastCommandSend = -1;

#define switchPosition1Command  "S1"
#define switchPosition2Command  "S2"
#define switchPosition3Command  "S3"

#define buttonPressedCommand    "B1"
#define buttonUnpressedCommand  "B0"

#define redLEDOnCommand         'R'
#define greenLEDOnCommand       'G'
#define blueLEDOnCommand        'B'
#define redLEDOffCommand        'r'
#define greenLEDOffCommand      'g'
#define blueLEDOffCommand       'b'

#define redLEDFlashOnCommand    'I'
#define greenLEDFlashOnCommand  'J'
#define blueLEDFlashOnCommand   'K'
#define redLEDFlashOffCommand   'i'
#define greenLEDFlashOffCommand 'j'
#define blueLEDFlashOffCommand  'k'

#define motorShortCommand       '1'
#define motorMediumCommand      '2'
#define motorLongCommand        '3'

#define alarmOnCommand          'X'
#define alarmOffCommand         'x'

#define ackCommand              'A'

void setup(void) {

  delay(STARTUP_DELAY_TIME);

  // -------------------- PINS --------------------

  pinMode(buttonPin, INPUT);

  pinMode(switchPos1Pin, INPUT);
  pinMode(switchPos2Pin, INPUT);

  pinMode(motorPin, OUTPUT);
  pinMode(statusLedPin, OUTPUT);

  pinMode(rgbLEDPins[0], OUTPUT);
  pinMode(rgbLEDPins[1], OUTPUT);
  pinMode(rgbLEDPins[2], OUTPUT);

  // -------------------- BLUETOOTH ------------------

  Serial.begin(115200);
  initilizeBluetooth();

  // -------------------- EEPROM --------------------

  loadEEPROMData();

}

void loop(void) {

  getBluetoothData();
  processUserInteraction();
  sendBluetoothData();

  checkAlarm();
  updateRgbLEDFlashing();

  updateMotorStatus();
  updateStatus();

}

// -------------------- HELPER ------------------

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void updateStatus() {

  if (bluetoothConnected && millis() - lastBluetoothDataInTime > BLUETOOTH_CONNECTION_TIMEOUT) {
    stopWaitingConnectionLEDFlasing();
    bluetoothConnected = false;
    alarmIsActive = true;
  }

  prev_switchStatus = switchStatus;
  prev_buttonIsPressed =  buttonIsPressed;

  prev_motorIsOn = motorIsOn;
  for (int index = 0; index < 3; index++) {
    prev_rgbLEDStatus[index] = rgbLEDStatus[index];
    prev_rgbLEDFlashAciveStatus[index] = rgbLEDFlashAciveStatus[index];
  }

  //prev_bluetoothConnected = bluetoothConnected;
  prev_alarmIsActive = alarmIsActive;

}
