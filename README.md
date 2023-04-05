# PiicoDev® Buzzer MicroPython Module and Firmware

<!-- TODO update link URL with CE SKU -->
This is the firmware repo for the [Core Electronics PiicoDev® Buzzer Module](https://core-electronics.com.au/catalog/product/view/sku/CE08219)


This module depends on the [PiicoDev Unified Library](https://github.com/CoreElectronics/CE-PiicoDev-Unified), include `PiicoDev_Unified.py` in the project directory on your MicroPython device.


## Details
### PiicoDev_Buzzer(bus=, freq=, sda=, scl=, addr=, id=, volume=)

Parameter | Type | Range | Default | Description
--- | --- | --- | --- | ---
bus | int | 0,1 | Raspberry Pi Pico: 0, Raspberry Pi: 1 | I2C Bus.  Ignored on Micro:bit
freq | int | 100-1000000 | Device dependent | I2C Bus frequency (Hz).  Ignored on Raspberry Pi
sda | Pin | Device Dependent | Device Dependent | I2C SDA Pin. Implemented on Raspberry Pi Pico only
scl | Pin | Device Dependent | Device Dependent | I2C SCL Pin. Implemented on Raspberry Pi Pico only
addr | int | 0x08 <= addr <= 0x77 | 0x5C | Manually specify the address of the connected device. For when a sofware address is set on the device
id | List: [id1,id2,id3,id4] | (1=Closed, 0=Open). | [0,0,0,0] | Hardware switches change the device address - Abstracts the need for user to look up an address, simply input the switch positions. Alternatively, use `addr` for explicit address.
volume | int | 0:2 | 2 | Set the buzzer volume 0=quietest -> 2=loudest


### PiicoDev_Buzzer.tone(frequency, [duration])
Play a tone at `frequency` Hz for `duration` ms

Parameter | Type | Default | Description
--- | --- | --- | ---
frequency | int |   | frequency of the tone in Hertz.
duration | int |   | duration of the tone in milliseconds. Tone is continuous if omitted.

### PiicoDev_Buzzer.noTone()
Stop the buzzer


### PiicoDev_Buzzer.volume(volume)
**Deprecated in v20 hardware**
Set the buzzer volume 0=quietest -> 2=loudest

Parameter | Type | Description
--- | --- | ---  
volume | int  | Set the buzzer volume 0=quietest -> 2=loudest


### PiicoDev_Buzzer.pwrLED(state)
Control the onboard LED. On powerup, the LED defaults to `ON`

Parameter | Type | Description
--- | --- | ---  
state | Boolean  | Set the LED state. True=On, False=Off


# License
This project is open source - please review the LICENSE.md file for further licensing information.

If you have any technical questions, or concerns about licensing, please contact technical support on the [Core Electronics forums](https://forum.core-electronics.com.au/).

# Attribution
C++ code firmware project is based off the [Qwiic Switch](https://github.com/sparkfunX/Qwiic_Switch) project by Sparkfun. It makes use of the register structure and i2c device drivers.

*\"PiicoDev\" and the PiicoDev logo are trademarks of Core Electronics Pty Ltd.*
