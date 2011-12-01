from ctypes import *
import ctypes
import sys
from os.path import dirname
                                                # __file__ gets the full path of this pgm
dlldir = dirname(__file__)                      # this strips it to the directory only
dlldir.replace( '\\', '\\\\' )                  # Replaces \ with \\ in dlldir
lib = cdll.LoadLibrary(dlldir+'\\BBzetPy2.dll') # Loads from the full path to your module.

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
		elif type(input)== type(""):
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

	def COUNT(self):
		return lib.BinaryBzet_countBits(self.obj)
	COUNT.restype = c_uint

	def FIRST(self):
		return lib.BinaryBzet_getFirstBit(self.obj)
	FIRST.restype = c_uint

	def LAST(self):
		return lib.BinaryBzet_getLastBit(self.obj)
	LAST.restype = c_uint


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

	def __or__    (self,other): return self.OR(other)
	def __and__   (self,other): return self.AND(other)
	def __xor__   (self,other): return self.XOR(other)
	def __invert__(self):       return self.NotA(self)
	def __ior__   (self,other): return self.OR(other)
	def __iand__  (self,other): return self.AND(other)
	def __ixor__  (self,other): return self.XOR(other)

	def NOT(self):
		return ~self

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
		lib.BinaryBzet_rightShift(self.obj, c_uint(distance))

	#Slice/Substring
	def slice(self, startIndex, endIndex):
		output = BZET()
		lib.BinaryBzet_slice(self.obj, c_uint(startIndex), c_uint(endIndex), output.obj)
		return output
	
	def LIST_T(self):
		size = self.COUNT()
		list = (c_uint * int(size))()
		lib.BinaryBzet_bitList(self.obj, byref(list))
		output = []
		for i in range(0, size):
			output.append(list[i])
		return output

	def test(self, index):
		return lib.BinaryBzet_test(self.obj, c_uint(index))
        
	def __getitem__(self, key):
		return self.test(key)
        
	def __setitem__(self, key, value):
		if (value == 1):
			self.set(key)
		else:
			self.unset(key);
