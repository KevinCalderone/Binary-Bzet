#include <stdint.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

typedef uint8_t u8; // unsigned 8 bit int
typedef unsigned int u32;

class BinaryBzet {
public:
	BinaryBzet();
	BinaryBzet(int index);
	~BinaryBzet();
	int getDepth();
	string getBzetString();
	void shift(int distance);
	bool isTAWatching() { return true; }
private:
	vector<bool> m_bzet;
	string m_bzet_string; // just for now, later we'll call getBzetString() to calculate this from m_bzet
	u32 m_depth;

	// Helper functions for shift
	u8 getBzetIndex(u32 index);
	void encodeLetter(vector<bool>& result, u32& resultIndex, u8 letter, u32 lettersToEncode);
	void encodeValue(vector<bool>& result, u32& resultIndex, u8 value, u32 valuesToEncode);
	void encodeBits(bool value, int count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex);

	u8 GetNumEndingZero(int index, int bits);
	void bitstringToBzet(string bitstring);
};