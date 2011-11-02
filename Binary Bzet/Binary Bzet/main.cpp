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
	cout << "============ Testing GetLastBit ===========" << endl;

	BinaryBzet::getLastBitTest();

	cout << endl;
	cout << "========== Testing Setting Bit ==========" << endl;

    BinaryBzet set;
    set.setTEST();
    
	cout << endl;
    cout << "============= Testing expand ============" << endl;
    
    BinaryBzet x;
    x.expandTEST();
    
	cout << endl;
    cout << "============ Testing bzetWalk ===========" << endl;
    
    BinaryBzet walk;
    walk.bzetWalkTEST();
    
    cout << endl;
    cout << "========= Testing bitSetCollapse ========" << endl;
    
    BinaryBzet collapse;
    collapse.bitSetCollapseTEST();

    cout << endl;
    cout << "============= Testing flip ==============" << endl;
    
    BinaryBzet flip;
    flip.flipTEST();
    
	cout << endl;
	cout << "========== Testing Pretty Print =========" << endl;
	cout <<endl;
	cout<<a.getBzetPretty()<<endl;
	cout<<a.getBzetString()<<endl;
	cout << endl;
	cout << "=========================================" << endl;
*/

//	=======
	//Michael's Test Code
//	{


	/*cout << "Copy to result Test\n";
	vector<bool> subtreeData;
	subtreeData.push_back(1); subtreeData.push_back(0);
	subtreeData.push_back(1); subtreeData.push_back(0);
	subtreeData.push_back(1); subtreeData.push_back(1);
	vector<bool> resultBzet;
	resultBzet.push_back(1); resultBzet.push_back(1);
	resultBzet.push_back(0); resultBzet.push_back(0);
	resultBzet.push_back(0); resultBzet.push_back(1);
	for(u32 i = 0; i<subtreeData.size(); i++)
	{
			resultBzet.push_back(subtreeData.at(i));
	}
	for(u32 i = 0; i<resultBzet.size(); i++)
	{
			cout << resultBzet[i] << "\n";
	}

	cout << "DataOP Test\n";
	vector<bool>::iterator itA;
	vector<bool>::iterator itB;
	vector<bool> bzetRet;
	
	itA = subtreeData.begin();
	itB = resultBzet.begin();
	for(itA; itA != subtreeData.end(); itA++)
	{
		bzetRet.push_back((*itA) & (*itB));
		itB++;
	}
	for(u32 i = 0; i<bzetRet.size(); i++)
	{
		cout << bzetRet[i] << "\n";
	}
	*/
	BinaryBzet a;
	//TT1Ttt
	vector<bool> bzetA;
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(1); bzetA.push_back(1);
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(1); bzetA.push_back(0);
	bzetA.push_back(0); bzetA.push_back(1);
	bzetA.push_back(0); bzetA.push_back(1);
	vector<bool> bzetB;
	//T1TT1
	bzetB.push_back(1); bzetB.push_back(0);
	bzetB.push_back(1); bzetB.push_back(1);
	bzetB.push_back(1); bzetB.push_back(0);
	bzetB.push_back(1); bzetB.push_back(0);
	bzetB.push_back(1); bzetB.push_back(1);
	u32 level = 3;
	vector<bool> resultBzet = a.binaryOp(1,bzetA,0,bzetB,0,level);
	cout << "Result\n";
	//Should output TT1TT0t
	for(u32 i = 0; i<resultBzet.size(); i++)
	{
			cout << resultBzet[i] << " ";
	}
	/*
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