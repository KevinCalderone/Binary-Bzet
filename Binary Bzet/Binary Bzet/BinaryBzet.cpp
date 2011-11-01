#include "BinaryBzet.h"
#include <stack>
#include <cassert>
#include <iomanip>
#include <sstream>

//Used for determining 6 cases in binary operations
static int g_caseType[] = {0,0,0,0,1,6,3,6,2,4,6,6,5,6,6,6};
static string g_binOp[][6]  = {           
	{ "FALSE", "0",  "DB0", "DA0", "DB0", "DA0" }, //00 0000 Result
	{ "AND",   "&",  "DB0", "DA0", "CB",  "CA"  }, //01 0001    |
    { "A<-B",  "<-", "CB",  "CA",  "NB",  "DA0" }, //02 0010    |
    { "A",     "a",  "DB0", "CA",  "DB0", "CA"  }, //03 0011    V
    { "????",  "x",  "DB0", "DA0", "DB0", "CA"  }, //04 0100
    { "B",     "b",  "CB",  "DA0", "CB",  "DA0" }, //05 0101
    { "XOR",   "^",  "CB",  "CA",  "NB",  "NA"  }, //06 0110
    { "OR",    "|",  "CB",  "CA",  "DB1", "DA1" }, //07 0111
    { "NOR",   "~|", "NB",  "NA",  "DB0", "DA0" }, //08 1000
    { "EQ",    "~^", "NB",  "NA",  "CB",  "CA"  }, //09 1001
    { "~B",    "~b", "NB",  "DA0", "NB",  "DA0" }, //10 1010
    { "????",  "x",  "CB",  "CA",  "CB",  "CA"  }, //11 1011
    { "~A",    "~a", "DB0", "NA",  "DB0", "NA"  }, //12 1100
    { "A->B",  "->", "DB0", "NA",  "CB",  "CA"  }, //13 1101
    { "NAND",  "~&", "CB",  "DA1", "NB",  "NA"  }, //14 1110
    { "TRUE",  "1",  "DB1", "DA1", "DB1", "DA1" }}; //15 1111

BinaryBzet::BinaryBzet() {
	m_depth = 0;
}

/* index is the index of the bit in the
   BINARY BITSTRING you would like turned ON.*/
BinaryBzet::BinaryBzet(u32 index){
	if(index < 0)
		return;
	string temp;
	temp.append(index,'0');
	temp+='1';
	bitstringToBzet(temp);
}

///* indexe is the index of the bit in the
//   BINARY BITSTRING you would like turned OFF.*/
////BinaryBzet::BinaryBzet(u32 indexi, u32 indexe){
////	BinaryBzet(indexi,indexe,0);
//	/*if(indexe <= indexi || indexi < 0)
//		return;
//	string temp;
//	temp.append(indexi,'0');
//	string temp2;
//	temp2.append(indexe-indexi,'1');
//	bitstringToBzet(temp+temp2);*/
////}

/* step is the number of 0s to place between
   each 1 in the BINARY BITSTRING. If a step
   lands the index on indexe, that bit will
   be turned OFF */
BinaryBzet::BinaryBzet(u32 indexi, u32 indexe, u32 step){
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
	while(temp.length()<=indexe)
		temp+="0";
	bitstringToBzet(temp);
}

BinaryBzet::BinaryBzet(string bitstring){
	bitstringToBzet(bitstring);
}

BinaryBzet::BinaryBzet(vector<bool>* bzetvector, u32 depth){
	m_bzet = *bzetvector;
	m_depth = depth;
}

BinaryBzet::~BinaryBzet() {

}

u8 BinaryBzet::GetNumEndingZero(u32 index, u32 bits)
{
	if(index==0)
		return bits;
	u8 c = 0;
	if (!(index % 2))
		c = u8(1) + GetNumEndingZero(index >> 1, bits);
	return c;
}

u32 BinaryBzet::getDepth()
{
	return m_depth;
}

string BinaryBzet::getBzetString(){
	//return m_bzet_string;

	//test:TTT1TTTTT0tT10 TTTt000
	/*bool myints[] = {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,};
    vector<bool> m_bzet_new (myints, myints + sizeof(myints) / sizeof(bool) );
	m_bzet=m_bzet_new;
    m_depth = 5;*/

	ostringstream output;
	output<< m_depth<<":";
	u32 indexB;
	for(indexB = 0 ; indexB < m_bzet.size()/2; indexB++){
		output<<getCharFromBzet(indexB);
	}
	return output.str();
}

//vector<bool> BinaryBzet::getBzetBinaryString(){
//	return m_bzet;
//}

string 	BinaryBzet::getBzetPretty(){	//get pretty formatted Bzet
	//test:TTT1TTTTT0tT10 TTTt000
	/*bool myints[] = {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,};
    vector<bool> m_bzet_new (myints, myints + sizeof(myints) / sizeof(bool) );
	m_bzet=m_bzet_new;
    m_depth = 5;*/

	ostringstream oss;
	oss << m_depth<<": ";
	string output = oss.str();
	u32 indexB=1;
	if(m_depth > 0){
		output[2] = getCharFromBzet(0);
		output += getBzetPrettyRecursive(m_depth-1,indexB);
	}
	return output;

}// end getBzetPRetty()

string BinaryBzet::getBzetPrettyRecursive(u32 level, u32& indexB){
	string output = "[ - ]";

	if(level == 1){	
		output[1] = getCharFromBzet(indexB);
		output[3] = getCharFromBzet(++indexB);
		output += "\n";
		return output;
	}else{
		
		char leftChar = getCharFromBzet(indexB);
		int  rightInd = 0;
		int  num_of_space = 3+5*(m_depth-level);
		ostringstream space;
		space <<setw(num_of_space)<<"";
		
		if(leftChar != 'T'){
			output[1] = leftChar;
			output[3] = getCharFromBzet(++indexB);
			output += "\n";
			output += space.str()+ getBzetPrettyRecursive(level-1, ++indexB);		
		}else{
			output[1] = leftChar;
			output += getBzetPrettyRecursive(level-1, ++indexB);			
			char rightChar = getCharFromBzet(++indexB);
			output[3] = rightChar;
			if(rightChar == 'T') output += space.str()+ getBzetPrettyRecursive(level-1, ++indexB);
		}

		return output;

	}//end if-else level
}//end getBzetPrettyResucrive()

u8 BinaryBzet::getBzetIndex(u32 index) {
	// Get the value of 2-bit number in the bzet representing '0', 't', 'T', or '1'

	return (m_bzet[index * 2] ? 1 : 0) << 1 | (m_bzet[index * 2 + 1] ? 1 : 0);
}


/*return character in Bzet at indexB, return 'E' if indexB is out of bound.*/
char BinaryBzet::getCharFromBzet(u32 indexB){
	
	if((indexB*2+1) >= m_bzet.size()) return 'E';
	
	bool firstBit = m_bzet.at(indexB*2);
	bool secondBit = m_bzet.at(indexB*2+1);

	if(!firstBit && !secondBit) return '0';
    else if(!firstBit && secondBit) return 't';
    else if(firstBit && !secondBit) return 'T';
    else /*if(firstBit && secondBit)*/ return '1';
}

void BinaryBzet::writeLetter(vector<bool>& result, u32& resultIndex, u8 letter, u32 lettersToEncode) {
	// Write a letter into the result bzet
	for (u32 i = 0; i < lettersToEncode; ++i) {
		result.push_back((letter >> 1) & 0x1);
		result.push_back(letter & 0x1);
	}
}

void BinaryBzet::writeValue(vector<bool>& result, u32& resultIndex, u8 resultDepth, u8 value, u32 valuesToEncode) {
	// Write the buffered data into the result bzet in the most compact form
	int valuesLeftToEncode = valuesToEncode;
	while (valuesLeftToEncode > 0) {
		int numLeadingT = GetNumEndingZero(resultIndex, resultDepth - 1);
		int numToEncode = 1;

		// Collapse levels of the tree if possible for an optimal representation
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

void BinaryBzet::writeBits(bool value, u32 count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth) {
	// If there is an odd number of bits counted, you need ot make sure the boundry is encoded 
	// correctly since everything is encoded in pairs of bits
	if (bitsCounted % 2) {
		writeValue(result, resultIndex, resultDepth, valueCounting ? 3 : 0, (bitsCounted - 1) / 2);
		writeValue(result, resultIndex, resultDepth, (valueCounting ? 1 : 0) << 1 | (value ? 1 : 0), 1);

		valueCounting = value;
		bitsCounted = count - 1;
	}
	// If there are an even number of bits counted up, simply encode them all
	else {
		writeValue(result, resultIndex, resultDepth, valueCounting ? 3 : 0, bitsCounted / 2);

		valueCounting = value;
		bitsCounted = count;
	}
}

void BinaryBzet::encodeBits(bool value, u32 count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex, u8 resultDepth, bool forced) {
	// If it is not currently counting up any data, start counting up this data and encode it later
	if (bitsCounted == 0) {
		valueCounting = value;
		bitsCounted = count;
	}
	// If you found more of the same value you are currently counting, just increase the number found
	else if (value == valueCounting) {
		bitsCounted += count;
	}
	// If you found data that is different than what you are counting, write the buffered data out and start
	// counting up the new data
	else {
		writeBits(value, count, result, valueCounting, bitsCounted, resultIndex, resultDepth);
	}
	
	// Force it to output the remaining data instead of saving it
	if (forced)
		writeBits(0, 0, result, valueCounting, bitsCounted, resultIndex, resultDepth);
}

void BinaryBzet::shift(int distance) {
	// This algoritm parses through the bzet string extracting the data that it is storing,
	// and reencodes the data back into a result bzet starting from some offset into the 
	// original bzet.

	// TODO: -This should probably be encapsulated into a seperate struct so that it will be a lot
	//	     cleaner passing the algorithm state through the control flow of the algorithm.
	//		 -This produces a bzet that is fully collapsed, but it may not output a correctly
	//       normalized bzet(eg. it may use a depth that is too large).  It picks the output
	//		 depth based on the worst case, and this can easily be improved to use the optimal 
	//       depth by adding a method that will find the index of the last one in the bzet.
	//		 -This only updates m_bzet, so m_bzet_string will be incorrect after execution.
	//	     -The copy of the result into the current bzet should be done in a copy constructor.

	// ALGORITHM STATE
	vector<bool> result;		// The result bzet
	u32 resultIndex = 0;		// The index across the data level of the result bzet to encode next
	u32 bitsToSkip = 0;			// Number of bits left to ignore when parsing through the original bzet
	bool valueCounting = 0;		// Whether it is currently parsing through '0' or '1' in the bzet
	u32 bitsCounted = 0;		// Number of bits it has found in a row that have the value "valueCounting"
	u32 bitstringIndex = 0;		// The index across the data level of the current bzet to decode next
	u32 bzetIndex = 0;			// The index of the current letter in the compressed bzet form that it is currently parsing

	// DEPTH OF RESULT BZET
	// Can avoid need for normalization by implementing getIndexOfLastOne
	// and finding the minimum size depth bzet that will fit it.
	// For now, just assume the worst case that all bits are one.
	u32 neededSize = (1 << m_depth);
	if (distance < 0 && (u32)(distance * -1) > neededSize)
		neededSize = 0;
	else 
		neededSize -= u32(-1 * distance);

	u8 resultDepth = m_depth;
	while ((u32)(1 << resultDepth) < neededSize)
		resultDepth++;

	// Memory hint for bzet size to reduce reallocations of result buffer
	result.reserve(m_bzet.size());

	// STATE INITIALIZATION
	// Determine where to start encoding into the result bzet
	if (distance > 0) {
		bitsCounted = distance;
		valueCounting = 0;
		bitsToSkip = 0;
	}
	else {
		bitsToSkip = u32(-1 * distance);
	}

	// MAIN ALGORITHM
	// Parse through the bzet, and reencode the values into the result bzet
	do {
		// Parse a piece of the bzet, and determine what it is encoding
		// and how many bits that piece is encoding by looking at how many
		// leading 'T's there are
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

		// Throw away data if you have not reached the part you want to access yet
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

		// Encode the extracted data into the result bzet
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

	// FINISHING TOUCHES
	// Fill in the rest of the result bzet with the left over data found, and add
	// enough '0' bits to fill in the rest of the bzet if there is extra space
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
	m_depth = resultDepth;
}

void BinaryBzet::bitstringToBzet(string bitstring)
{
	string input = "";
	u32 i = 1;
	for(i;i<31;i++)
		if(bitstring.length()<=(u32)(1<<i))
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
	m_depth = (u32)(log((float)input.length())/log(2.0) + 1);
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
				for(u32 j = i; (j < i + (1 << num_to_encode) - 1) && (j < input.length()-1); j++){////////////////////
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
			if (num_to_encode == 1 && m_depth!=1 /*input.length() > i*/) {
				m_bzet_string += input.at(i);
				i++;
			}
	} while ( GetNumEndingZero(i, m_depth-1) != m_depth-1);
	for(u32 i=0;i<m_bzet_string.length();i++){
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

void BinaryBzet::set(u32 index) {
	bitSet(index, true);
}

void BinaryBzet::unset(u32 index) {
	bitSet(index, false);
}

// currently only traverses bzet
// TODO: Collapsing
void BinaryBzet::bitSet(u32 index, bool value) {
	//    s: a stack to keep track of the level the current index in the array is for
	// seen: an array to keep track of the number of times a level has been seen.

	u32 bzetIndex = 0;
	u32 bitIndex = 0; // index bitstring if it had not been compressed to a bzet.

	stack<u32> s;
	u32 *seen = new u32[m_depth];

	u32 top = m_depth - 1;
	u32 next;
	s.push(top);
	seen[top] = 1;

	bitpair curBP;
	while (!s.empty() && bzetIndex < (u32)(m_bzet.size())) {
		top = s.top();
		seen[top]++;
		if (seen[top] == 2)
			s.pop();

		curBP = getBitPairAtBzetIndex(bzetIndex);
		if (top == 1) {
			// curBP: T has special meaning now
			bitpair newBP;
			// change first bit?
			if (index == bitIndex) {
				if (value) {
					// change 1st bit to 1
					switch (curBP) {
						case '0': newBP = 'T'; break;
						case 't': newBP = '1'; break;
						case 'T': case '1': newBP = curBP; break;
						default:
							cout << "Unrecognized bitpair value!\n";
							break;
					}
				} else {
					// change 1st bit to 0
					switch (curBP) {
						case '0': case 't': newBP = curBP; break;
						case 'T': newBP = '0'; break;
						case '1': newBP = 't'; break;
						default:
							cout << "Unrecognized bitpair value!\n";
							break;
					}
				}
				setBitPairAtBzetIndex(bzetIndex, newBP);
				curBP = newBP;
			} else if (index == bitIndex + 1) { // change 2nd bit?
				if (value) {
					// change 2nd bit to 1
					switch (curBP) {
						case '0': newBP = 't'; break;
						case 'T': newBP = '1'; break;
						case 't': case '1': newBP = curBP; break;
						default:
							cout << "Unrecognized bitpair value!\n";
							break;
					}
				} else {
					// change 2nd bit to 0
					switch (curBP) {
						case '0': case 'T': newBP = curBP; break;
						case 't': newBP = '0'; break;
						case '1': newBP = 'T'; break;
						default:
							cout << "Unrecognized bitpair value!\n";
							break;
					}
				}
				setBitPairAtBzetIndex(bzetIndex, newBP);
				curBP = newBP;
			}
			cout << curBP;
			bitIndex += 2;
		} else if (curBP == '0' || curBP == '1') {
			// there is 2^top 1's or 0's
			u32 size = (u32)pow(2.0, (double)top);
			if (index >= bitIndex && index < bitIndex + size) {
				// the bit is in this range
				vector<bool> expandTo;
				// push 'T' onto vector;
				//expandTo.push_back(1);
				//expandTo.push_back(0);
				expand(expandTo, bitIndex, bitIndex + size - 1, index, value);
				cout << "\nexpandTo:\n";
				for (size_t i = 0; i < expandTo.size(); i++)
					cout << expandTo[i] ? "1" : "0";
				// erase current index then insert expansion
				m_bzet.erase(m_bzet.begin() + bzetIndex*2-1, m_bzet.begin() + bzetIndex*2 + 1);
				cout << "\nbzet is now:\n";
				for (size_t i = 0; i < m_bzet.size(); i++)
					cout << m_bzet[i] ? "1" : "0";
				m_bzet.insert(m_bzet.begin() + bitIndex*2, expandTo.begin(), expandTo.end());
				// done
			} else {
				// bit to set is not in this range..
				bitIndex += size; // skip bits
			}
		} else if (bzetIndex == 0 || !s.empty()) {
			// set up next index level
			next = top - 1;
			s.push(next);
			seen[next] = 0;
		}
		bzetIndex++;
	}
	delete [] seen;
}

void BinaryBzet::expand(vector<bool> &newbzet, u32 start, u32 end, u32 bitLocation, bool value) {
	int midPoint = (start + end) / 2;
	if (bitLocation >= start && bitLocation <= midPoint) {
		if (end - start == 1) {
			// size is two
			if (bitLocation == start) {
				// left bit is change
				newbzet.push_back(value);
				newbzet.push_back(!value);
			} else {
				// right bit is change
				newbzet.push_back(!value);
				newbzet.push_back(value);
			}
		} else {
			// left node: push 'T' and go into recursive call
			newbzet.push_back(1);
			newbzet.push_back(0);
			expand(newbzet, start, (start + end)/2, bitLocation, value);

			// right node stays the same
			newbzet.push_back(!value);
			newbzet.push_back(!value);
		}
	} else {
		if (end - start == 1) { // size is two
			if (bitLocation == start) {
				// left bit is changed
				newbzet.push_back(value);
				newbzet.push_back(!value);
			} else {
				// right bit is changed
				newbzet.push_back(!value);
				newbzet.push_back(value);
			}
		} else {
			// left node: stays the same
			newbzet.push_back(!value);
			newbzet.push_back(!value);

			// right node: push 'T' and go into recursive call
			newbzet.push_back(1);
			newbzet.push_back(0);
			expand(newbzet, (start + end)/2, end, bitLocation, value);
		}
	}
}

bitpair BinaryBzet::getBitPairAtBzetIndex(u32 index) {
	u32 indexA = index * 2; // doesn't skip first 8 bits since depth is its own member variable
	if (indexA + 1 < m_bzet.size()) {
		bool lBit = m_bzet[indexA];
		bool rBit = m_bzet[indexA+1];
		return lBit ? ( rBit ? '1' : 'T') : ( rBit ?  't': '0');
	}
	return 'x';
}

void BinaryBzet::setBitPairAtBzetIndex(u32 index, bitpair value) {
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

	int indexA = index * 2; // doesn't skip first 8 bits since depth is its own member variable
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
	bool d[] = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1 };

	vector<bool> test_bzet;
	test_bzet.insert(test_bzet.begin(), d, d+10);

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
	bitSet(5, 0); // should display 0t1

	cout << "\nVerify m_bzet is changed correctly (size = " << m_bzet.size() << ")\n";
	for (size_t i = 0; i < m_bzet.size(); i++) {
		if (i != 0 && i % 4 == 0)
			cout << ' ';
		cout << (m_bzet[i]) ? '1' : '0';
	}
	cout << "\n";

	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
}


/*    # The case type breaks each trees c & d bits
    # from the left side of the operator and the c & d
    # bits from the right side (total of 4 bits
    # and 16 possibilities) into 6 fundamental cases
    # Trees never have associated data bits thus in
    # the comments an x or error is placed before
    # a 1 data bit that corresponds to a tree bit.
    # These are type 6 data errors and should never
    # occur. This case table maps each of the 16
    # possibilities into the 6 operational cases 0-5
    # and case 6 representing a data error.

    # Inputs  DD      0T     T0     1T     T1    TT
    #        Case0   Case1  Case2  Case3  Case4 Case5
    #
    #                       c1,c2, d1,d2
    case_type = [0,  #  0     0,0,  0, 0  DD
                 0,  #  1     0,0,  0, 1  DD
                 0,  #  2     0,0,  1, 0  DD
                 0,  #  3     0,0,  1, 1  DD
                 1,  #  4     0,1,  0, 0  0T
                 6,  #  5     0,1,  0,x1  0T
                 3,  #  6     0,1,  1, 0  1T
                 6,  #  7     0,1,  1,x1  1T
                 2,  #  8     1,0,  0, 0  T0
                 4,  #  9     1,0,  0, 1  T1
                 6,  # 10     1,0, x1, 0  T0
                 6,  # 11     1,0, x1, 1  T1
                 5,  # 12     1,1,  0, 0  TT
                 6,  # 13     1,1,  0,x1  TT
                 6,  # 14     1,1, x1, 0  TT
                 6,  # 15     1,1, x1,x1  TT
                 ]


    # Definitions of all 16 binary Boolean operations.
    # This table drives the computation both of data and tree nodes.
    # Each operation is a 6 tuple with the name of the operation,
    # a machine operation code to do on byte to byte operations,
    # and the action to take for each possible case. Case 0 is
    # the binop row index reindexed by the right and left hand data
    # bits as a number 0-3.

    binop  = (
    # 0        1     2      3      4      5        Tuple Index
    #                                               a= 0011
    #                                        operation ....
    #                                               b= 0101  Case                                 
    #                       index = a<<1 | b  ===>     0123    0               
    ( 'FALSE', '0',  'DB0', 'DA0', 'DB0', 'DA0' ), #00 0000 Result
    ( 'AND',   '&',  'DB0', 'DA0', 'CB',  'CA'  ), #01 0001    |
    ( 'A<-B',  '<-', 'CB',  'CA',  'NB',  'DA0' ), #02 0010    |
    ( 'A',     'a',  'DB0', 'CA',  'DB0', 'CA'  ), #03 0011    V
    ( '????',  'x',  'DB0', 'DA0', 'DB0', 'CA'  ), #04 0100
    ( 'B',     'b',  'CB',  'DA0', 'CB',  'DA0' ), #05 0101
    ( 'XOR',   '^',  'CB',  'CA',  'NB',  'NA'  ), #06 0110
    ( 'OR',    '|',  'CB',  'CA',  'DB1', 'DA1' ), #07 0111
    ( 'NOR',   '~|', 'NB',  'NA',  'DB0', 'DA0' ), #08 1000
    ( 'EQ',    '~^', 'NB',  'NA',  'CB',  'CA'  ), #09 1001
    ( '~B',    '~b', 'NB',  'DA0', 'NB',  'DA0' ), #10 1010
    ( '????',  'x',  'CB',  'CA',  'CB',  'CA'  ), #11 1011
    ( '~A',    '~a', 'DB0', 'NA',  'DB0', 'NA'  ), #12 1100
    ( 'A->B',  '->', 'DB0', 'NA',  'CB',  'CA'  ), #13 1101
    ( 'NAND',  '~&', 'CB',  'DA1', 'NB',  'NA'  ), #14 1110
    ( 'TRUE',  '1',  'DB1', 'DA1', 'DB1', 'DA1' )) #15 1111
    #       Shortcut  <-- C1 to C4 Actions -->  Recur  Result
    # Name     raw   Case1  Case2  Case3  Case4 Case5  Case 0   
    # Inputs         0T     T0     1T     T1    TT
    # 0        1     2      3      4      5      

*/

//
BinaryBzet BinaryBzet::operator &(const BinaryBzet& rhs)
{
	vector<bool> bzetA = m_bzet;
	vector<bool> bzetB = rhs.m_bzet;
	vector<bool> emptyBzet;
	
	//Create emptyBzet
	//TODO confirm emptyBzet representation
	emptyBzet.push_back(0);
	emptyBzet.push_back(0);
	
	//empty set against anything
	if(equal( bzetA.begin(), bzetA.end(), emptyBzet.begin()))
	{
		//return empty bzet
		//TODO confirm emptyBzet representation
		return BinaryBzet();
	}
	if(equal( bzetB.begin(), bzetB.end(), emptyBzet.begin()))
	{
		// return empty bzet
		//TODO confirm emptyBzet representation
		return BinaryBzet();
	}

	u32 depthA = m_depth;
	u32 depthB = rhs.m_depth;

	align(bzetA,depthA,bzetB,depthB);
	
	//Full set vs anything
	//TODO move to above align after testing
	vector<bool> fullBzet;
	//TODO confirm full representation
	fullBzet.push_back(1);
	fullBzet.push_back(1);
	if(equal( bzetA.begin(), bzetA.end(), fullBzet.begin()))
	{
		return rhs;
	}
	if(equal( bzetB.begin(), bzetB.end(), fullBzet.begin()))
	{
		return *this;
	}
	
	//Level 0 bzet
	//TODO finish once constructor implemented
	if(depthA == 0)
	{
		vector<bool>::iterator itA;
		vector<bool>::iterator itB;
		vector<bool> bzetRet;

		itA = bzetA.begin();
		itB = bzetB.begin();

		for(itA; itA != bzetA.end(); itA++)
		{
			bzetRet.push_back((*itA) & (*itB));
			itB++;
		}
		//TODO change this - need new consturctor
		// return BinaryBzet(bzetRet,0);
		return rhs;
	}

	//Non-0 Level bzet
	int flag = 0;
	u32 curPosA = 0;
	u32 curPosB = 0;
	//TODO: curPos is used for debug - remove later
	vector<bool> bzetRet = binaryOp(1,bzetA,0,bzetB,0,depthA,flag,curPosA,curPosB);
	if(flag == 0)
	{
		//TODO change this - need new consturctor
		//TODO finish normlaize function
		// BinaryBzet(bzetRet,0);
		// return normalize(bzetRet,depthA);
		return rhs;
	}
	else if(flag == 1)
	{
		//TODO change this - need new consturctor
		//return the full bitset
		//return BinaryBzet(fullBzet,depthA);
		return rhs;
	}
	else if(flag == 2)
	{
		//TODO change this - need new consturctor
		//return the empty set
		//return BinaryBzet(emptyBzet,1);
		return rhs;
	}
	return rhs;
}

//flag is 0 - return NONE python
//flag is 1 - return true python
//flag is 2 - return false python
//branchData == cdr python
//treeData == ddr python
vector<bool> BinaryBzet::binaryOp(int operationNo, vector<bool> bzetA, u32 posA, vector<bool> bzetB, u32 posB, u32 level, int& f, u32& currentPosA, u32& currentPosB)
{
	string* operation = g_binOp[operationNo];
	vector<bool> bzet1 = bzetA;
	u32 currentPos1 = posA; 
	vector<bool> bzet2 = bzetB; 
	u32 currentPos2 = posB;
	
	vector<bool> resultBzet;
	//Handle Level 1 case - subtrees just have data
	if(level == 1)
	{
		//TBD
	}
	else
	{
		//Determine case of data
		//TODO - create function to look up value
		bool lBit = bzet1[currentPos1];
		bool rBit = bzet1[currentPos1+1];
		bitpair bp1 = lBit ? ( rBit ? '1' : 'T') : ( rBit ?  't': '0');
		currentPos1 += 2;

		lBit = bzet1[currentPos2];
		rBit = bzet1[currentPos2+1];
		bitpair bp2 = lBit ? ( rBit ? '1' : 'T') : ( rBit ?  't': '0');
		currentPos2 += 2;
		//TODO - replace with use of case_type array
		int caseType = 6;
		if((bp1 == '1'|| bp1 == '0') && (bp2 == '1' || bp2 == '0'))
		{
			caseType = 0;
		}
		else if(bp1 == '0' && bp2 == 'T')
		{
			caseType = 1;
		}
		else if(bp1 == 'T' && bp2 == '0')
		{
			caseType = 2;
		}
		else if(bp1 == '1' && bp2 == 'T')
		{
			caseType = 3;
		}
		else if(bp1 == 'T' && bp2 == '1')
		{
			caseType = 4;
		}
		else if(bp1 == 'T' && bp2 == 'T')
		{
			caseType = 5;
		}
		//Invalid case type
		else
		{
			cout << "Invalid Case Type";
		}

		//00 or 11 or 10 or 01
		if( caseType == 0)
		{
			//TBD
		}
		//0T T0 1T or T1
		else if (caseType >= 1 && caseType <= 4)
		{
			string oper = operation[caseType + 1];
			//python call cp1, cp2 ,tr = do_tree_op(opr, lev-1,bset1,cp1,bset2,cp2)
			vector<bool> res = doTreeOp(oper,level-1,bzet1, currentPos1, bzet2, currentPos2);
			//apped res to resultBzet + handle compression possibly
		}
		//TT
		else if(caseType ==5)
		{
			//recurse the tree and apped to result
			u32 currPosA = 0;
			u32 currPosB = 0;
			int resultFlag = 0;
			vector<bool> res = binaryOp(operationNo,bzet1, currentPos1, bzet2, currentPos2, level-1, resultFlag, currPosA, currPosB);
			//update positions
			currentPos1 = currPosA;
			currentPos2 = currPosB;
			//TODO: Append res to resultBzet
			//TODO: Possibly handle flag stuff
		}
	}

	f = 0;
	//TODO do stuff with flag possibly


	return resultBzet;
}

//implements CA and CB
//TODO test when Walk Finished - copying currently seems to work
vector<bool> BinaryBzet::bsCopy(vector<bool> bzet, u32 currentPos, u32 level, u32& endPos)
{
	if(currentPos >= (u32)bzet.size())
	{
		endPos = bzet.size();
	}
	else
	{
	//	endPos = bzetWalk(bzet,currentPos,level);
	}
	vector<bool> returnBzet;
	//gets copy of subtree at positions currentPos-endPos
	for(u32 i = currentPos; i<endPos; i++)
	{
		returnBzet.push_back(bzet.at(i));
	}
	return returnBzet;
}

//implements NA and NB
//TODO test when Walk Finished - copying currently seems to work
vector<bool> BinaryBzet::bsNeg(vector<bool> bzet, u32 currentPos, u32 level, u32& endPos)
{
	vector<bool> returnBzet;
	if(currentPos >= bzet.size())
	{
		endPos = bzet.size();
		//TODO - return empty vector<bool> bytes([]) - check correctness
		return returnBzet;
	}
	else
	{
//		endPos = bzetWalk(bzet,currentPos,level);
	}
	//gets copy of subtree at positions currentPos-endPos
	for(u32 i = currentPos; i<endPos; i++)
	{
		returnBzet.push_back(bzet.at(i));
	}
	//not the subtree in place
	//subtreeNot(returnBzet, 1, level);
	return returnBzet;
}

//implements DA and DB
//returns position of next subtree
int BinaryBzet::bsDrop(vector<bool> bzet, u32 currentPos, u32 level)
{ 
	u32 endPos = bzetWalk(bzet,currentPos,level);
	if(endPos >= bzet.size())
	{
		endPos = bzet.size();
	}
	return endPos;
}

//TODO finish typing problems with this function
vector<bool> BinaryBzet::doTreeOp(string operation, u32 level, vector<bool> bzetA, u32& posA, vector<bool> bzetB, u32& posB)
{
	bool dr;
	int end = 0;
	if(operation.compare("DA0") == 0)
	{
		posA = bsDrop(bzetA,posA,level);
		dr = false;
	}
	else if(operation.compare("DA1") == 0)
	{
		posA = bsDrop(bzetA,posA,level);
		dr = true;
	}
	else if(operation.compare("DB0") == 0)
	{
		posB = bsDrop(bzetB,posB,level);
		dr = false;
	}
	else if(operation.compare("DB1") == 0)
	{
		posB = bsDrop(bzetB,posB,level);
		dr = true;
	}
	else if(operation.compare("CA") == 0)
	{
		//posA = bsCopy(bzetA,posA,level,end);
	}
	else if(operation.compare("CB") == 0)
	{
		//posB = bsCopy(bzetB,posB,level,end);
	}
	else if(operation.compare("NA") == 0)
	{
		//posA = bsNeg(bzetA,posA,level,end);
	}
	else if(operation.compare("NB") == 0)
	{
		//posB = bsNeg(bzetB,posB,level,end);
	}

	return bzetA;  // I just put this so it would compile for me, delete this when you fix it
}

//not sure if we need this function
vector<bool> BinaryBzet::doDataOp(string operation, vector<bool> data1, vector<bool> data2)
{
	vector<bool>::iterator itA;
	vector<bool>::iterator itB;
	vector<bool> bzetRet;
	
	itA = data1.begin();
	itB = data2.begin();

	if(operation.compare("&") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back((*itA) & (*itB));
			itB++;
		}
	}
	else if(operation.compare("|") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back((*itA) | (*itB));
			itB++;
		}
	}
	else if(operation.compare("^") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back((*itA) ^ (*itB));
			itB++;
		}
	}
	else if(operation.compare("~&") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back((~((*itA)&(*itB))));
			itB++;
		}
	}
	else if(operation.compare("~|") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back((~((*itA)|(*itB))));
			itB++;
		}
	}
	else if(operation.compare("~^") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back((~((*itA)^(*itB))));
			itB++;
		}
	}
	else if(operation.compare("0") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(0);
			itB++;
		}
	}
	else if(operation.compare("1") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(1);
			itB++;
		}
	}
	return bzetRet;
}

//NOT's current subtree in place
//implements _not_ in python code
u32 BinaryBzet::subtreeNot(vector<bool>& bzet, u32 currentPos, u32 level)
{
	u32 nextPos = currentPos + 2;
	switch(bzet[(currentPos - 1)] << 1 | bzet[currentPos]) {
		case 0:	// '0'
			bzet[(currentPos - 1)] = 1;
			bzet[currentPos] = 1;
		break;
	
		case 1: // 't'
			bzet[(currentPos - 1)] = 1;
			bzet[currentPos] = 0;	
		break;
	
		case 2: // 'T'
			if (level > 1) {
				// not left subtree
				nextPos = subtreeNot(bzet, nextPos, level - 1);
				// not right subtree
				nextPos = subtreeNot(bzet, nextPos, level - 1);
			}
			else {
				bzet[(currentPos - 1)] = 0;
				bzet[currentPos] = 1;	
			}
		break;
	
		case 3: // '1'
			bzet[(currentPos - 1)] = 0;
			bzet[currentPos] = 0;
		break;
	}

	return nextPos;
}

//returns end position of subtree starting at currentPos - ends exclusive
// michael - this seems to be working correctly except when the current position is given to something in level 1
//           since only passing in the current position and the level doesn't give any info on whether it's
//           refering to the left or right child. make sense?
u32 BinaryBzet::bzetWalk(vector<bool>& bzet, u32 currentPos, u32 currentLev) {  
	u32 *seen = new u32[currentLev+1];
	seen[currentLev] = 0;
	u32 top, next, bzetIndex = currentPos;
	stack<u32> s;
	s.push(currentLev);
    
    // travers all of the current subtree
	while (!s.empty() && bzetIndex < (u32)(bzet.size())) {
		top = s.top();
		seen[top]++;
		if (seen[top] == 2)
			s.pop();
        
		if (!s.empty()) {
            if (top != 1) {
                bool lBit = bzet[bzetIndex];
                bool rBit = bzet[bzetIndex+1];
                bitpair curBP = lBit ? (rBit ? '1' : 'T') : (rBit ?  't': '0');
                if (curBP == '1' || curBP == '0') {
                    delete [] seen;
                    return bzetIndex+2;
                }
                // set up next index level
                next = top - 1;
                s.push(next);
                seen[next] = 0;
            }
            bzetIndex += 2; // 'T' 't' '1' '0' take up two bits so skip both to get to the next..
		}
	}
    
	delete [] seen;
    
	return bzetIndex;
}

void BinaryBzet::bzetWalkTEST() {
	cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
         << "bzetWalkTEST():\n"
         << "--------------------------------------------------\n"
         << "\n"
         << "bzet should be TTTt1 so:\n\n";
    
    bool values[] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0 }; // TTTTTTttT10
    vector<bool> bzet;
    bzet.insert(bzet.begin(), values, values + 22);
    
    // display vector values
    cout << "bzet: ";
    for (size_t i = 0; i < bzet.size(); i++)
        cout << (bzet[i] ? '1' : '0');
    cout << "\n";
    
    assert(bzetWalk(bzet, 2, 3) == 16);
    cout << "Test 01: Passed\n";
    assert(bzetWalk(bzet, 4, 2) == 10);
    cout << "Test 02: Passed\n";
    assert(bzetWalk(bzet, 10, 2) == 16);
    cout << "Test 03: Passed\n";
    cout << "bzetWalk(bzet, 18, 2) = " << bzetWalk(bzet, 18, 2) << "\n";
    assert(bzetWalk(bzet, 18, 2) == 20);
    cout << "Test 04: Passed\n";
    cout << "\nAll tests Passed\n";
        
	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
}

//remove unneccessary upper levels that have zeros at the end of the bitset
vector<bool> BinaryBzet::normalize(vector<bool> bzet, u32 level)
{
	//TODO Finish
	return bzet;
}

//Aligns two bzets of different sizes
//if the depths are the same this function does nothing
void BinaryBzet::align(vector<bool>& bzetA, u32& depthA, vector<bool>& bzetB, u32& depthB)
{
	u32 changeLevel;
	vector<bool>::iterator it;

	//BzetA is bigger - need to make BzetB larger
	if(depthA > depthB)
	{
		changeLevel = depthA - depthB;

		//change bzetB's depth
		depthB = depthA;

		for(int i = 0; i < (int) changeLevel; i++)
		{
			// add changeLevel # of T(10)'s to the beginning of the Bzet
			it = bzetB.begin();
			bzetB.insert(it, 0);
			it = bzetB.begin();
			bzetB.insert(it,1);
			// add changeLevel # of 0(00)s to the end of the Bzet
			bzetB.push_back(0);
			bzetB.push_back(0);
		}
	}
	//BzetB is bigger - need to make BzetA smaller
	else if(depthB > depthA)
	{
		changeLevel = depthB - depthA;
		
		//change bzetA's depth
		depthA = depthB;
		
		for(int i = 0; i < (int) changeLevel; i++)
		{
			// add changeLevel # of T(10)'s to the beginning of the Bzet
			it = bzetA.begin();
			bzetA.insert(it, 0);
			it = bzetA.begin();
			bzetA.insert(it,1);
			// add changeLevel # of 0(00)s to the end of the Bzet
			bzetA.push_back(0);
			bzetA.push_back(0);
		}
	}
	//Bzet length already equal
	return;
}

//sets depth of a BinaryBzet
void BinaryBzet::setDepth(u32 newDepth)
{
	m_depth = newDepth;
}

// == operator for BinaryBzet
bool BinaryBzet::operator ==(const BinaryBzet& rhs)
{
	vector<bool> bzetB = rhs.m_bzet;

	return (equal( bzetB.begin(), bzetB.end(), this->m_bzet.begin()) && rhs.m_depth == this->m_depth);	
}

bool BinaryBzet::AlignCompare (const BinaryBzet& other) {
	vector<bool> bzetA = m_bzet;
	u32 depthA = m_depth;
	vector<bool> bzetB = other.m_bzet;
	u32 depthB = other.m_depth;
	align(bzetA, depthA, bzetB, depthB);
		
	return (equal( bzetB.begin(), bzetB.end(), bzetA.begin()) && depthB == depthA);	
}

void BinaryBzet::testShift () {
	bool passed = 1;

	for (int i=2; i<20; ++i)
		for (int j=2; j<20; ++j) {
			BinaryBzet a(i);
			a.shift(0);			// The constructor isn't creating an optimized bzet
			BinaryBzet b(j);
			b.shift(i - j);
	
			if (!a.AlignCompare(b)) {	// The shifting operation may not create an correctly normalized bzet
				cout << "Shifting Test (" << i << ", " << j << "): Failed!" << endl;
				passed = 0;
			}
		}

	{
		BinaryBzet a(0, 4, 2);
		a.shift(0);			// The constructor isn't creating an optimized bzet
		BinaryBzet b(1, 5, 2);
		b.shift(-1);
	
		if (!a.AlignCompare(b)) { // The shifting operation may not create an correctly normalized bzet
			cout << "Shifting Test: Failed!" << endl;
			passed = 0;
		}

	}

	if (passed)
		cout << "All shifting tests passed :D" << endl;
}
