_B='big'
_A=None
from PiicoDev_Unified import *
_baseAddr=92
_DevID=81
_regDevID=17
_regStatus=1
_regFirmMaj=2
_regFirmMin=3
_regI2cAddr=4
_regTone=5
_regVolume=6
_regLED=7
class PiicoDev_Buzzer:
	def tone(self,freq,dur=0):
		_f=int(freq);_d=int(dur);f=_f.to_bytes(2,_B);d=_d.to_bytes(2,_B)
		try:self.i2c.writeto_mem(self.addr,_regTone,f+d);return 0
		except:print(i2c_err_str.format(self.addr));return 1
	def noTone(self):r=self.tone(0);return r
	def volume(self,vol):
		_v=int(vol);assert _v>=0 and _v<=2,'volume must be 0, 1 or 2';v=vol.to_bytes(1,_B)
		try:self.i2c.writeto_mem(self.addr,_regVolume,v);sleep_ms(5);return 0
		except:print(i2c_err_str.format(self.addr));return 1
	def setI2Caddr(self,newAddr):
		x=int(newAddr);assert 8<=x<=119,'address must be >=0x08 and <=0x77'
		try:self.i2c.writeto_mem(self.addr,_regI2cAddr,bytes([x]));self.addr=x;sleep_ms(5);return 0
		except:print(i2c_err_str.format(self.addr));return 1
	def readFirmware(self):
		v=[0,0]
		try:v[1]=self.i2c.readfrom_mem(self.addr,_regFirmMaj,1);v[0]=self.i2c.readfrom_mem(self.addr,_regFirmMin,1);return v[1],v[0]
		except:return 0,0
	def readStatus(self):sts=self.i2c.readfrom_mem(self.addr,_regStatus,1);return sts
	def readID(self):x=self.i2c.readfrom_mem(self.addr,_regDevID,1);return int.from_bytes(x,_B)
	def pwrLED(self,x):
		try:self.i2c.writeto_mem(self.addr,_regLED,bytes([x]));return 0
		except:print(i2c_err_str.format(self.addr));return 1
	def __init__(self,bus=_A,freq=_A,sda=_A,scl=_A,addr=_baseAddr,id=_A,volume=2):
		self.i2c=create_unified_i2c(bus=bus,freq=freq,sda=sda,scl=scl);a=addr
		if type(id)is list and not all((v==0 for v in id)):assert max(id)<=1 and min(id)>=0 and len(id)is 4,'id must be a list of 1/0, length=4';self.addr=8+id[0]+2*id[1]+4*id[2]+8*id[3]
		else:self.addr=addr
		try:self.i2c.writeto_mem(self.addr,_regLED,b'\x01')
		except Exception as e:print(i2c_err_str.format(self.addr));raise e
		self.volume(volume)