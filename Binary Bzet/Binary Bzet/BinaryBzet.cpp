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