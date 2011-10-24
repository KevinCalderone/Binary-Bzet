#include <stdint.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

typedef uint8_t u8; // unsigned 8 bit int

class BinaryBzet {
public:
	BinaryBzet();
	BinaryBzet(int index);
	BinaryBzet(int indexi, int indexe);
	BinaryBzet(int indexi, int indexe, int step);
	~BinaryBzet();
	int getDepth();
	string getBzetString();
private:
	vector<bool> m_bzet;
	string m_bzet_string; // just for now, later we'll call getBzetString() to calculate this from m_bzet
	int m_depth;
	u8 GetNumEndingZero(int index, int bits);
	void bitstringToBzet(string bitstring);
};