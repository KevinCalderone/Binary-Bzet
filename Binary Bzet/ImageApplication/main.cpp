#include <fstream>
#include <iostream>
#include "..\Binary Bzet\BinaryBzet.h"
#include <string>
#include <sstream>
#include <time.h>
using namespace std;

char* ReadFile(const string& fileName);
bool isItSet(const std::vector<uint>& vec, uint index);

uint startingindex = 0;

int main()
{
	string filename;
	cout << "Enter Name of first Image File: "; // don't include .bmp! Right now I have testpicR
	cin >> filename;
	char* data = ReadFile(filename+".bmp");
	unsigned int dataBegin = *((unsigned int*)(data+10));
	unsigned int width = *((unsigned int*)(data+18));
	unsigned int height = *((unsigned int*)(data+22));
	unsigned int bpp = *((unsigned int*)(data+28));
	unsigned int iwidth = bpp * width / 8;
	unsigned int pixsize = iwidth*height;

	ofstream myfile;
	string bitstring = "";
	
	myfile.open(filename+"bitstring.txt");		// just for reference

	for(uint h=0;h<height;h++){
		for(uint r=0;r<iwidth;r+=3){
			if(data[dataBegin+h*iwidth+r] == 0){
				myfile << '0';
				bitstring += "0";}
			else{
				myfile << '1';
				bitstring += "1";}}
		myfile << '\n';}

	myfile.close();

	BinaryBzet bzet(bitstring+"#");

	ofstream os;
	os.open(filename+"out.bmp",std::ios_base::trunc);

	uint i = 0;
	for(i;i<dataBegin;i++){
		os << data[i];}

	vector<uint> indexlist = bzet.bitList();
	char temp;

	clock_t start = clock();

	for(i=0;i<pixsize/3;i++){
		temp = (char)(-isItSet(indexlist, i));	// -1 for black, 0 for white
		os << temp << temp << temp;				// 3 bytes per pixel (BGR)
	}

	cout << "Finished outputting in: " << ((double)clock() - start ) / CLOCKS_PER_SEC << " seconds." << endl;

	os.close();

	system("pause");
}


char* ReadFile (const string& fileName) {
	ifstream is;
	is.open (fileName.c_str(), ios::binary);

	if( !is.is_open() ) {
		cout << "Error loading File: " << fileName << endl;
		return NULL;
	}

	int length;
	char* data;
	
	is.seekg (0, ios::end);
	length = is.tellg();
	is.seekg (0, ios::beg);

	data = new char [length];

	is.read (data,length);
	is.close();

	return data;
}

bool isItSet(const std::vector<uint>& vec, uint index)
{
	for(uint i=startingindex; i<vec.size(); i++)
		if(vec.at(i) == index){
			startingindex = i+1;
			return true;}
	return false;
}
