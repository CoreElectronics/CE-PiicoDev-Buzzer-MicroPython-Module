/*
  User accessible functions
*/

// ToDo
void setTone(char *data) {
  frequency = (uint8_t(data[0]) << 8) + uint8_t(data[1]);
  duration = (uint8_t(data[2]) << 8) + uint8_t(data[3]);
  updateFlag = true;
}

void playTone(void) {
  if (duration == 0){
    tone(buzzerPin, frequency);
  } else {
    tone(buzzerPin, frequency, duration);
  }
}

// ToDo
void setVolume(char *data) {
  return;
}



void idReturn(char *data) {
  // responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.id);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command success
}

void statusReturn(char *data) {
  // responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.status);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command successful. Set status bit.
}

void firmwareMajorReturn(char *data) {
  // responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.firmwareMajor);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command success
}

void firmwareMinorReturn(char *data) {
  // responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.firmwareMinor);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command success
}

// Control the power LED - open drain output so toggle between enable (output, low) and disable (high-impedance input)
void setPowerLed(char *data) {
  powerLed( (data[0] == 1) );
}
void powerLed(bool state) {
  if (state) {
    pinMode(powerLedPin, OUTPUT);
    digitalWrite(powerLedPin, HIGH);
  } else {
    pinMode(powerLedPin, INPUT);
  }
}

void setAddress(char *data) {
  return;
  // byte tempAddress = data[0];
  //
  // if (tempAddress < 0x08 || tempAddress > 0x77)
  //   return; //Command failed. This address is out of bounds.
  // valueMap.i2cAddress = tempAddress;
  //
  // //Our I2C address may have changed because of user's command
  // // startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
  // EEPROM.put(LOCATION_ADDRESS_TYPE, SOFTWARE_ADDRESS);
  // startI2C();
  // recordSystemSettings();
  // valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS);
}













//Loads a long into the start of the responseBuffer
void loadArray(unsigned long myNumber)
{
  for (byte x = 0 ; x < sizeof(myNumber) ; x++)
    responseBuffer[x] = (myNumber >> (((sizeof(myNumber) - 1) - x) * 8)) & 0xFF;
  responseSize = sizeof(myNumber);
}

void loadArray(long myNumber)
{
  for (byte x = 0 ; x < sizeof(myNumber) ; x++)
    responseBuffer[x] = (myNumber >> (((sizeof(myNumber) - 1) - x) * 8)) & 0xFF;
  responseSize = sizeof(myNumber);
}

//Loads an int into the start of the responseBuffer
void loadArray(int myNumber)
{
  for (byte x = 0 ; x < sizeof(myNumber) ; x++)
    responseBuffer[x] = (myNumber >> (((sizeof(myNumber) - 1) - x) * 8)) & 0xFF;
  responseSize = sizeof(myNumber);
}

void loadArray(unsigned int myNumber)
{
  for (byte x = 0 ; x < sizeof(myNumber) ; x++)
    responseBuffer[x] = (myNumber >> (((sizeof(myNumber) - 1) - x) * 8)) & 0xFF;
  responseSize = sizeof(myNumber);
}

//Loads an byte into the start of the responseBuffer
void loadArray(byte myNumber)
{
  for (byte x = 0 ; x < sizeof(myNumber) ; x++)
    responseBuffer[x] = (myNumber >> (((sizeof(myNumber) - 1) - x) * 8)) & 0xFF;
  responseSize = sizeof(myNumber);
}

//Loads a bool into the start of the responseBuffer
void loadArray(boolean myStatus)
{
  responseBuffer[0] = myStatus;
  responseSize = sizeof(myStatus);
}
