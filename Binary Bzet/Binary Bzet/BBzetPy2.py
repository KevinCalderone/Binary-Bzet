from ctypes import *
import ctypes
lib = cdll.LoadLibrary('Python2DLL.dll')

class BITR(object):
	def __init__(self):
		self.obj = lib.bitR_new()

	def bitR_add(self, start, end=0, step=0):
		return lib.bitR_add(self.obj, c_int(start), c_int(end), c_int(step))
	
	def bitR_at(self, index):
		return lib.bitR_at(self.obj,c_int(index))
	
	def bitR_size(self):
		return lib.bitR_size(self.obj)

	bitR_at.restype = bool
	bitR_size.restype = int

class BZET(object):
	def __init__(self,input=None):
		if input == None:
			self.obj = lib.BinaryBzet_new()
		if type(input)== type(""):
			self.obj = lib.BinaryBzet_new_string(c_char_p(input))
		else:
			self.obj = lib.BinaryBzet_new()
	
	def LEV(self):
		return lib.BinaryBzet_getDepth(self.obj)
	LEV.restype = int

	def size(self):
		return lib.BinaryBzet_size(self.obj)
	size.restype = int

	def getBzetPretty(self):
		return lib.BinaryBzet_getBzetPretty(self.obj)
	getBzetPretty.restype = str

	def getBzetString(self):
		return lib.BinaryBzet_getBzetString(self.obj)
	getBzetString.restype = str

	def getCharFromBzet(self, ind):
		return lib.BinaryBzet_getCharFromBzet(self.obj, c_int(ind))
	getCharFromBzet.restype = str
