#include <fstream>
#include <iostream>
#include "..\Binary Bzet\BinaryBzet.h"
#include <string>
#include <sstream>
#include <time.h>
using namespace std;

char* ReadFile(const string& fileName);
bool isItSet(const std::vector<uint>& vec, uint index);
void addImages(string file1, string file2);
void invertImage(string file1);
void cutoutImage(string file1, string file2);
void overlapImages(string file1, string file2);

uint startingindex = 0;

int main()
{
	string file1, file2, operation;
	cout << "Name of first image: ";
	cin >> file1;
	cout << "Operation? (add, invert, cutout, overlap): ";
	cin >> operation;

	if(operation!="invert"){
	cout << "Name of second image: ";
	cin >> file2;}
	
	clock_t start = clock();

	cout << "Working...";

	if(operation=="add")		addImages(file1, file2);
	else if(operation=="invert")		invertImage(file1);
	else if(operation=="cutout")		cutoutImage(file1, file2);
	else if(operation=="overlap")	overlapImages(file1, file2);
	else cout << "Incorrect Operation!" << endl;

	cout << endl << "Finished outputting in: " << ((double)clock() - start ) / CLOCKS_PER_SEC << " seconds." << endl;
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

void addImages(string file1, string file2)
{
	char* data1 = ReadFile(file1+".bmp");
	char* data2 = ReadFile(file2+".bmp");

	unsigned int dataBegin1 = *((unsigned int*)(data1+10));
	unsigned int width1 = *((unsigned int*)(data1+18));
	unsigned int height1 = *((unsigned int*)(data1+22));
	unsigned int bpp1 = *((unsigned int*)(data1+28));
	unsigned int iwidth1 = bpp1 * width1 / 8;
	unsigned int pixsize1 = iwidth1*height1;
	string bitstring1 = "";

	unsigned int dataBegin2 = *((unsigned int*)(data2+10));
	unsigned int width2 = *((unsigned int*)(data2+18));
	unsigned int height2 = *((unsigned int*)(data2+22));
	unsigned int bpp2 = *((unsigned int*)(data2+28));
	unsigned int iwidth2 = bpp2 * width2 / 8;
	unsigned int pixsize2 = iwidth2*height2;
	string bitstring2 = "";

	for(uint h=0;h<height1;h++)
		for(uint r=0;r<iwidth1;r+=3)
			if(data1[dataBegin1+h*iwidth1+r] == 0)
				bitstring1 += "0";
			else
				bitstring1 += "1";

	for(uint h=0;h<height2;h++)
		for(uint r=0;r<iwidth2;r+=3)
			if(data2[dataBegin2+h*iwidth2+r] == 0)
				bitstring2 += "0";
			else
				bitstring2 += "1";

	BinaryBzet bzet1(bitstring1+"#");
	BinaryBzet bzet2(bitstring2+"#");

	BinaryBzet result = bzet1&bzet2;

	ofstream os;
	os.open(file1+"ADDED"+file2+".bmp",std::ios_base::trunc);

	uint i = 0;
	for(i;i<dataBegin1;i++)
		os << data1[i];

	vector<uint> indexlist = result.bitList();
	char temp;

	for(i=0;i<pixsize1/3;i++){
		temp = (char)(-isItSet(indexlist, i));	// -1 for black, 0 for white
		os << temp << temp << temp;				// 3 bytes per pixel (BGR)
	}

	os.close();

	delete data1;
	delete data2;
}

void invertImage(string file1)
{
	char* data1 = ReadFile(file1+".bmp");

	unsigned int dataBegin1 = *((unsigned int*)(data1+10));
	unsigned int width1 = *((unsigned int*)(data1+18));
	unsigned int height1 = *((unsigned int*)(data1+22));
	unsigned int bpp1 = *((unsigned int*)(data1+28));
	unsigned int iwidth1 = bpp1 * width1 / 8;
	unsigned int pixsize1 = iwidth1*height1;
	string bitstring1 = "";

	for(uint h=0;h<height1;h++)
		for(uint r=0;r<iwidth1;r+=3)
			if(data1[dataBegin1+h*iwidth1+r] == 0)
				bitstring1 += "0";
			else
				bitstring1 += "1";


	BinaryBzet bzet1(bitstring1+"#");
	BinaryBzet bzet2((uint)0,bitstring1.length());

	BinaryBzet result = bzet1^bzet2;

	ofstream os;
	os.open(file1+"INVERTED.bmp",std::ios_base::trunc);

	uint i = 0;
	for(i;i<dataBegin1;i++)
		os << data1[i];

	vector<uint> indexlist = result.bitList();
	char temp;

	for(i=0;i<pixsize1/3;i++){
		temp = (char)(-isItSet(indexlist, i));	// -1 for black, 0 for white
		os << temp << temp << temp;				// 3 bytes per pixel (BGR)
	}

	os.close();

	delete data1;
}

void cutoutImage(string file1, string file2)
{
	char* data1 = ReadFile(file1+".bmp");
	char* data2 = ReadFile(file2+".bmp");

	unsigned int dataBegin1 = *((unsigned int*)(data1+10));
	unsigned int width1 = *((unsigned int*)(data1+18));
	unsigned int height1 = *((unsigned int*)(data1+22));
	unsigned int bpp1 = *((unsigned int*)(data1+28));
	unsigned int iwidth1 = bpp1 * width1 / 8;
	unsigned int pixsize1 = iwidth1*height1;
	string bitstring1 = "";

	unsigned int dataBegin2 = *((unsigned int*)(data2+10));
	unsigned int width2 = *((unsigned int*)(data2+18));
	unsigned int height2 = *((unsigned int*)(data2+22));
	unsigned int bpp2 = *((unsigned int*)(data2+28));
	unsigned int iwidth2 = bpp2 * width2 / 8;
	unsigned int pixsize2 = iwidth2*height2;
	string bitstring2 = "";

	for(uint h=0;h<height1;h++)
		for(uint r=0;r<iwidth1;r+=3)
			if(data1[dataBegin1+h*iwidth1+r] == 0)
				bitstring1 += "0";
			else
				bitstring1 += "1";

	for(uint h=0;h<height2;h++)
		for(uint r=0;r<iwidth2;r+=3)
			if(data2[dataBegin2+h*iwidth2+r] == 0)
				bitstring2 += "0";
			else
				bitstring2 += "1";

	BinaryBzet bzet1(bitstring1+"#");
	BinaryBzet bzet2(bitstring2+"#");
	BinaryBzet bzet3((uint)0,bitstring1.length());

	BinaryBzet result = bzet1|(bzet2^bzet3);

	ofstream os;
	os.open(file1+"CUTOUT"+file2+".bmp",std::ios_base::trunc);

	uint i = 0;
	for(i;i<dataBegin1;i++)
		os << data1[i];

	vector<uint> indexlist = result.bitList();
	char temp;

	for(i=0;i<pixsize1/3;i++){
		temp = (char)(-isItSet(indexlist, i));	// -1 for black, 0 for white
		os << temp << temp << temp;				// 3 bytes per pixel (BGR)
	}

	os.close();

	delete data1;
	delete data2;
}

void overlapImages(string file1, string file2)
{
	char* data1 = ReadFile(file1+".bmp");
	char* data2 = ReadFile(file2+".bmp");

	unsigned int dataBegin1 = *((unsigned int*)(data1+10));
	unsigned int width1 = *((unsigned int*)(data1+18));
	unsigned int height1 = *((unsigned int*)(data1+22));
	unsigned int bpp1 = *((unsigned int*)(data1+28));
	unsigned int iwidth1 = bpp1 * width1 / 8;
	unsigned int pixsize1 = iwidth1*height1;
	string bitstring1 = "";

	unsigned int dataBegin2 = *((unsigned int*)(data2+10));
	unsigned int width2 = *((unsigned int*)(data2+18));
	unsigned int height2 = *((unsigned int*)(data2+22));
	unsigned int bpp2 = *((unsigned int*)(data2+28));
	unsigned int iwidth2 = bpp2 * width2 / 8;
	unsigned int pixsize2 = iwidth2*height2;
	string bitstring2 = "";

	for(uint h=0;h<height1;h++)
		for(uint r=0;r<iwidth1;r+=3)
			if(data1[dataBegin1+h*iwidth1+r] == 0)
				bitstring1 += "0";
			else
				bitstring1 += "1";

	for(uint h=0;h<height2;h++)
		for(uint r=0;r<iwidth2;r+=3)
			if(data2[dataBegin2+h*iwidth2+r] == 0)
				bitstring2 += "0";
			else
				bitstring2 += "1";

	BinaryBzet bzet1(bitstring1+"#");
	BinaryBzet bzet2(bitstring2+"#");

	BinaryBzet result = bzet1|bzet2;

	ofstream os;
	os.open(file1+"OVERLAP"+file2+".bmp",std::ios_base::trunc);

	uint i = 0;
	for(i;i<dataBegin1;i++)
		os << data1[i];

	vector<uint> indexlist = result.bitList();
	char temp;

	for(i=0;i<pixsize1/3;i++){
		temp = (char)(-isItSet(indexlist, i));	// -1 for black, 0 for white
		os << temp << temp << temp;				// 3 bytes per pixel (BGR)
	}

	os.close();

	delete data1;
	delete data2;
}