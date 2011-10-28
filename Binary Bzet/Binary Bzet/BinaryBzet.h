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
	BinaryBzet(u32 index);
	BinaryBzet(u32 indexi, u32 indexe);
	BinaryBzet(u32 indexi, u32 indexe, u32 step);
	~BinaryBzet();
	
	u32 getDepth();
	string getBzetString();		//return "TTT11..." thingy
	vector<bool> getBzetBinaryString();	// return binary Bzet string
	string getBzetPretty();	//get pretty formatted Bzet
	//char getDepth();		//get the depth of the Bzet tree
	char getCharFromBzet(u32 indexB);	//return a char from Bzet

	void shift(int distance);
	bool isTAWatching() { return true; } // lol
	void set(u32 index);
	void unset(u32 index);
	void testSET(); // temporary
	static void testShift();
	BinaryBzet operator& (const BinaryBzet& rhs);
	bool operator== (const BinaryBzet& rhs);
	bool AlignCompare (const BinaryBzet& other);

	//TODO - move method to private - temporary for testing
	void align(vector<bool>& bzetA, u32& depthA, vector<bool>& bzetB, u32& depthB);

private:
	vector<bool> m_bzet;
	string m_bzet_string; // just for now, later we'll call getBzetString() to calculate this from m_bzet
	u32 m_depth;

	//Helper functions for binary operations
	void setDepth(u32 newDepth);  // Might not need this
	vector<bool> normalize(vector<bool> bzet, int level);
	vector<bool> binaryOp(int operationNo, vector<bool> bzetA, int posA, vector<bool> bzetB, int posB, int level, int& f, int& currentPos);
	vector<bool> bsCopy(vector<bool> bzet, int currentPos, int level, int& endPos); //implements CA and CB
	vector<bool> bsNeg(vector<bool> bzet, int currentPos, int level, int& endPos); //implements NA and NB
	int bsDrop(vector<bool> bzet, int currentPos, int level); //implements DA and DB
	void subtreeNot(vector<bool>& bzet, int currentPos, int level); // implements _not_
	int bzetWalk(vector<bool> bzet, int currentPos, int currentLev); //implements walk when ret_n = false
	vector<bool> doTreeOp(string operation, int level, vector<bool> bzetA, int& posA, vector<bool> bzetB, int& posB);
	vector<bool> doDataOp(string operation, vector<bool> data1, vector<bool> data2);
	
	// Helper functions for shift
	u8 getBzetIndex(u32 index);
	void writeLetter(vector<bool>& result, u32& resultIndex, u8 letter, u32 lettersToEncode);
	void writeValue(vector<bool>& result, u32& resultIndex, u8 resultDepth, u8 value, u32 valuesToEncode);
	void writeBits(bool value, u32 count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth);
	void encodeBits(bool value, u32 count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth, bool forced = false);

	u8 GetNumEndingZero(u32 index, u32 bits);
	void bitstringToBzet(string bitstring);


	//helper function for printPretty
	string getBzetPrettyRecursive(u32 level, u32& indexB);

	// set/unset helper functions:
	void bitSet(u32 index, bool value);
	/* get and set two bits at a time indices go by one, example:
	   say you have: string bitstring = "TT10T10";.. then you can say

	       size_t size = bitstring.size();
	       for(int i = 0; i < size; i++) {
	           setBitPairAtBzetIndex(i, bitstring[i]);
	       } */
	bitpair getBitPairAtBzetIndex(u32 index);
	void setBitPairAtBzetIndex(u32 index, bitpair value);
};
