#include <stdint.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

typedef uint8_t u8; // unsigned 8 bit int
typedef unsigned int u32;
typedef unsigned char bitpair;

class BinaryBzet {
public:
	BinaryBzet();
	BinaryBzet(int index);
	BinaryBzet(int indexi, int indexe);
	BinaryBzet(int indexi, int indexe, int step);
	~BinaryBzet();
	int getDepth();
	string getBzetString();
	void shift(int distance);
	bool isTAWatching() { return true; } // lol
	void set(int index);
	void unset(int index);
	void testSET(); // temporary
private:
	vector<bool> m_bzet;
	string m_bzet_string; // just for now, later we'll call getBzetString() to calculate this from m_bzet
	u32 m_depth;

	// Helper functions for shift
	u8 getBzetIndex(u32 index);
	void writeLetter(vector<bool>& result, u32& resultIndex, u8 letter, u32 lettersToEncode);
	void writeValue(vector<bool>& result, u32& resultIndex, u8 resultDepth, u8 value, u32 valuesToEncode);
	void writeBits(bool value, int count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth);
	void encodeBits(bool value, int count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth, bool forced = false);

	u8 GetNumEndingZero(int index, int bits);
	void bitstringToBzet(string bitstring);

	// set/unset helper functions:
	void bitSet(int index, bool value);
	/* get and set two bits at a time indices go by one, example:
	   say you have: string bitstring = "TT10T10";.. then you can say

	       size_t size = bitstring.size();
	       for(int i = 0; i < size; i++) {
	           setBitPairAtBzetIndex(i, bitstring[i]);
	       } */
	bitpair getBitPairAtBzetIndex(int index);
	void setBitPairAtBzetIndex(int index, bitpair value);
};