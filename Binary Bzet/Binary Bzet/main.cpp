//#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"

enum operations {
	AND=1, OR=7, NAND=14, NOR=1, NOT=10
};

int main()
{
	BinaryBzet a(7);
	BinaryBzet b((u32)0,7);
	BinaryBzet z(1,7,1);

	cout << "\nBinaryBzet A(7);\nBinaryBzet B(0,7);\nBinaryBzet C(1,7,1);\n" << endl
		 << "A:  00000001     " << a.getBzetString() << endl 
		 << "B:  11111110     " << b.getBzetString() << endl
		 << "C:  01010100     " << z.getBzetString() << endl;

	cout << "==========================\n" << endl;
	vector<bool> v = a.binaryOp(AND,a.getBzetBinaryString(),0,b.getBzetBinaryString(),0,a.getDepth());
	BinaryBzet c(&v, a.getDepth()); c.shift(0);
	
	cout << "A  &  B =        " << c.getBzetString() << endl;

	v = a.binaryOp(OR,a.getBzetBinaryString(),0,b.getBzetBinaryString(),0,a.getDepth());
	BinaryBzet d(&v, a.getDepth()); d.shift(0);

	cout << "A  |  B =        " << d.getBzetString() << endl;

	/*
	// Messed up NOT test
	{
		BinaryBzet z(0,8,1);

		v = a.binaryOp(NOT,z.getBzetBinaryString(),0,b.getBzetBinaryString(),0,z.getDepth());
		BinaryBzet q(&v, z.getDepth()); d.shift(0);

		cout << "~B =             " << q.getBzetString() << endl;
	}
	*/

	v = a.binaryOp(NAND,a.getBzetBinaryString(),0,b.getBzetBinaryString(),0,a.getDepth());
	BinaryBzet e(&v, a.getDepth()); e.shift(0);

	cout << "A  ~& B =        " << e.getBzetString() << endl;

	v = a.binaryOp(NOR,a.getBzetBinaryString(),0,b.getBzetBinaryString(),0,a.getDepth());
	BinaryBzet f(&v, a.getDepth()); f.shift(0);

	cout << "A  ~| B =        " << f.getBzetString() << endl;

	BinaryBzet g(a);
	g.shift(3);
	cout << "A  >> 3 =        " << g.getBzetString() << endl;

	BinaryBzet h(a);
	h.shift(-2);
	cout << "A  << 2 =        " << h.getBzetString() << endl;

	cout << "B.getLastBit():  " << b.getLastBit() << endl;
	cout << "B.charAt(2):     " << b.getCharFromBzet(2) << endl;
	cout << "C.prettyOutput():" << endl << endl 
		 << z.getBzetPretty() << endl;


	system("pause");
    return 0;
}