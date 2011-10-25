#include "BinaryBzet.h"

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

void BinaryBzet::encodeLetter(vector<bool>& result, u32& resultIndex, u8 letter, u32 lettersToEncode)
{
	for (u32 i = 0; i < lettersToEncode; ++i)
	{
		result.push_back((letter >> 1) & 0x1);
		result.push_back(letter & 0x1);
	}
}

void BinaryBzet::encodeValue(vector<bool>& result, u32& resultIndex, u8 value, u32 valuesToEncode)
{
	int valuesLeftToEncode = valuesToEncode;
	while (valuesLeftToEncode > 0)
	{
		int numLeadingT = GetNumEndingZero(resultIndex, m_depth - 1);
		int numToEncode = 1;

		while (numLeadingT > 0 && 2 * numToEncode <= valuesLeftToEncode) 
		{
			numToEncode *= 2;
			numLeadingT--;
		}

		resultIndex += numToEncode / 2;

		encodeLetter(result, resultIndex, 2, numLeadingT);
		encodeLetter(result, resultIndex, value, 1);
	}
}

void BinaryBzet::encodeBits(bool value, int count, vector<bool>& result, bool& valueCounting, u32& bitsCounted, u32& resultIndex) 
{
	if (bitsCounted == 0)
	{
		valueCounting = value;
		bitsCounted = count;
	}
	else if (value == valueCounting)
	{
		bitsCounted += count;
		return;
	}
	else
	{
		if (bitsCounted % 2)
		{
			encodeValue(result, resultIndex, valueCounting ? 3 : 0, (bitsCounted - 1) / 2);

			if (valueCounting)
			{
				encodeValue(result, resultIndex, 2, 1);
			}
			else
			{
				encodeValue(result, resultIndex, 1, 1);
			}

			valueCounting = value;
			bitsCounted = count - 1;
		}
		else
		{
			encodeValue(result, resultIndex, valueCounting ? 3 : 0, bitsCounted / 2);

			valueCounting = value;
			bitsCounted = count;
		}
	}
}

void BinaryBzet::shift(int distance)
{
	return; // Does not work yet
	
	vector<bool> result;
	result.reserve(m_bzet.size());

	u32 resultIndex = 0;
	u32 bitsToSkip = 0;
	bool valueCounting = 0;
	u32 bitsCounted = 0;

	u32 bitstringIndex = 0;
	u32 bzetIndex = 0;

	// Need to calculate optimal depth for result bzet
	// using the wrong m_depth everywhere

	if (distance > 0) 
	{
		bitsCounted = distance;
		bitsToSkip = 0;
	}
	else
	{
		bitsToSkip = u32(distance);
	}

	// Decode the bitstring, and reencode at a starting index
	do {
		int numLeadingT = GetNumEndingZero(bitstringIndex, m_depth - 1);
		u32 bitsToEncode = 2 << numLeadingT;
		int value = getBzetIndex(bzetIndex);
		bzetIndex++;

		while (value == 0x2 && numLeadingT > 0)
		{
			value = getBzetIndex(bzetIndex);
			bzetIndex++;
			bitsToEncode /= 2;
			numLeadingT--;
		}

		bitstringIndex += bitsToEncode / 2;

		if (bitsToSkip > 0) 
		{
			if (bitsToSkip > bitsToEncode)
			{
				bitsToSkip -= bitsToEncode;
				continue;
			}
			else
			{
				bitsToEncode -= bitsToSkip;
				bitsToSkip = 0;
			}
		}

		if (bitsToEncode == 0)
			continue;

		switch (value) 
		{
		case 1:
			if (bitsToEncode == 1)
			{
				encodeBits(0, 1, result, valueCounting, bitsCounted, resultIndex);
			}
			else
			{
				encodeBits(0, 1, result, valueCounting, bitsCounted, resultIndex);
				encodeBits(1, 1, result, valueCounting, bitsCounted, resultIndex);
			}
			break;

		case 2:
			if (bitsToEncode == 1)
			{
				encodeBits(1, 1, result, valueCounting, bitsCounted, resultIndex);
			}
			else
			{
				encodeBits(1, 1, result, valueCounting, bitsCounted, resultIndex);
				encodeBits(0, 1, result, valueCounting, bitsCounted, resultIndex);
			}
			break;

		case 0:
			encodeBits(0, bitsToEncode, result, valueCounting, bitsCounted, resultIndex);
			break;

		case 3:
			encodeBits(1, bitsToEncode, result, valueCounting, bitsCounted, resultIndex);
			break;
		};

	} while (bitstringIndex < u32(1) << m_depth);

	// encode enough 0's at the end to fill in rest of bzet
	
	// copy result bzet into this bzet
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