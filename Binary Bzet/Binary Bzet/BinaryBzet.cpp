#include "BinaryBzet.h"
#include <stack>

BinaryBzet::BinaryBzet() {
	m_bzet_string = "";
	m_depth = 0;
}

/* index is the index of the bit in the
   BINARY BITSTRING you would like turned ON.*/
BinaryBzet::BinaryBzet(int index){
	if(index < 0)
		return;
	string temp;
	temp.append(index,'0');
	temp+='1';
	bitstringToBzet(temp);
}

/* indexe is the index of the bit in the
   BINARY BITSTRING you would like turned OFF.*/
BinaryBzet::BinaryBzet(int indexi, int indexe){
	if(indexe <= indexi || indexi < 0)
		return;
	string temp;
	temp.append(indexi,'0');
	string temp2;
	temp2.append(indexe-indexi,'1');
	bitstringToBzet(temp+temp2);
}

/* step is the number of 0s to place between
   each 1 in the BINARY BITSTRING. If a step
   lands the index on indexe, that bit will
   be turned OFF */
BinaryBzet::BinaryBzet(int indexi, int indexe, int step){
	if(indexe <= indexi || indexi < 0 || step < 0)
		return;
	string temp;
	temp.append(indexi, '0');
	int a = step;
	for(indexi;indexi<indexe;indexi++){
		if(a==step){
			temp+="1"; a=0;}
		else{
			temp+="0"; a++;}
	}
	bitstringToBzet(temp);
}

BinaryBzet::~BinaryBzet() {

}

u8 BinaryBzet::GetNumEndingZero(int index, int bits)
{
	if(index==0)
		return bits;
	u8 c = 0;
	if (!(index % 2))
		c = u8(1) + GetNumEndingZero(index >> 1, bits);
	return c;
}

int BinaryBzet::getDepth()
{
	return m_depth;
}

string BinaryBzet::getBzetString()
{
	return m_bzet_string;
}

u8 BinaryBzet::getBzetIndex(u32 index)
{
	return (m_bzet[index * 2] ? 1 : 0) << 1 | (m_bzet[index * 2 + 1] ? 1 : 0);
}

void BinaryBzet::writeLetter(vector<bool>& result, u32& resultIndex, u8 letter, u32 lettersToEncode) {
	for (u32 i = 0; i < lettersToEncode; ++i) {
		result.push_back((letter >> 1) & 0x1);
		result.push_back(letter & 0x1);
	}
}

void BinaryBzet::writeValue(vector<bool>& result, u32& resultIndex, u8 resultDepth, u8 value, u32 valuesToEncode) {
	int valuesLeftToEncode = valuesToEncode;
	while (valuesLeftToEncode > 0) {
		int numLeadingT = GetNumEndingZero(resultIndex, resultDepth - 1);
		int numToEncode = 1;

		while (numLeadingT > 0 && 2 * numToEncode <= valuesLeftToEncode) {
			numToEncode *= 2;
			numLeadingT--;
		}

		resultIndex += numToEncode;

		writeLetter(result, resultIndex, 2, numLeadingT);
		writeLetter(result, resultIndex, value, 1);
		valuesLeftToEncode -= numToEncode;
	}
}

void BinaryBzet::writeBits(bool value, int count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth) {
	if (bitsCounted % 2) {
		writeValue(result, resultIndex, resultDepth, valueCounting ? 3 : 0, (bitsCounted - 1) / 2);
		writeValue(result, resultIndex, resultDepth, (valueCounting ? 1 : 0) << 1 | (value ? 1 : 0), 1);

		valueCounting = value;
		bitsCounted = count - 1;
	}
	else {
		writeValue(result, resultIndex, resultDepth, valueCounting ? 3 : 0, bitsCounted / 2);

		valueCounting = value;
		bitsCounted = count;
	}
}

void BinaryBzet::encodeBits(bool value, int count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth, bool forced) {
	if (bitsCounted == 0) {
		valueCounting = value;
		bitsCounted = count;
	}
	else if (value == valueCounting) {
		bitsCounted += count;
	}
	else {
		writeBits(value, count, result, valueCounting, bitsCounted, resultIndex, resultDepth);
	}

	if (forced)
		writeBits(0, 0, result, valueCounting, bitsCounted, resultIndex, resultDepth);
}

void BinaryBzet::shift(int distance) {
	vector<bool> result;
	u32 resultIndex = 0;
	u32 bitsToSkip = 0;
	bool valueCounting = 0;
	u32 bitsCounted = 0;

	u32 bitstringIndex = 0;
	u32 bzetIndex = 0;

	// Can avoid need for possible normalization by implementing getIndexOfLastOne
	// and normalizing to the minimum size that will fit it.
	// For now, just assume the worst case that all bits are one.
	u32 neededSize = (1 << m_depth);
	if (distance < 0 && distance * -1 > neededSize)
		neededSize = 0;
	else 
		neededSize -= u32(-1 * distance);

	// Memory hint for bzet size to reduce reallocations
	result.reserve(m_bzet.size());

	u8 resultDepth = m_depth;
	while (1 << resultDepth < neededSize)
		resultDepth++;

	if (distance > 0) {
		bitsCounted = distance;
		valueCounting = 0;
		bitsToSkip = 0;
	}
	else {
		bitsToSkip = u32(-1 * distance);
	}

	// Decode the bitstring, and reencode at a starting index
	do {
		int numLeadingT = GetNumEndingZero(bitstringIndex, m_depth - 1);
		u32 bitsToEncode = 2 << numLeadingT;
		int value = getBzetIndex(bzetIndex);
		bzetIndex++;

		while (value == 0x2 && numLeadingT > 0) {
			value = getBzetIndex(bzetIndex);
			bzetIndex++;
			bitsToEncode /= 2;
			numLeadingT--;
		}

		bitstringIndex += bitsToEncode / 2;

		if (bitsToSkip > 0) {
			if (bitsToSkip > bitsToEncode) {
				bitsToSkip -= bitsToEncode;
				continue;
			}
			else {
				bitsToEncode -= bitsToSkip;
				bitsToSkip = 0;
			}
		}

		if (bitsToEncode == 0)
			continue;

		switch (value) {
		case 1:
			if (bitsToEncode == 1) {
				encodeBits(1, 1, result, valueCounting, bitsCounted, resultIndex, resultDepth);
			}
			else {
				encodeBits(0, 1, result, valueCounting, bitsCounted, resultIndex, resultDepth);
				encodeBits(1, 1, result, valueCounting, bitsCounted, resultIndex, resultDepth);
			}
		break;

		case 2:
			if (bitsToEncode == 1) {
				encodeBits(0, 1, result, valueCounting, bitsCounted, resultIndex, resultDepth);
			}
			else {
				encodeBits(1, 1, result, valueCounting, bitsCounted, resultIndex, resultDepth);
				encodeBits(0, 1, result, valueCounting, bitsCounted, resultIndex, resultDepth);
			}
		break;

		case 0:
			encodeBits(0, bitsToEncode, result, valueCounting, bitsCounted, resultIndex, resultDepth);
		break;

		case 3:
			encodeBits(1, bitsToEncode, result, valueCounting, bitsCounted, resultIndex, resultDepth);
		break;
		};

	} while (bitstringIndex < u32(1) << (m_depth - 1));

	// encode enough 0's at the end to fill in rest of bzet
	u32 bitsEncoded = (resultIndex << 1);
	u32 bitsFound = bitsEncoded + bitsCounted;
	u32 bitsExpected = (1 << resultDepth);

	if (bitsFound > bitsExpected) {
		bitsCounted = bitsExpected - bitsEncoded;
		encodeBits(0, 0, result, valueCounting, bitsCounted, resultIndex, resultDepth, true);
	}
	else {
		u32 bitsNeeded = bitsExpected - bitsFound;
		encodeBits(0, bitsNeeded, result, valueCounting, bitsCounted, resultIndex, resultDepth, true);
	}

	// copy result bzet into this bzet
	m_bzet = result;
}

void BinaryBzet::bitstringToBzet(string bitstring)
{
	string input = "";
	int i = 1;
	for(i;i<31;i++)
		if(bitstring.length()<=(1<<i))
			break;
	input.append((1<<i)-bitstring.length(),'0');
	bitstring += input;
	input = "";
	for(unsigned int i=0; i<bitstring.length(); i+=2){
		if(bitstring.at(i)=='0')
			if(bitstring.at(i+1)=='0')
				input.push_back('0');
			else
				input.push_back('t');
		else
			if(bitstring.at(i+1)=='0')
				input.push_back('T');
			else
				input.push_back('1');
	}
	m_depth = log((float)input.length())/log(2.0) + 1;
	i = 0;
	int num_end_zero;
	do{
		num_end_zero = GetNumEndingZero(i,m_depth-1);
		int num_to_encode = 1;
		int number_of_times_to_expand = num_end_zero;
		if (input.at(i) == '1' || input.at(i) == '0')
			while (number_of_times_to_expand  > 0) {
				bool same = true;
				//cout << number_of_times_to_expand << " " << num_to_encode <<  "   " << input.length()-1 << endl;////////////////
				for(int j = i; (j < i + (1 << num_to_encode) - 1) && (j < input.length()-1); j++){////////////////////
					same = same && (input[j]==input[j+1]);}
				//cout << endl;/////////////////
				if (!same)
					break;
				num_to_encode*=2;
				number_of_times_to_expand--;
			}
			string temp = "";
			m_bzet_string += temp.append(number_of_times_to_expand,'T');
			m_bzet_string += input.at(i);
			i +=num_to_encode;
			if (num_to_encode == 1) {
				m_bzet_string += input.at(i);
				i++;
			}
	} while ( GetNumEndingZero(i, m_depth-1) != m_depth-1);
	for(int i=0;i<m_bzet_string.length();i++){
		switch(m_bzet_string.at(i)){
		case '0':
			m_bzet.push_back(0); m_bzet.push_back(0); break;
		case '1':
			m_bzet.push_back(1); m_bzet.push_back(1); break;
		case 'T':
			m_bzet.push_back(1); m_bzet.push_back(0); break;
		case 't':
			m_bzet.push_back(0); m_bzet.push_back(1); break;
		}
		}
}

void BinaryBzet::set(int index) {
	bitSet(index, true);
}

void BinaryBzet::unset(int index) {
	bitSet(index, false);	
}

// currently only traverses bzet
void BinaryBzet::bitSet(int index, bool value) {
	//    s: a stack to keep track of the level the current index in the array is for
	// seen: an array to keep track of the number of times a level has been seen.

	int bzetIndex = 0; // bits 0-7 containg tree depth.. first node starts at bit 8;
	int bitstringIndex = 0; // index bitstring if it had no been compressed to a bzet.

	stack<int> s;
	int *seen = new int[m_depth];

	int top = m_depth -1;;
	int next;
	s.push(top);
	seen[top] = 1;

	bitpair curBP;
	while (!s.empty() && bzetIndex < m_bzet.size()) {
		top = s.top();
		seen[top]++;
		if (seen[top] == 2)
			s.pop();

		curBP = getBitPairAtBzetIndex(bzetIndex);
		if (top == 1) {
			// curBP: T has special meaning now
			cout << curBP;
		} else if (curBP == '0' || curBP == '1') {
			// there is 2^top 1's or 0's
			cout << curBP;
		} else if (bzetIndex == 0 || !s.empty()) { 
			next = top - 1;
			s.push(next);
			seen[next] = 0;
		}
		bzetIndex++;
	}

	delete [] seen;
}

bitpair BinaryBzet::getBitPairAtBzetIndex(int index) {
	int indexA = index*2 + 8; // skip first 8 bits since thats where the tree depth is set.
	if (indexA + 1 < m_bzet.size()) {
		bool lBit = m_bzet[indexA];
		bool rBit = m_bzet[indexA+1];
		return lBit ? ( rBit ? '1' : 'T') : ( rBit ?  't': '0');
	}
	return 'x';
}

void BinaryBzet::setBitPairAtBzetIndex(int index, bitpair value) {
	if (index >= m_bzet.size())
		m_bzet.reserve(index);

	bool lBit, rBit;
	switch (value) {
		case '0': lBit = 0; rBit = 0; break;
		case 't': lBit = 0; rBit = 1; break;
		case 'T': lBit = 1; rBit = 0; break;
		case '1': lBit = 1; rBit = 1; break;
		default:
			cout << "\nUnrecognized value for a bitpair\n";
			break;
	}

	int indexA = index*2 + 8; // skip first 8 bits since thats where the tree depth is set.
	m_bzet[indexA] = lBit;
	m_bzet[indexA+1] = rBit;
}

// temporary
void BinaryBzet::testSET() {
	cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		 << "TEST: set():\n"
		 << "--------------------------------------------------\n"
		 << "The following test is for the bitstring 10011111\n"
		 << "bzet should be TTTt1 so:\n\n";

	// d is bzet representation of the bitstring 10011111:
	// first 8 values are for depth
	// last 10 values are for TTTt1.
	bool d[] = { 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1 };

	vector<bool> test_bzet;
	// set first 8 bits to represent depth which is 4
	vector<bool>::iterator it = test_bzet.begin();
	test_bzet.insert(it, d, d+18);

	cout << "Verify test_bzet is set correctly (size = " << test_bzet.size() << ")\n";
	for (size_t i = 0; i < test_bzet.size(); i++) {
		if (i != 0 && i % 4 == 0)
			cout << ' ';
		cout << (test_bzet[i]) ? '1' : '0';
	}
	cout << "\n";

	// set function works on member variables so...
	m_bzet = test_bzet;
	m_depth = 4;

	cout << "\n\nrunning set function....\n\n";
	bitSet(0, 0); // should display Tt1
	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
}
