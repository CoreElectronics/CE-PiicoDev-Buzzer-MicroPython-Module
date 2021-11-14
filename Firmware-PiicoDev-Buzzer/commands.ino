/*
  User accessible functions
*/

// Macro for number of elements in an array
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))


void setTone(char *data) {
  uint16_t frequency = (uint8_t(data[0]) << 8) + uint8_t(data[1]);
  uint16_t duration = (uint8_t(data[2]) << 8) + uint8_t(data[3]);
  playTone(frequency, duration);
}

void playTone(uint16_t frequency, uint16_t duration) {
  if (frequency == 0){ // stop playing
    noTone(buzzerPin);
  }
  if (duration == 0){
    tone(buzzerPin, frequency);
  } else {
    tone(buzzerPin, frequency, duration);
  }
}

void setVolume(char *data) {
  uint8_t vol = data[0];
  if (vol >=0 && vol < COUNT_OF(buzzerPins)) volume(uint8_t(data[0]));
}

void volume(uint8_t vol) {
  // Reset all buzzer pins
  for (uint8_t i = 0; i < COUNT_OF(buzzerPins); i++) {
    pinMode(buzzerPins[i], INPUT);
  }
  // Select the desired pin to use
  buzzerPin = buzzerPins[vol];
  pinMode(buzzerPin, OUTPUT);
}


void idReturn(char *data) {
  responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.id);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command success
}

void statusReturn(char *data) {
  responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.status);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command successful. Set status bit.
}

void firmwareMajorReturn(char *data) {
  responseType = RESPONSE_VALUE;
  loadArray((byte)valueMap.firmwareMajor);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS); //Command success
}

void firmwareMinorReturn(char *data) {
  responseType = RESPONSE_VALUE;
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
  byte tempAddress = data[0];

  if (tempAddress < 0x08 || tempAddress > 0x77)
    return; //Command failed. This address is out of bounds.
  valueMap.i2cAddress = tempAddress;

  EEPROM.put(LOCATION_ADDRESS_TYPE, SOFTWARE_ADDRESS);
  valueMap.status |= (1 << STATUS_LAST_COMMAND_SUCCESS);
  updateFlag = true; // will trigger a I2C re-initalise and save custom address to EEPROM
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
