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
    
	def TESTAND(self, other):
		output = BZET()
		lib.BinaryBzet_TEST_AND(self.obj, other.obj, output.obj)
		return output

	def __or__    (self,other): return self.OR(other)
	def __and__   (self,other): return self.AND(other)
	def __xor__   (self,other): return self.XOR(other)
	def __invert__(self):       return self.NotA(self)
	def __ior__   (self,other): return self.OR(other)
	def __iand__  (self,other): return self.AND(other)
	def __ixor__  (self,other): return self.XOR(other)
	
	def FALSE(self, other):
		output = BZET()
		lib.BinaryBzet_FALSE(self.obj, other.obj, output.obj)
		return output

	def AND(self, other):
		output = BZET()
		lib.BinaryBzet_AND(self.obj, other.obj, output.obj)
		return output

	def NonImplication(self, other):
		output = BZET()
		lib.BinaryBzet_NonImplication(self.obj, other.obj, output.obj)
		return output

	def A(self, other):
		output = BZET()
		lib.BinaryBzet_A(self.obj, other.obj, output.obj)
		return output

	def ConverseNonImplication(self, other):
		output = BZET()
		lib.BinaryBzet_ConverseNonImplication(self.obj, other.obj, output.obj)
		return output

	def B(self, other):
		output = BZET()
		lib.BinaryBzet_B(self.obj, other.obj, output.obj)
		return output

	def XOR(self, other):
		output = BZET()
		lib.BinaryBzet_XOR(self.obj, other.obj, output.obj)
		return output

	def OR(self, other):
		output = BZET()
		lib.BinaryBzet_OR(self.obj, other.obj, output.obj)
		return output

	def NOR(self, other):
		output = BZET()
		lib.BinaryBzet_NOR(self.obj, other.obj, output.obj)
		return output

	def EQ(self, other):
		output = BZET()
		lib.BinaryBzet_EQ(self.obj, other.obj, output.obj)
		return output

	def NotB(self, other):
		output = BZET()
		lib.BinaryBzet_NotB(self.obj, other.obj, output.obj)
		return output

	def ConverseImplication(self, other):
		output = BZET()
		lib.BinaryBzet_ConverseImplication(self.obj, other.obj, output.obj)
		return output

	def NotA(self, other):
		output = BZET()
		lib.BinaryBzet_NotA(self.obj, other.obj, output.obj)
		return output

	def Implication(self, other):
		output = BZET()
		lib.BinaryBzet_Implication(self.obj, other.obj, output.obj)
		return output

	def NAND(self, other):
		output = BZET()
		lib.BinaryBzet_NAND(self.obj, other.obj, output.obj)
		return output

	def TRUE(self, other):
		output = BZET()
		lib.BinaryBzet_TRUE(self.obj, other.obj, output.obj)
		return output

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
		output = BZET()
		lib.BinaryBzet_slice(self.obj, c_uint(startIndex), c_uint(endIndex), output.obj)
		return output
	
	#def LIST_T(self):
	#	output = lib.BinaryBzet_bitList(self.obj)
	#	print type(output)
	#	return output
	#LIST_T.restype = py_object
	

x = BZET("00001111")
y = x.LIST_T()
print y
strA = x.getBzetString()
strB = x.getBzetPretty()
print strA
print strB
y = x.FALSE(x)
print "41"
y = x.AND(x)
print "31"
y = x.NonImplication(x)
print "1"
y = x.A(x)
print "2"
y = x.ConverseNonImplication(x)
print "7"
y = x.B(x)
print "6"
y = x.XOR(x)
print "5"
y = x.OR(x)
print "4"
y = x.NOR(x)
print "81"
y = x.EQ(x)
print "8"
y = x.NotB(x)
print "19"
y = x.ConverseImplication(x)
print "7771"
y = x.NotA(x)
print "771"
y = x.Implication(x)
print "17"
y = x.NAND(x)
print "177577"
y = x.TRUE(x)
print "1773774"
a = BZET("00001110#");
b = BZET("00001111#");
c = BZET("00001111#");
d = BZET("00111000#");
print a.equals(b)
print b.equals(c)
print d.slice(2,6).getBzetString();
