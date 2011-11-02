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

u32 BinaryBzet::getLastBit () {
	u32 resultIndex = c_u32_max;		
	u32 bitstringIndex = 0;		
	u32 bzetIndex = 0;			

	do {
		int numLeadingT = GetNumEndingZero(bitstringIndex, m_depth - 1);
		u32 bitsFound = 2 << numLeadingT;
		int value = getBzetIndex(bzetIndex);
		bzetIndex++;

		while (value == 0x2 && numLeadingT > 0) {
			value = getBzetIndex(bzetIndex);
			bzetIndex++;
			bitsFound /= 2;
			numLeadingT--;
		}

		bitstringIndex += bitsFound / 2;

		if (value == 0)
			continue;

		// depending on the value, how far back was the last one bit
		//					    x, 01, 10, 11
		const u32 offset[4] = { 0, 1, 2, 1 };
		resultIndex = bitstringIndex * 2 - offset[value];

	} while (bitstringIndex < u32(1) << (m_depth - 1));

	return resultIndex;
}

void BinaryBzet::getLastBitTest () {
	for (int i = 0; i < 10; ++i) {
		u32 testNum = rand()%1000;
		BinaryBzet a(testNum);
		u32 lastBit = a.getLastBit();
		if (lastBit == c_u32_max) { // couldn't find any bit set
			;// count << "something seems to be messed up?";
		}
		else if (lastBit != testNum) {  // some bit was found, but it was wrong one
			cout << "getLastBit Failed! " << testNum<< endl;
		}
	}
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

void BinaryBzet::flip(u32 index) {
    bool currentBitValue;
	u32 *seen = new u32[m_depth+1];
	seen[m_depth] = 0;
    
	u32 currentLevel;
    u32 nextLevel;
    u32 bzetIndex = 0;
    u32 bitIndex = 0;
	
    stack<u32> s;
	s.push(m_depth);
	
    while (!s.empty() && bitIndex < (u32)(m_bzet.size())) {
		currentLevel = s.top();
		seen[currentLevel]++;
		if (seen[currentLevel] == 2)
			s.pop();
        
		if (!s.empty()) {
            bitpair curBP = bitpairFromBools(m_bzet[bzetIndex], m_bzet[bzetIndex+1]);
            if (currentLevel == 1) {
                // at level one
                if (index == bitIndex) {
                    currentBitValue = m_bzet[bzetIndex];
                    bitSet(index, !currentBitValue);
                    break;
                } else if (index == (bitIndex + 1)) {
                    currentBitValue = m_bzet[bzetIndex + 1];
                    bitSet(index, !currentBitValue);
                    break;
                }
                bitIndex += 2;
            } else if (curBP == '1' || curBP == '0') {
                u32 count = (u32)pow(2.0, (double)currentLevel);
                if (index >= bitIndex && index < (bitIndex + count)) {
                    currentBitValue = (curBP == '1' ? 1: 0);
                    bitSet(index, !currentBitValue);
                    break;
                }
                
            } else {
                // set up next index level
                nextLevel = currentLevel - 1;
                s.push(nextLevel);
                seen[nextLevel] = 0;
            }
            bzetIndex += 2; // 'T' 't' '1' '0' take up two bits so skip both to get to the next..
		}
	}
    
	delete [] seen;
}

// currently only traverses bzet
void BinaryBzet::bitSet(u32 index, bool value) {
	//    s: a stack to keep track of the level the current index in the array is for
	// seen: an array to keep track of the number of times a level has been seen.

	u32 bzetIndex = 0;
	u32 bitIndex = 0; // index bitstring if it had not been compressed to a bzet.

	stack<u32> s;
	u32 *seen = new u32[m_depth+1];

	u32 top = m_depth;
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
                newBP = bitpairByChangingIndexValue(curBP, 0, value);
				setBitPairAtBzetIndex(bzetIndex, newBP);
				curBP = newBP;
                
                // collapse 
                int parent, leftChild, rightChild;
                if (seen[top] == 1) {
                    // at left child
                    leftChild = bzetIndex*2; // = bitIndex;
                    parent = leftChild - 2;
                    rightChild = leftChild + 2;
                } else {
                    // at right child
                    rightChild = bzetIndex*2; // = bitIndex;
                    leftChild = rightChild - 2;
                    parent = leftChild - 2;
                }
                bitSetCollapse(m_bzet, parent, leftChild, rightChild);
                break; // done
			} else if (index == bitIndex + 1) { // change 2nd bit?
                newBP = bitpairByChangingIndexValue(curBP, 1, value);
				setBitPairAtBzetIndex(bzetIndex, newBP);
				curBP = newBP;
                
                // collapse 
                int parent, leftChild, rightChild;
                if (seen[top] == 1) {
                    // at left child
                    leftChild = bzetIndex*2; // = bitIndex;
                    parent = leftChild - 2;
                    rightChild = leftChild + 2;
                } else {
                    // at right child
                    rightChild = bzetIndex*2; // = bitIndex;
                    leftChild = rightChild - 2;
                    parent = leftChild - 2;
                }
                bitSetCollapse(m_bzet, parent, leftChild, rightChild);
                break; // done
			}
			bitIndex += 2;
		} else if (curBP == '0' || curBP == '1') {
			// there is 2^top 1's or 0's
			u32 size = (u32)pow(2.0, (double)top);
			if (index >= bitIndex && index < bitIndex + size) {
				// the bit is in this range
				vector<bool> expandTo;
				expand(expandTo, bitIndex, bitIndex + size - 1, index, value);
				m_bzet.erase(m_bzet.begin() + bzetIndex*2-1, m_bzet.begin() + bzetIndex*2 + 1);
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

void BinaryBzet::bitSetCollapse(vector<bool>& bzet, int& parentIndex, int& leftChildIndex, int& rightChildIndex) {
    if (parentIndex >= 0 && rightChildIndex+1 < bzet.size()) {
        bitpair lChild = bitpairFromBools(bzet[leftChildIndex], bzet[leftChildIndex+1]);
        bitpair rChild = bitpairFromBools(bzet[rightChildIndex], bzet[rightChildIndex+1]);
        if (lChild == rChild && (lChild == '1' || lChild == '0')) {
            // need to collapse
            vector<bool>::iterator itBegin = bzet.begin(); 
            bzet.erase(itBegin + parentIndex, itBegin + rightChildIndex); // erases first two values only.. "T11" is collased to "1" by erasing "T1"
            
            // recursive call to check if further collapsing is needed
            parentIndex-=2;
            leftChildIndex-=2;
            rightChildIndex-=2;
            bitSetCollapse(bzet, parentIndex, leftChildIndex, rightChildIndex);
        }
    }
}

void BinaryBzet::bitSetCollapseTEST() {
    int parentIndex = 4;
    int leftChildIndex = 6;
    int rightChildIndex = 8;
    
    //  input: TTT1111
    //           ^^^ collapse #1, now have: TT111
    //                                       ^^^ collapse #2, now have T11
    //                                                                 ^^^ collapse #3, now have: 1
    bool input1[] = { 1,0, 1,0, 1,0, 1,1, 1,1, 1,1, 1,1 }; // input
    
    // answer:
    bool answer1[] = { 1, 1 }; // result should be this
    vector<bool>v1;
    bool t1_passed = true;
    v1.insert(v1.begin(), input1, input1+14);
    bitSetCollapse(v1, parentIndex, leftChildIndex, rightChildIndex);
    for (size_t i = 0; i < v1.size(); i++) {
        if (i < 2) {
            if (v1[i] != answer1[i]) {
                t1_passed = false;
            }
        } else {
            t1_passed = false;
        }
    }
    assert(t1_passed);
    cout << "Test 1 Passed\n";
    
    
    //  input: TTTTTT1tTTt1TtT
    //              ^^^ should be collapsed
    bool  input2[] = { 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,1, 1,1, 1,0, 1,0, 0,1, 1,1, 1,0, 0,1, 1,0 };
    
    // answer: TTTTT1TTt1TtT
    //              ^ collapsed value
    bool answer2[] = { 1,0, 1,0, 1,0, 1,0, 1,0, 1,1, 1,0, 1,0, 0,1, 1,1, 1,0, 0,1, 1,0 }; // result should be this
    vector<bool>v2;
    bool t2_passed = true;
    v2.insert(v2.begin(), input2, input2+30);
    bitSetCollapse(v2, parentIndex, leftChildIndex, rightChildIndex);
    for (size_t i = 0; i < v1.size(); i++) {
        if (i < 26) {
            if (v2[i] != answer2[i]) {
                t2_passed = false;
            }
        } else {
            t2_passed = false;
        }
    }
    assert(t2_passed);
    cout << "Test 2 Passed\n";
    
    //  input: TT11TtT
    //          ^^^ should be collapsed
    bool  input3[] = { 1,0, 1,0, 1,1, 1,1, 1,0, 0,1, 1,0 };
    
    // answer: T1TtT
    //          ^ collapsed value
    bool answer3[] = { 1,0, 1,1, 1,0, 0,1, 1,1 };
    vector<bool>v3;
    bool t3_passed = true;
    v3.insert(v3.begin(), input3, input3+14);
    bitSetCollapse(v3, parentIndex, leftChildIndex, rightChildIndex);
    for (size_t i = 0; i < v1.size(); i++) {
        if (i < 10) {
            if (v2[i] != answer3[i]) {
                t3_passed = false;
            }
        } else {
            t3_passed = false;
        }
    }
    assert(t3_passed);
    cout << "Test 3 Passed\n";
    
}

bitpair BinaryBzet::bitpairFromBools(bool leftBit, bool rightBit) {
    return (leftBit ? (rightBit ? '1' : 'T') : (rightBit ? 't' : '0'));
}

// given a bit pair, modifies bit at index (0 or 1) to tovalue;
// bitpairByChangingIndexValue('T', 0, 0) --> T (10), 0th bit is change to a zero and becomes 0 (00)
bitpair BinaryBzet::bitpairByChangingIndexValue(bitpair currentBitPair, int index_0or1, bool toValue) {
    bool lBit, rBit;
    switch (currentBitPair) {
        case '0': lBit = 0; rBit = 0; break;
        case 't': lBit = 0; rBit = 1; break;
        case 'T': lBit = 1; rBit = 0; break;
        case '1': lBit = 1; rBit = 1; break;
        default:
            cout << "bitpairByChangingIndexValue: Unrecognized bitpair value!\n";
            return 'x';
    }
    
    if (index_0or1 == 0) {
        lBit = toValue;
    } else {
        rBit = toValue;
    }
    
    return lBit ? (rBit ? '1': 'T') : (rBit ? 't': '0');
}

void BinaryBzet::expandTEST() {
    vector<bool> bzet1;
    expand(bzet1, 0, 15, 1, false);
    bool t1[] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 };
    bool t1_passed = true;
    for (size_t i = 0; i < bzet1.size(); i++)
        if (bzet1[i] != t1[i]) {
            t1_passed = false;
            break;
        }
    assert(t1_passed);
    cout << "Expand Test 1 Passed\n";
    //-------------------------------------------------------
    
    vector<bool> bzet2;
    expand(bzet2, 0, 15, 15, false);
    bool t2[] = { 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
    bool t2_passed = true;
    for (size_t i = 0; i < bzet2.size(); i++)
        if (bzet2[i] != t2[i]) {
            t2_passed = false;
            break;
        }
    assert(t2_passed);
    cout << "Expand Test 2 Passed\n";
    //-------------------------------------------------------
    
    vector<bool> bzet3;
    expand(bzet3, 0, 15, 6, false);
    bool t3[] = { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 };
    bool t3_passed = true;
    for (size_t i = 0; i < bzet3.size(); i++)
        if (bzet3[i] != t3[i]) {
            t3_passed = false;
            break;
        }
    assert(t3_passed);
    cout << "Expand Test 3 Passed\n";
    //-------------------------------------------------------
    
    vector<bool> bzet4;
    expand(bzet4, 0, 15, 11, true);
    bool t4[] = { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0 };
    bool t4_passed = true;
    for (size_t i = 0; i < bzet4.size(); i++)
        if (bzet4[i] != t4[i]) {
            t4_passed = false;
            break;
        }
    assert(t4_passed);
    cout << "Expand Test 4 Passed\n";    
}

// value is the value to which the single bit is change so current bit values is !value
void BinaryBzet::expand(vector<bool> &newbzet, u32 start, u32 end, u32 bitLocation, bool value) {
	u32 midPoint = (start + end) / 2;
    u32 length = end - start + 1;
    if (length == 2) {
        if (bitLocation == start) {
            newbzet.push_back(value);
            newbzet.push_back(!value);
        } else {
            newbzet.push_back(!value);
            newbzet.push_back(value);
        }
    } else if (bitLocation >= start && bitLocation <= midPoint) {
        // push a T
        newbzet.push_back(1);
        newbzet.push_back(0);
        
        // left side of tree is changed
        expand(newbzet, start, midPoint, bitLocation, value);
        
        // right side of tree stays the same
        newbzet.push_back(!value);
        newbzet.push_back(!value);
    } else {
        // push a T
        newbzet.push_back(1);
        newbzet.push_back(0);
        
        // left side of tree stays the same
        newbzet.push_back(!value);
        newbzet.push_back(!value);
        
        // right side of tree changes
        expand(newbzet, midPoint+1, end, bitLocation, value);
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
void BinaryBzet::setTEST() {
    //     T
    //  T    1
    // T 1  1 1
    // 1011 1111
    //  ^
    // TTT11
    // after set(1) // turns on bit at index 1
    bool p1[] = { 1,0, 1,0, 1,0, 1,1, 1,1 };
    vector<bool>bzet;
    bzet.insert(bzet.begin(), p1, p1+10);
    m_bzet = bzet;
    m_depth = 3;
    set(1);
    assert(m_bzet.size() == 2 && m_bzet[0] == 1 && m_bzet[1] == 1);
    cout << "Test 1 Passed\n";
    
    // now unsetting the same bit should result in the original (p1)
    unset(1);
    bool t2_passed = true;
    for (int i = 0; i < m_bzet.size(); i++) {
        if (i < 10) {
            if (m_bzet[i] != p1[i]) {
                t2_passed = false;
            }
        } else {
            t2_passed = false;
            break;
        }        
    }
    assert(t2_passed);
    cout << "Test 2 Passed\n";
}

// temporary
void BinaryBzet::flipTEST() {
    //     T
    //  T    1
    // T 1  1 1
    // 1011 1111
    //  ^
    // TTT11
    // after set(1) // turns on bit at index 1
    bool p1[] = { 1,0, 1,0, 1,0, 1,1, 1,1 };
    vector<bool>bzet;
    bzet.insert(bzet.begin(), p1, p1+10);
    m_bzet = bzet;
    m_depth = 3;
    cout << "b_zet = ";
    for (size_t i = 0; i < m_bzet.size(); i++) {
        cout << m_bzet[i];
    }
    cout << "\n";
    flip(1);
    cout << "b_zet = ";
    for (size_t i = 0; i < m_bzet.size(); i++) {
        cout << m_bzet[i];
    }
    cout << "\n";
    assert(m_bzet.size() == 2 && m_bzet[0] == 1 && m_bzet[1] == 1);
    cout << "Test 1 Passed\n";
    
    // flipping again should get back to original (p1)
    flip(1);
    bool t2_passed = true;
    for (int i = 0; i < m_bzet.size(); i++) {
        if (i < 10) {
            if (m_bzet[i] != p1[i]) {
                t2_passed = false;
            }
        } else {
            t2_passed = false;
            break;
        }        
    }
    assert(t2_passed);
    cout << "Test 2 Passed\n";
}


/* 
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
	vector<bool> bzetRet = binaryOp(1,bzetA,0,bzetB,0,depthA);
	BinaryBzet b = BinaryBzet(&bzetRet,m_depth);
	//Used to collapse Bzet
	b.shift(0);
	return b;
}

//branchData == cdr python
//treeData == ddr python
vector<bool> BinaryBzet::binaryOp(int operationNo, vector<bool> bzetA, u32 posA, vector<bool> bzetB, u32 posB, u32 level)
{
	string* operation = g_binOp[operationNo];
	vector<bool> bzet1 = bzetA;
	vector<bool> bzet2 = bzetB; 
	vector<bool> resultBzet;

	u32 *seenA = new u32[level+1/* number of levels in tree + 1 so you can an index of the highest level */];
	stack<u32> sA; // for the levels
	sA.push(level/* the top level */);
	seenA[level/* the top level */] = 0;
	u32 currentLevelA = 0, nextA = 0, bzetIndexA = posA;

	u32 *seenB = new u32[level+1/* number of levels in tree + 1 so you can an index of the highest level */];
	stack<u32> sB; // for the levels
	sB.push(level/* the top level */);
	seenB[level/* the top level */] = 0;
	u32 currentLevelB = 0, nextB  = 0, bzetIndexB = posB;
   
	while (!sA.empty()) 
	{
		currentLevelA = sA.top();
		cout << "Current Level A: " << currentLevelA << "\n";
		currentLevelB = sB.top();
		cout << "Current Level B: " << currentLevelB << "\n";
		if(currentLevelA <= currentLevelB)
		seenA[currentLevelA]++;
		if(currentLevelB <= currentLevelA)
		seenB[currentLevelB]++;
		if (seenA[currentLevelA] == 2) {
			sA.pop(); // this will empty out the stack when you are done
		}
		if (seenB[currentLevelA] == 2) {
			sB.pop(); // this will empty out the stack when you are done
		}
		if (!sA.empty()){
		//Do operations on this level - only do if on the same level
		if(currentLevelA == currentLevelB)
		{
			if(currentLevelA == 1 && currentLevelB == 1)	{
				//bottom level do data operation directly
				vector<bool> data1;
				data1.push_back(bzet1[bzetIndexA]);
				data1.push_back(bzet1[bzetIndexA+1]);
				vector<bool> data2;
				data2.push_back(bzet2[bzetIndexB]);
				data2.push_back(bzet2[bzetIndexB+1]);
				vector<bool> res = doDataOp(operation[1], data1,data2);
				for(u32 i = 0; i<res.size(); i++)
				{
					resultBzet.push_back(res[i]);
				}
			}
			else	{
				//Determine case of data
				//TODO - create function to look up value
				bool lBit3 = bzet1[bzetIndexA];
				bool rBit3 = bzet1[bzetIndexA+1];
				bitpair bp1 = lBit3 ? ( rBit3 ? '1' : 'T') : ( rBit3 ?  't': '0');

				bool lBit4 = bzet2[bzetIndexB];
				bool rBit4 = bzet2[bzetIndexB+1];
				bitpair bp2 = lBit4 ? ( rBit4 ? '1' : 'T') : ( rBit4 ?  't': '0');
				//TODO - replace with use of case_type array
				int caseType = 6;
				if((bp1 == '1'|| bp1 == '0') && (bp2 == '1' || bp2 == '0'))	{
					caseType = 0;
				}
				else if(bp1 == '0' && bp2 == 'T')	{
					caseType = 1;
				}
				else if(bp1 == 'T' && bp2 == '0')	{
					caseType = 2;
				}
				else if(bp1 == '1' && bp2 == 'T')	{
					caseType = 3;
				}
				else if(bp1 == 'T' && bp2 == '1')	{
					caseType = 4;
				}
				else if(bp1 == 'T' && bp2 == 'T')	{
					caseType = 5;
				}
				//Invalid case type
				else	{
					cout << "Invalid Case Type\n";
				}
				cout << "Case Type" << caseType << "\n";
				//00 or 11 or 10 or 01
				//Do the operation directly
				if( caseType == 0)	{
					vector<bool> data1;
					data1.push_back(bzet1[bzetIndexA]);
					data1.push_back(bzet1[bzetIndexA+1]);
					vector<bool> data2;
					data2.push_back(bzet2[bzetIndexB]);
					data2.push_back(bzet2[bzetIndexB+1]);
					vector<bool> res = doDataOp(operation[1], data1,data2);
					for(u32 i = 0; i<res.size(); i++)
					{
						resultBzet.push_back(res[i]);
					}
				}
				//0T T0 1T or T1
				else if (caseType >= 1 && caseType <= 4)	{
					string oper = operation[caseType + 1];
					cout << "Oper" << oper << "\n";
					//python call cp1, cp2 ,tr = do_tree_op(opr, lev-1,bset1,cp1,bset2,cp2)
					vector<bool> res = doTreeOp(oper,currentLevelA,bzet1, bzetIndexA, bzet2, bzetIndexB);
					//apped res to resultBzet + handle compression possibly
					for(u32 i = 0; i<res.size(); i++)
					{
						resultBzet.push_back(res[i]);
					}
				}
				//TT
				else if(caseType ==5)	{
					//Push a T to the resultBzet
					//TODO fix collapsing problem
					resultBzet.push_back(1);
					resultBzet.push_back(0);
				}
			}
		}
	}

		if (!sA.empty()) {		
			bool lBit = bzet1[bzetIndexA];
			bool rBit = bzet1[bzetIndexA+1];
			bitpair curBPA = lBit ? ( rBit ? '1' : 'T') : ( rBit ?  't': '0');
			cout << "curBPA: " << curBPA << " Index: "<< bzetIndexA <<"\n"; 

			bool lBit2 = bzet2[bzetIndexB];
			bool rBit2 = bzet2[bzetIndexB+1];
			bitpair curBPB = lBit2 ? ( rBit2 ? '1' : 'T') : ( rBit2 ?  't': '0');
			cout << "curBPB: " << curBPB <<" Index: "<< bzetIndexB << "\n"; 


			//Traverse A
			if (currentLevelA == 1) {
				// at level one
			
			} else if (curBPA == '1' || curBPA == '0') {
				// if you get here it means you are not at level 1 but the value in the tree is 1 or 0
				 // # of 1's or 0's represented by curBP is 2^(currentLevel)
				if (seenA[currentLevelA] == 2)
				sA.pop();
				// if the stack is empty you have reached the end of the bzet so break?
				//     (not sure what would work for you so im just leaving a comment)
				// if its not empty let while loop continue to next iteration
				//     if you call "continue;" make sure to call "bzetIndex += 2;" before hand to increment index
			} else if(currentLevelA <= currentLevelB)	{
					// set up next iteration
					nextA = currentLevelA - 1;
					sA.push(nextA);
					seenA[nextA] = 0;
			}

			//TraverseB
			if (currentLevelB == 1 /* && currentLevelB == 1*/) {
				// at level one
			
			} else if (curBPB == '1' || curBPB == '0') {
				// if you get here it means you are not at level 1 but the value in the tree is 1 or 0
				// # of 1's or 0's represented by curBP is 2^(currentLevel)
				if (seenB[currentLevelB] == 2)
				sB.pop();
				// if the stack is empty you have reached the end of the bzet so break?
				//     (not sure what would work for you so im just leaving a comment)
				// if its not empty let while loop continue to next iteration
				//     if you call "continue;" make sure to call "bzetIndex += 2;" before hand to increment index
			} else if(currentLevelB <= currentLevelA)	{
					// set up next iteration
					nextB = currentLevelB - 1;
					sB.push(nextB);
					seenB[nextB] = 0;
			}
				if(currentLevelA <= currentLevelB)
					bzetIndexA += 2; // its +2 because values are represented by 2 bits in the vector
				if(currentLevelB <= currentLevelA)
					bzetIndexB += 2;
		}

	}

	// release memory
	delete [] seenA;
	delete [] seenB;
	return resultBzet;
}

//implements CA and CB
//TODO remove endPos
vector<bool> BinaryBzet::bsCopy(vector<bool> bzet, u32 currentPos, u32 level, u32& endPos)
{
	vector<bool> returnBzet;
	if(currentPos >= (u32)bzet.size())
	{
		endPos = bzet.size();
		return returnBzet;
	}
	else
	{
		endPos = bzetWalk(bzet,currentPos,level);
	}

	//gets copy of subtree at positions currentPos-endPos
	for(u32 i = currentPos; i<endPos; i++)
	{
		returnBzet.push_back(bzet.at(i));
	}
	return returnBzet;
}

//implements NA and NB
//TODO remove endPos
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
		endPos = bzetWalk(bzet,currentPos,level);
	}
	//gets copy of subtree at positions currentPos-endPos
	for(u32 i = currentPos; i<endPos; i++)
	{
		returnBzet.push_back(bzet.at(i));
	}
	//not the subtree in place
	subtreeNot(returnBzet, 0, level);
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
vector<bool> BinaryBzet::doTreeOp(string operation, u32 level, vector<bool> bzetA, u32 posA, vector<bool> bzetB, u32 posB)
{
	bool dr;
	u32 end = 0;
	vector<bool> bzetRet;
	//TODO find out difference if any between DA0, DA1, DB0, DB1 are
	if(operation.compare("DA0") == 0)
	{
//		posA = bsDrop(bzetA,posA,level);
//		dr = false;
		bzetRet.push_back(0);
		bzetRet.push_back(0);
	}
	else if(operation.compare("DA1") == 0)
	{
//		posA = bsDrop(bzetA,posA,level);
//		dr = true;
		cout << "OP DA1\n";
		bzetRet.push_back(1);
		bzetRet.push_back(1);
	}
	else if(operation.compare("DB0") == 0)
	{
		bzetRet.push_back(0);
		bzetRet.push_back(0);
//		posB = bsDrop(bzetB,posB,level);
//		dr = false;
	}
	else if(operation.compare("DB1") == 0)
	{
		bzetRet.push_back(1);
		bzetRet.push_back(1);
//		posB = bsDrop(bzetB,posB,level);
//		dr = true;
	}
	else if(operation.compare("CA") == 0)
	{
		bzetRet = bsCopy(bzetA,posA,level,end);
	}
	else if(operation.compare("CB") == 0)
	{
		bzetRet = bsCopy(bzetB,posB,level,end);
	}
	else if(operation.compare("NA") == 0)
	{
		bzetRet = bsNeg(bzetA,posA,level,end);
	}
	else if(operation.compare("NB") == 0)
	{
		bzetRet = bsNeg(bzetB,posB,level,end);
	}

	return bzetRet;
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
			bzetRet.push_back(0);
			itB++;
		}
	}
	else if(operation.compare("1") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(1);
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


void BinaryBzet::traversalSkeleton(vector<bool> bzet, u32 level) {
	// initialize
	u32 *seen = new u32[level + 1/* number of levels in tree + 1 so you can an index of the highest level */];
	stack<u32> s; // for the levels
	s.push(level/* the top level */);
	seen[level/* the top level */] = 0;
	 u32 currentLevel, next, bzetIndex = 0;
//    
	while (!s.empty()) {
		currentLevel = s.top();
		cout << "Current Level" << currentLevel << "\n";
		seen[currentLevel]++;
		if (seen[currentLevel] == 2) {
			cout << "Removing Current Level from Stack\n";
			s.pop(); // this will empty out the stack when you are done
		}
//        
		if (!s.empty()) {
			// get value in bzet
			bool lBit = bzet[bzetIndex];   // bzet of your choice..
			bool rBit = bzet[bzetIndex+1]; // bzet of your choice..
			 bitpair curBP = (lBit ? (rBit ? '1' : 'T') : (rBit ?  't': '0'));
			cout << curBP << "\n";
			if (currentLevel == 1) {
				// at level one
			} else if (curBP == '1' || curBP == '0') {
				// if you get here it means you are not at level 1 but the value in the tree is 1 or 0
				// # of 1's or 0's represented by curBP is 2^(currentLevel)
				if (seen[currentLevel] == 2)
				s.pop();
				// if the stack is empty you have reached the end of the bzet so break?
				//     (not sure what would work for you so im just leaving a comment)
				// if its not empty let while loop continue to next iteration
				//     if you call "continue;" make sure to call "bzetIndex += 2;" before hand to increment index
			} else {
				// set up next iteration
				next = currentLevel - 1;
				s.push(next);
				seen[next] = 0;
			}
			bzetIndex += 2; // its +2 because values are represented by 2 bits in the vector
		}
	}
	// release memory
	delete [] seen;
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
    assert(bzetWalk(bzet, 18, 2) == 20);
    cout << "Test 04: Passed\n";
    cout << "\nAll tests Passed\n";
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
