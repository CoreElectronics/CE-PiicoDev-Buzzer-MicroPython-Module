from PiicoDev_Unified import *
import struct
_baseAddr=0x08
_DevID=0x51
_regDevID=0x00
_regFirmVer=0x02
_regI2cAddr=0x04
_regTone=0x05
_regVolume=0x06
_regLED=0x07

def hsv_to_rgb(h, s=1, v=1):
    if s == 0.0: v*=255; return (v, v, v)
    i = int(h*6.) # assume int() truncates
    f = (h*6.)-i; p,q,t = int(255*(v*(1.-s))), int(255*(v*(1.-s*f))), int(255*(v*(1.-s*(1.-f)))); v*=255; i%=6
    if i == 0: return [v, t, p]
    if i == 1: return [q, v, p]
    if i == 2: return [p, v, t]
    if i == 3: return [p, q, v]
    if i == 4: return [t, p, v]
    if i == 5: return [v, p, q]

class PiicoDev_Buzzer(object):
#     def setPixel(self,n,r,g,b):
#         self.led[n]=[round(r),round(g),round(b)]
# 
#     def show(self):
#         buffer = bytes(self.led[0]) + bytes(self.led[1]) + bytes(self.led[2])
#         self.i2c.writeto_mem(self.addr, _regLedVals, buffer)
# 
#     def setBrightness(self,x):
#         self.bright= x if 0 <= x <= 255 else 255
#         self.i2c.writeto_mem(self.addr, _regBright, bytes([self.bright]))
#         sleep_ms(1)
# 
#     def clear(self):
#         d=self.i2c.readfrom_mem(self.addr, _regCtrl, 1)
#         r=int.from_bytes(d,'big')
#         sleep_ms(1)
#         r|=(1<<0) # zero-th bit is the clear flag
#         self.i2c.writeto_mem(self.addr,_regCtrl,bytes([r]))
#         sleep_ms(1)







    def tone(self, freq, dur=0):
        f = freq.to_bytes(2,'big')
        d = dur.to_bytes(2,'big')
        print("f {}   {}".format(freq, f))
        try:
            self.i2c.writeto_mem(self.addr, _regTone, f+d)
        except:
            print("couldn't write")












    def setI2Caddr(self, newAddr):
        x=int(newAddr)
        assert 8 <= x <= 0x77, 'address must be >=0x08 and <=0x77'
        self.i2c.writeto_mem(self.addr, _regI2cAddr, bytes([x]))
        self.addr = x
        sleep_ms(5)

    def readFirmware(self):
        v=self.i2c.readfrom_mem(self.addr, _regFirmVer, 2)
        return (v[1],v[0])

    def readID(self):
        return self.i2c.readfrom_mem(self.addr, _regDevID, 1)[0]

    def pwrLED(self, x):
        self.i2c.writeto_mem(self.addr, _regLED, bytes([x]))
        
    def __init__(self, bus=None, freq=None, sda=None, scl=None, addr=_baseAddr, volume=3):
        self.i2c = create_unified_i2c(bus=bus, freq=freq, sda=sda, scl=scl)
        print("pass")
        a=addr
        if type(a) is list: # to accept DIP switch-positions eg [0,0,0,1]
            self.addr=_baseAddr+a[0]+2*a[1]+4*a[2]+8*a[3]
        else:
            self.addr = a # accept an integer
#         try:
#             if self.readID() != _DevID:
#                 print("* Incorrect device found at address {}".format(addr))
#             self.setBrightness(bright)
#             self.show()
#         except:
#             print("* Couldn't find a device - check switches and wiring")
#         
        