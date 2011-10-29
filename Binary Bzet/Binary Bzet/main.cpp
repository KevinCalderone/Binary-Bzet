#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"

int main()
{
	// ARASH'S TEST
/*
	BinaryBzet b;
	if(true){
		BinaryBzet a(1,5);
		b = BinaryBzet(&a.getBzetBinaryString(),a.getDepth());
	}
	cout << b.getBzetString() << endl << endl;
*/
	// END OF ARASH' TEST



//=======
	// TEST CODE
	cout << "=========================================" << endl;
	cout << "========== Binary Bzet - CS130 ==========" << endl;
	cout << "===== Milestone 1: Core Algorithms ======" << endl;
	cout << "=========================================" << endl;

	cout << endl;
	cout << "========== Testing Constructors =========" << endl;

	BinaryBzet a((1<<2),(1<<5),516);
	cout << a.getBzetString() << endl;

	cout << endl;
	cout << "============ Testing Shifting ===========" << endl;

	BinaryBzet::testShift();

	cout << endl;
	cout << "========== Testing Setting Bit ==========" << endl;

	BinaryBzet L;
	L.testSET();

	cout << endl;
	cout << "========== Testing Pretty Print =========" << endl;
	cout <<endl;
	cout<<a.getBzetPretty()<<endl;
	cout<<a.getBzetString()<<endl;
	cout << endl;
	cout << "=========================================" << endl;

/*
=======
	//Michael's Test Code
	{

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
	u32 test = 8;
	vector<bool> result = a.bsNeg(bzetA,2,depthA,test);
	
	vector<bool>::iterator it;
	cout<<"Bzet A\n Depth A: " << depthA << "\n";
	for(it = result.begin(); it != result.end(); it++)
	{
		cout<<*it<<" ";
	};
	cout << "\n";
	
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
	//End of Michael's Test code.
	*/

	system("pause");
    return 0;
}