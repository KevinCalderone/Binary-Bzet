//#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"
#include <sstream>
#include <time.h>
#include <fstream>

int main()
{
	/*
	BinaryBzet a(0);

	BinaryBzet::getLastBitTest ();
	a.flipTEST();           // temporary
	a.setTEST();            // temporary
    a.expandTEST();         // temporary
    a.bzetWalkTEST();       // temporary
    a.bitSetCollapseTEST(); // tempporary
	BinaryBzet::testShift();
	*/
///////*
//////	cout << "NANA NANA NANA NANA BATMAAAAAAAN!" << endl;
//////	cout << endl <<
//////"               _,    _   _    ,_ " << endl <<
//////"          .o888P     Y8o8Y     Y888o. " << endl <<
//////"         d88888      88888      88888b " << endl <<
//////"        d888888b_  _d88888b_  _d888888b " << endl <<
//////"        8888888888888888888888888888888 " << endl <<
//////"        8888888888888888888888888888888 " << endl <<
//////"        YJGS8P'Y888P'Y888P'Y888P'Y8888P " << endl <<
//////"         Y888   '8'   Y8P   '8'   888Y " << endl <<
//////"          '8o          V          o8' " << endl <<
//////"            `                     ` " << endl << endl;
//////*/
//////	BinaryBzet a;
//////	vector<bool> data1;
//////	data1.push_back(1);	data1.push_back(0);
//////	data1.push_back(1);	data1.push_back(0);
//////	data1.push_back(1);	data1.push_back(1);
//////	data1.push_back(1);	data1.push_back(0);
//////	data1.push_back(1);	data1.push_back(0);
//////	data1.push_back(0);	data1.push_back(1);
//////	data1.push_back(0);	data1.push_back(1);
////////	data1.push_back(0);	data1.push_back(1);
//////
//////	vector<bool> data2;
////////	data2.push_back(1);	data2.push_back(0);
////////	data2.push_back(0);	data2.push_back(0);
////////	data2.push_back(1);	data2.push_back(0);
////////	data2.push_back(1);	data2.push_back(0);
////////	data2.push_back(0);	data2.push_back(0);
//////	data2.push_back(1);	data2.push_back(0);
//////	data2.push_back(0);	data2.push_back(0);
//////	data2.push_back(1);	data2.push_back(0);
//////	data2.push_back(1);	data2.push_back(0);
//////	data2.push_back(1);	data2.push_back(1);
//////	
//////	vector<bool> result = a.binaryOp(11,data2,0,data1,0,3);
//////	 
////////	vector<bool> result = a.doDataOp( "~&", data1, data2);
//////	vector<bool>::iterator it;
//////	for(it = result.begin(); it != result.end(); it++)
//////	{
//////		cout<<*it;
//////		it++;
//////		cout<<*it<<endl;
//////	}

	/*string line1;
	ifstream myfile1 ("bitstringexample1.txt");
	if (myfile1.is_open())
	{
		while ( myfile1.good() )
			getline (myfile1,line1);
		myfile1.close();
	}

	string line2;
	ifstream myfile2 ("bitstringexample2.txt");
	if (myfile2.is_open())
	{
		while ( myfile2.good() )
			getline (myfile2,line2);
		myfile2.close();
	}*/

	//BinaryBzet O(line1+"#");
	//BinaryBzet A(line2+"#");
	
//	BinaryBzet O("01 01 01 11 01 01 01 00 00 10 10 10 11 01 01 00#");
//	BinaryBzet A("01 01 01 00 01 11 00 00 00 00 00 00 00 00 00 00#");
//	BinaryBzet O("0101011101010100001010101101010#");
//	BinaryBzet A("0101010001110#");
	BinaryBzet O(0);
	O.rightShift((uint)1<<61);
	
	cout << "IN " << sizeof(size_t)*8 << " BIT MODE!" << endl << endl << endl;


	cout << O.getBzetString() << endl;
	//cout << A.getBzetString() << endl;
	//BinaryBzet anded = O & A;

	//std::cout << O.getBzetString() << std::endl;
	
	cout << endl;
	system("pause");
    return 0;
}