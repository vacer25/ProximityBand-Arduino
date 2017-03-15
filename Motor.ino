void updateMotorStatus() {

  if (motorIsOn) {
    if (millis() >= motorOffTime || millis() < 1000) {
      digitalWrite(motorPin, LOW);
      motorIsOn = false;
    }
  }

}

void pulseMotor(int motorOnTimeToSet) {
  digitalWrite(motorPin, HIGH);
  motorOffTime = millis() + motorOnTimeToSet;
  motorIsOn = true;
}
