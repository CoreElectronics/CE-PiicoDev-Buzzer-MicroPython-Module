# PiicoDev Buzzer example. Play tones and change the volume.

from PiicoDev_Buzzer import PiicoDev_Buzzer
from PiicoDev_Unified import sleep_ms

buzz = PiicoDev_Buzzer(volume=2) # volume may be 0, 1 or 2 (loudest)

buzz.tone(800, 500) # high tone (800Hz for 500ms)
sleep_ms(500)

buzz.tone(400, 500) # low tone (400Hz for 500ms)
sleep_ms(1500)

buzz.volume(0) # low volume

buzz.tone(800) # high tone - continuous
sleep_ms(500)
buzz.noTone() # stop playing

buzz.tone(400) # low tone - continuous
sleep_ms(500)
buzz.noTone()