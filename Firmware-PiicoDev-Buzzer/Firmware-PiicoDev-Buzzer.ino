/*
 * PiicoDev Buzzer Module Firmware
 * Written by Michael Ruppe @ Core Electronics
 * Based off the Qwiic Button Project https://github.com/sparkfun/Qwiic_Button
 * Date: FEB 2023
 * An I2C based buzzer module that allows tone and timing control
 *
 * Feel like supporting PiicoDev? Buy a module here: https://core-electronics.com.au/catalog/product/view/sku/CE07910
 *
 * Changes in v2.0
 *  The new v20 hardware has a louder buzzer and does not support volume control. Volume funcitons are still provisioned here but do nothing.
 *  Pinout has changed significantly to facilitate routing the v20 hardware.
 *  There is also a self-test utility that checks address pins are not shorted together during PCBA
 *
 * Changes in v1.1
 *  Default address is now not shared with other smart modules
 *  hardware address switches select from the same shared pool of addresses for smart modules
 */

#define DEBUG 0

#include <Wire.h>
#include <EEPROM.h>
#include <stdint.h>
#include <avr/sleep.h> // For sleep_mode
#include <avr/power.h> // For powering-down peripherals such as ADC and Timers

#define FIRMWARE_MAJOR 0x02
#define FIRMWARE_MINOR 0x00

#define DEVICE_ID 0x51
#define DEFAULT_I2C_ADDRESS 0x5C    // The default address when all switches are off
#define I2C_ADDRESS_POOL_START 0x08 // The start of the 'smart module address pool' - addresses settable by switches
#define SOFTWARE_ADDRESS true
#define HARDWARE_ADDRESS false
#define I2C_BUFFER_SIZE 32 //For ATmega328 based Arduinos, the I2C buffer is limited to 32 bytes

enum eepromLocations {
  LOCATION_I2C_ADDRESS = 0x00,  // Device's address
  LOCATION_ADDRESS_TYPE = 0x01, // Address type can be either hardware defined (jumpers/switches), or software defined by user.
};

uint8_t oldAddress;

// Hardware Connectins
#if defined(__AVR_ATtiny806__) || defined(__AVR_ATtiny816__) || defined(__AVR_ATtiny1606__) || defined(__AVR_ATtiny1616__) || defined(__AVR_ATtiny3217__)
const uint8_t powerLedPin = PIN_PA3;
uint16_t buzzerPin = PIN_PA2;

const uint8_t addressPin1 = PIN_PA6;
const uint8_t addressPin2 = PIN_PA7;
const uint8_t addressPin3 = PIN_PB5;
const uint8_t addressPin4 = PIN_PB4;
#endif

// Prototyping with Arduino Uno
#if defined(__AVR_ATmega328P__)
  const uint8_t powerLedPin = 3;
  const int buzzerPin = 9;

  const int addressPin1 = 8;
  const int addressPin2 = 7;
  const int addressPin3 = 6;
  const int addressPin4 = 5;
#endif

// System global variables
volatile bool updateFlag = true; // Goes true when new data received. Cause LEDs to update
volatile unsigned long lastSyncTime = 0;

// volatile uint16_t frequency = 0;
// volatile uint16_t duration = 0;

#define LOCAL_BUFFER_SIZE 20 // bytes
byte incomingData[LOCAL_BUFFER_SIZE]; //Local buffer to record I2C bytes before committing to file, add 1 for 0 character on end
volatile int incomingDataSpot = 0; //Keeps track of where we are in the incoming buffer

//These are the different types of data the device can respond with
enum Response {
  RESPONSE_STATUS, //1 byte containing status bits
  RESPONSE_VALUE, //Value byte containing measurements etc.
};

volatile Response responseType = RESPONSE_STATUS; //State engine that let's us know what the master is asking for
byte responseBuffer[I2C_BUFFER_SIZE]; //Used to pass data back to master
volatile byte responseSize = 1; //Defines how many bytes of relevant data is contained in the responseBuffer

#define STATUS_LAST_COMMAND_SUCCESS 1
#define STATUS_LAST_COMMAND_KNOWN 2

struct memoryMap {
  uint8_t id;
  uint8_t status;
  uint8_t firmwareMajor;
  uint8_t firmwareMinor;
  uint8_t i2cAddress;
  uint16_t tone;
  uint8_t volume;
  uint8_t led;
  uint8_t selfTestResult;
};

// Register addresses.
const memoryMap registerMap = {
  .id = 0x11,
  .status = 0x01,
  .firmwareMajor = 0x02,
  .firmwareMinor = 0x03,
  .i2cAddress = 0x04,
  .tone = 0x05,
  .volume = 0x06,
  .led = 0x07,
  .selfTestResult = 0x09
};

volatile memoryMap valueMap = {
  .id = DEVICE_ID,
  .status = 0x00,
  .firmwareMajor = FIRMWARE_MAJOR,
  .firmwareMinor = FIRMWARE_MINOR,
  .i2cAddress = DEFAULT_I2C_ADDRESS,
  .tone = 0x00,
  .volume = 0x00,
  .led = 0x01,
  .selfTestResult = 0x00
};

uint8_t currentRegisterNumber;

struct functionMap {
  byte registerNumber;
  void (*handleFunction)(char *myData);
};

void idReturn(char *data);
void statusReturn(char *data);
void firmwareMajorReturn(char *data);
void firmwareMinorReturn(char *data);
void setAddress(char *data);
void setTone(char *data);
void setVolume(char *data);
void setPowerLed(char *data);
void getSelfTest(char *data);

functionMap functions[] = {
  {registerMap.id, idReturn},
  {registerMap.status, statusReturn},
  {registerMap.firmwareMajor, firmwareMajorReturn},
  {registerMap.firmwareMinor, firmwareMinorReturn},
  {registerMap.i2cAddress, setAddress},
  {registerMap.tone, setTone},
  {registerMap.volume, setVolume},
  {registerMap.led, setPowerLed},
  {registerMap.selfTestResult, getSelfTest},
};



void setup() {

#if DEBUG
  Serial.begin(115200);
  Serial.println("Begin");
#endif

  // Pull up address pins
  pinMode(addressPin1, INPUT_PULLUP);
  pinMode(addressPin2, INPUT_PULLUP);
  pinMode(addressPin3, INPUT_PULLUP);
  pinMode(addressPin4, INPUT_PULLUP);
  pinMode(powerLedPin, OUTPUT);
  powerLed(true); // enable Power LED by default on every power-up

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  readSystemSettings(); //Load all system settings from EEPROM

  startI2C();          //Determine the I2C address we should be using and begin listening on I2C bus
  oldAddress = valueMap.i2cAddress;

}

void loop() {
  if (updateFlag) {
    startI2C(); // reinitialise I2C with new address, update EEPROM with custom address as necessary
    updateFlag = false;
  }
  sleep_mode();
}




//Begin listening on I2C bus as I2C slave using the global variable valueMap.i2cAddress
// ToDo don't use globals ie. pass in value map =>  void startI2C(memoryMap *map)
void startI2C()
{
  uint8_t address;
  uint8_t addressType;
  EEPROM.get(LOCATION_ADDRESS_TYPE, addressType);
  if (addressType == 0xFF) {
    EEPROM.put(LOCATION_ADDRESS_TYPE, SOFTWARE_ADDRESS);
  }

  // Add hardware address jumper values to the default address
  uint8_t IOaddress = DEFAULT_I2C_ADDRESS;
  uint8_t switchPositions = 0;
  bitWrite(switchPositions, 0, !digitalRead(addressPin1));
  bitWrite(switchPositions, 1, !digitalRead(addressPin2));
  bitWrite(switchPositions, 2, !digitalRead(addressPin3));
  bitWrite(switchPositions, 3, !digitalRead(addressPin4));
  if (switchPositions != 0) IOaddress = I2C_ADDRESS_POOL_START + switchPositions; // use the "smart-module address pool" when any hardware address is set

  //If any of the address jumpers are set, we use jumpers
  if ((IOaddress != DEFAULT_I2C_ADDRESS) || (addressType == HARDWARE_ADDRESS))
  {
    address = IOaddress;
    EEPROM.put(LOCATION_ADDRESS_TYPE, HARDWARE_ADDRESS);
  }
  //If none of the address jumpers are set, we use registerMap (but check to make sure that the value is legal first)
  else
  {
    //if the value is legal, then set it
    if (valueMap.i2cAddress > 0x07 && valueMap.i2cAddress < 0x78)
      address = valueMap.i2cAddress;

    //if the value is illegal, default to the default I2C address for our platform
    else
      address = DEFAULT_I2C_ADDRESS;
  }

  //save new address to the register map
  valueMap.i2cAddress = address;

  recordSystemSettings(); // save the new address to EEPROM

  //reconfigure Wire instance
  Wire.end();          //stop I2C on old address
  Wire.begin(address); //rejoin the I2C bus on new address

  //The connections to the interrupts are severed when a Wire.begin occurs, so here we reattach them
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
