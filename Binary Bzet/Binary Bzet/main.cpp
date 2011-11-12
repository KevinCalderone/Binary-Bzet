//#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"
#include <sstream>
#include <time.h>

int main()
{
/*
	cout << "NANA NANA NANA NANA BATMAAAAAAAN!" << endl;
	cout << endl <<
"               _,    _   _    ,_ " << endl <<
"          .o888P     Y8o8Y     Y888o. " << endl <<
"         d88888      88888      88888b " << endl <<
"        d888888b_  _d88888b_  _d888888b " << endl <<
"        8888888888888888888888888888888 " << endl <<
"        8888888888888888888888888888888 " << endl <<
"        YJGS8P'Y888P'Y888P'Y888P'Y8888P " << endl <<
"         Y888   '8'   Y8P   '8'   888Y " << endl <<
"          '8o          V          o8' " << endl <<
"            `                     ` " << endl << endl;
*/
	BinaryBzet a;
	vector<bool> data1;
	data1.push_back(1);	data1.push_back(0);
	data1.push_back(1);	data1.push_back(0);
	data1.push_back(1);	data1.push_back(1);
	data1.push_back(1);	data1.push_back(0);
	data1.push_back(1);	data1.push_back(0);
	data1.push_back(0);	data1.push_back(1);
	data1.push_back(0);	data1.push_back(1);
//	data1.push_back(0);	data1.push_back(1);

	vector<bool> data2;
//	data2.push_back(1);	data2.push_back(0);
//	data2.push_back(0);	data2.push_back(0);
//	data2.push_back(1);	data2.push_back(0);
//	data2.push_back(1);	data2.push_back(0);
//	data2.push_back(0);	data2.push_back(0);
	data2.push_back(1);	data2.push_back(0);
	data2.push_back(0);	data2.push_back(0);
	data2.push_back(1);	data2.push_back(0);
	data2.push_back(1);	data2.push_back(0);
	data2.push_back(1);	data2.push_back(1);
	
	vector<bool> result = a.binaryOp(11,data2,0,data1,0,3);
	 
//	vector<bool> result = a.doDataOp( "~&", data1, data2);
	vector<bool>::iterator it;
	for(it = result.begin(); it != result.end(); it++)
	{
		cout<<*it;
		it++;
		cout<<*it<<endl;
	}
	system("pause");
    return 0;
}