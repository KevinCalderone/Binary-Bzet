#include <cstdlib>
#include <stdio.h>

#include "BinaryBzet.h"
int main()
{
	// TEST CODE
	//BinaryBzet a((1<<13),(1<<20),516);
	//cout << a.getBzetString() << endl;

	BinaryBzet::testShift();

	// Need to have way of outputting m_bzet to see change

	BinaryBzet L;
	L.testSET();

	/*
	BinaryBzet a;
	vector<bool> bzetA;
//	bool val1[] = {1,0, 1,0, 1,1, 1,0, 1,0, 0,1, 0,1};
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(1); bzetA.push_back(1);
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(0); bzetA.push_back(1);
	bzetA.push_back(0); bzetA.push_back(1);

	u32 depthA = 3;

	vector<bool> bzetB;
//	bool val2[] = {1,0, 1,1, 1,0};
	bzetB.push_back(1); bzetB.push_back(0);
	bzetB.push_back(1); bzetB.push_back(1);
	bzetB.push_back(1); bzetB.push_back(0);
//	bzetB = vector<bool>(val1,val1+8);
	u32 depthB = 3;
	a.align(bzetB,depthB,bzetA,depthA);

	vector<bool>::iterator it;
	cout<<"Bzet A\n Depth A: " << depthA << "\n";
	for(it = bzetA.begin(); it != bzetA.end(); it++)
	{
		cout<<*it<<" ";
	};
	cout << "\n";
	cout<<"Bzet B\n Depth B: " << depthB << "\n";
	for(it = bzetB.begin(); it != bzetB.end(); it++)
	{
		cout<<*it<<" ";
	}
	*/
	system("pause");
    return 0;
}