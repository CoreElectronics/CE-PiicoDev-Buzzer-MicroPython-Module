from PiicoDev_Buzzer import PiicoDev_Buzzer
from PiicoDev_Unified import sleep_ms

buzz = PiicoDev_Buzzer()


while True:
    buzz.tone(1000, 1000) # high tone
    buzz.pwrLED(False)
    sleep_ms(500)
    
    buzz.tone(500,1000) # low tone
    buzz.pwrLED(True)
    sleep_ms(500)

