# PiicoDev Buzzer example. Set a software address
# requires all ID switches to be OFF

from PiicoDev_Buzzer import PiicoDev_Buzzer
from PiicoDev_Unified import sleep_ms

buzz = PiicoDev_Buzzer()
buzz.setI2Caddr(0x64)

buzz.tone(800, 500)
sleep_ms(500)
