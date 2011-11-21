#ifndef __BINARYBZET_H__
#define __BINARYBZET_H__

#include <stdint.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "bitR.h"

using namespace std;

typedef uint8_t u8; // unsigned 8 bit int
typedef size_t uint;
typedef unsigned char bitpair;

const uint c_uint_max = SIZE_MAX;

class BinaryBzet {
public:
	//Constructors
	BinaryBzet();
	//BinaryBzet(uint index);
	//BinaryBzet(uint indexi, uint indexe);
	BinaryBzet(uint indexi, uint indexe=0, uint step=0);
	BinaryBzet(string bitstring);
	BinaryBzet(vector<bool>* bzetvector, uint depth);
	~BinaryBzet();
	
	//Print Bzet
	string getBzetString();		//return "TTT11..." thingy
	vector<bool> getBzetBinaryString() { return m_bzet; }	// return binary Bzet string
	string getBzetPretty();	//get pretty formatted Bzet
	char getCharFromBzet(uint indexB);	//return a char from Bzet

	bool isTAWatching() { return true; } // lol

	BinaryBzet operator&  (const BinaryBzet& rhs);
	BinaryBzet operator|  (const BinaryBzet& rhs);
	BinaryBzet operator ^ (const BinaryBzet& rhs);
	bool	   operator== (const BinaryBzet& rhs);
	BinaryBzet operator ~();

	//Binary Boolean Operations
	BinaryBzet FALSE (const BinaryBzet& rhs);
	BinaryBzet AND (const BinaryBzet& rhs);
	BinaryBzet NonImplication (const BinaryBzet& rhs);
	BinaryBzet A(const BinaryBzet& rhs);
	BinaryBzet ConverseNonImplication(const BinaryBzet& rhs);
	BinaryBzet B(const BinaryBzet& rhs);
	BinaryBzet XOR(const BinaryBzet& rhs);
	BinaryBzet OR(const BinaryBzet& rhs);
	BinaryBzet NOR(const BinaryBzet& rhs);
	BinaryBzet EQ(const BinaryBzet& rhs);
	BinaryBzet NotB(const BinaryBzet& rhs);
	BinaryBzet ConverseImplication(const BinaryBzet& rhs);
	BinaryBzet NotA(const BinaryBzet& rhs);
	BinaryBzet Implication(const BinaryBzet& rhs);
	BinaryBzet NAND(const BinaryBzet& rhs);
	BinaryBzet TRUE(const BinaryBzet& rhs);

	//Comparison Operators
	int compare(BinaryBzet& bzet);

	//Shifing Operators
	void leftShift(uint distance);
	void rightShift(uint distance);

	//Slice/Substring
	BinaryBzet slice(uint startIndex, uint endIndex);

	//Miscellaneous Bzet Property Functions
	uint getDepth();
	uint size();
	bool test(uint index);
	void set(uint index);
	void unset(uint index);
    void flip(uint index);
	void clean();
	uint countBits ();
	uint getFirstBit (); // returns c_uint_max is none are found
	uint getLastBit ();	// returns c_uint_max is none are found
	vector<uint> bitList ();
	
	bool AlignCompare (const BinaryBzet& other);

	//Test functions
	static void getLastBitTest ();
	void flipTEST();           // temporary
	void setTEST();            // temporary
    void expandTEST();         // temporary
    void bzetWalkTEST();       // temporary
    void bitSetCollapseTEST(); // tempporary
	static void testShift();

	//TODO - move method to private - temporary for testing
	void align(vector<bool>& bzetA, uint& depthA, vector<bool>& bzetB, uint& depthB);
	vector<bool> bsCopy(vector<bool> bzet, uint currentPos, uint level, uint& endPos); //implements CA and CB
	vector<bool> bsNeg(vector<bool> bzet, uint currentPos, uint level, uint& endPos); //implements NA and NB
	uint bzetWalk(vector<bool> &bzet, uint currentPos, uint currentLev);
	void traversalSkeleton(vector<bool> bzet, uint level);
	vector<bool> doDataOp(string operation, vector<bool> data1, vector<bool> data2);
	vector<bool> binaryOp(int operationNo, vector<bool> bzetA, uint posA, vector<bool> bzetB, uint posB, uint level);

private:
	vector<bool> m_bzet;
	string m_bzet_string; // just for now, later we'll call getBzetString() to calculate this from m_bzet
	uint m_depth;

	//Helper functions for binary operations
	vector<bool> normalize(vector<bool> bzet, uint level);
	uint bsDrop(vector<bool> bzet, uint currentPos, uint level); //implements DA and DB
	uint subtreeNot(vector<bool>& bzet, uint currentPos, uint level); // implements _not_
	vector<bool> doTreeOp(string operation, uint level, vector<bool> bzetA, uint posA, vector<bool> bzetB, uint posB);
	
	// Helper functions for shift
	void shift(uint distance, bool isRightShift); // 0 is left, 1 is right
	u8 getBzetIndex(uint index);
	void writeLetter(vector<bool>& result, uint& resultIndex, u8 letter, uint lettersToEncode);
	void writeValue(vector<bool>& result, uint& resultIndex, u8 resultDepth, u8 value, uint valuesToEncode);
	void writeBits(bool value, uint count, vector<bool>& result, bool& valueCounting, uint& bitsCounted, uint& resultIndex, u8 resultDepth);
	void encodeBits(bool value, uint count, vector<bool>& result, bool& valueCounting, uint& bitsCounted, uint& resultIndex, u8 resultDepth, bool forced = false);

	// Helper function of bitList
	void findBits (vector<bool>& bzet, uint& bzetPos, uint& bitstringPos, uint level, vector<uint>& result);

	u8 GetNumEndingZero(uint index, uint bits);
	void bitstringToBzet(string bitstring);
	void generateBzet(bitR* bitr);


	//helper function for printPretty
	string getBzetPrettyRecursive(uint level, uint& indexB);

	// set/unset helper functions:
	void expand(vector<bool> &newbzet, uint start, uint end, uint bitLocation, bool value);
	void bitSet(uint index, bool value);
	/* get and set two bits at a time indices go by one, example:
	   say you have: string bitstring = "TT10T10";.. then you can say

	       size_t size = bitstring.size();
	       for(int i = 0; i < size; i++) {
	           setBitPairAtBzetIndex(i, bitstring[i]);
	       } */
	bitpair getBitPairAtBzetIndex(uint index);
	void setBitPairAtBzetIndex(uint index, bitpair value);
    bitpair bitpairByChangingIndexValue(bitpair currentBitPair, uint index_0or1, bool toValue);
    void bitSetCollapse(vector<bool>& bzet, uint& parentIndex, uint& leftChildIndex, uint& rightChildIndex);
    bitpair bitpairFromBools(bool leftBit, bool rightBith);
};
#endif