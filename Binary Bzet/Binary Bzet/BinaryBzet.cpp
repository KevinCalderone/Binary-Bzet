#include "BinaryBzet.h"
#include <stack>
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

string BinaryBzet::getBzetString(){
	//return m_bzet_string;

	//test:TTT1TTTTT0tT10 TTTt000
	/*bool myints[] = {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,};
    vector<bool> m_bzet_new (myints, myints + sizeof(myints) / sizeof(bool) );
	m_bzet=m_bzet_new;
    m_depth = 5;*/

	ostringstream output;
	output<< m_depth<<":";
	uint64_t indexB;
	for(indexB = 0 ; indexB < m_bzet.size()/2; indexB++){
		output<<getCharFromBzet(indexB);
	}
	return output.str();
}

string 	BinaryBzet::getBzetPretty(){	//get pretty formatted Bzet
	//test:TTT1TTTTT0tT10 TTTt000
	/*bool myints[] = {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,};
    vector<bool> m_bzet_new (myints, myints + sizeof(myints) / sizeof(bool) );
	m_bzet=m_bzet_new;
    m_depth = 5;*/

	ostringstream oss;
	oss << m_depth<<": ";
	string output = oss.str();
	uint64_t indexB=1;
	if(m_depth > 0){
		output[2] = getCharFromBzet(0);
		output += getBzetPrettyRecursive(m_depth-1,indexB);
	}
	return output;

}// end getBzetPRetty()

string BinaryBzet::getBzetPrettyRecursive(int level, uint64_t& indexB){
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

u8 BinaryBzet::getBzetIndex(u32 index){
	return (m_bzet[index * 2] ? 1 : 0) << 1 | (m_bzet[index * 2 + 1] ? 1 : 0);
}


/*return character in Bzet at indexB, return 'E' if indexB is out of bound.*/
char BinaryBzet::getCharFromBzet(uint64_t  indexB){
	
	if((indexB*2+1) >= m_bzet.size()) return 'E';
	
	bool firstBit = m_bzet.at(indexB*2);
	bool secondBit = m_bzet.at(indexB*2+1);

	if(!firstBit && !secondBit) return '0';
    else if(!firstBit && secondBit) return 't';
    else if(firstBit && !secondBit) return 'T';
    else if(firstBit && secondBit) return '1';
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
	int curPos = 0;
	vector<bool> bzetRet = binaryOp(1,bzetA,1,bzetB,1,depthA,flag,curPos);
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
vector<bool> BinaryBzet::binaryOp(int operationNo, vector<bool> bzetA, int posA, vector<bool> bzetB, int posB, int level, int& f, int& currentPos)
{
	string* operation = g_binOp[operationNo];
	vector<bool> bzet1 = bzetA;
	int currentPos1 = posA; 
	vector<bool> bzet2 = bzetB; 
	int currentPos2 = posB;
	
	int branchData = 0;
	int treeData = 0;

	return bzetA;
}

//implements CA and CB
//TODO test
vector<bool> BinaryBzet::bsCopy(vector<bool> bzet, int currentPos, int level, int& endPos)
{
	if(currentPos >= bzet.size())
	{
		endPos = bzet.size();
	}
	else
	{
		endPos = bzetWalk(bzet,currentPos,level);
	}
	vector<bool> returnBzet;
	//gets copy of subtree at positions currentPos-endPos
	for(int i = currentPos; i<endPos; i++)
	{
		returnBzet.push_back(bzet.at(i));
	}
	return returnBzet;
}

//implements NA and NB
//TODO finish/test
vector<bool> BinaryBzet::bsNeg(vector<bool> bzet, int currentPos, int level, int& endPos)
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
	for(int i = currentPos; i<endPos; i++)
	{
		returnBzet.push_back(bzet.at(i));
	}
	//not the subtree in place
	subtreeNot(returnBzet, 1, level);
	return returnBzet;
}

//implements DA and DB
//returns position of next subtree
int BinaryBzet::bsDrop(vector<bool> bzet, int currentPos, int level)
{
	//TODO finish
	int endPos = bzetWalk(bzet,currentPos,level);
	if(endPos >= bzet.size())
	{
		endPos = bzet.size();
	}
	return endPos;
}

//TODO finish typing problems with this function
vector<bool> BinaryBzet::doTreeOp(string operation, int level, vector<bool> bzetA, int& posA, vector<bool> bzetB, int& posB)
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

	return m_bzet;
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
			bzetRet.push_back(~((*itA)&(*itB)));
			itB++;
		}
	}
	else if(operation.compare("~|") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(~((*itA)|(*itB)));
			itB++;
		}
	}
	else if(operation.compare("~^") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(~((*itA)^(*itB)));
			itB++;
		}
	}
	else if(operation.compare("0") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(1);
			itB++;
		}
	}
	else if(operation.compare("1") == 0)
	{
		for(itA; itA != data1.end(); itA++)
		{
			bzetRet.push_back(0);
			itB++;
		}
	}
	return bzetRet;
}

//NOT's current subtree in place
//implements _not_ in python code
void BinaryBzet::subtreeNot(vector<bool>& bzet, int currentPos, int level)
{
	//TODO finish
	return;
}

//returns end position of subtree starting at currentPos - ends exclusive
int BinaryBzet::bzetWalk(vector<bool> bzet, int currentPos, int currentLev)
{
	//TODO finish
	return 0;
}

//remove unneccessary upper levels that have zeros at the end of the bitset
vector<bool> BinaryBzet::normalize(vector<bool> bzet, int level)
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
	return (equal( bzetB.begin(), bzetB.end(), this->m_bzet.begin()) && (rhs.m_depth == this->m_depth));	
}

