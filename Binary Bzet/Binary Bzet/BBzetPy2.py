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
			input+=str('#')
			self.obj = lib.BinaryBzet_new_string(c_char_p(input))
		else:
			self.obj = lib.BinaryBzet_new()

	#Miscellaneous Bzet Property Functions
	def LEV(self):
		return lib.BinaryBzet_getDepth(self.obj)
	LEV.restype = int

	def size(self):
		return lib.BinaryBzet_size(self.obj)
	size.restype = int

	def set(self, index):
		lib.BinaryBzet_set(self.obj, c_uint(index))
	
	def unset(self, index):
		lib.BinaryBzet_unset(self.obj, c_uint(index))

	def flip(self, index):
		lib.BinaryBzet_flip(self.obj, c_uint(index))

	def clean(self):
		lib.BinaryBzet_clean(self.obj)

	def countBits(self):
		return lib.BinaryBzet_countBits(self.obj)
	countBits.restype = c_uint

	def getFirstBit(self):
		return lib.BinaryBzet_getFirstBit(self.obj)
	getFirstBit.restype = c_uint

	def getLastBit(self):
		return lib.BinaryBzet_getLastBit(self.obj)
	getLastBit.restype = c_uint

	#vector<uint> bitList ();


	#Print Functions
	def getBzetPretty(self):
		output_ptr = c_char_p('rawr')
		lib.BinaryBzet_getBzetPretty(self.obj,output_ptr)
		return output_ptr.value
	getBzetPretty.restype = str

	def getBzetString(self):
		output_ptr = c_char_p('rawr')
		lib.BinaryBzet_getBzetString(self.obj,output_ptr)
		return output_ptr.value
	getBzetString.restype = str

	#Comparison Operators
	def compare(self, other):
		return lib.BinaryBzet_compare(self.obj, other.obj)
	compare.restype = int
 
	def equals(self, other):
		return lib.BinaryBzet_equals(self.obj, other.obj)
	equals.restype = c_bool

	def AlignCompare(self, other):
		return lib.BinaryBzet_AlignCompare(self.obj, other.obj)
	AlignCompare.restype = c_bool

	#Shifing Operators
	def leftShift(self, distance):
		lib.BinaryBzet_leftShift(self.obj, c_uint(distance))

	def rightShift(self, distance):
		lib.BinaryBzet_rigtShift(self.obj, c_uint(distance))

	#Slice/Substring
	def slice(self, startIndex, endIndex):
		output = lib.BinaryBzet_new()
		lib.BinaryBzet_slice(self.obj, c_uint(startIndex), c_uint(endIndex), output.obj)
		return output
	#slice.restype = type(self)

a = BZET("00001110#");
b = BZET("00001111#");
c = BZET("00001111#");
d = BZET("00111000#");
print a.equals(b)
print b.equals(c)
print type(a)