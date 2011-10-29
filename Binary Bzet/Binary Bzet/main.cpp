#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"

int main()
{
	BinaryBzet b;
	if(true){
		BinaryBzet a(1,5);
		b = BinaryBzet(&a.getBzetBinaryString(),a.getDepth());
	}
	cout << b.getBzetString() << endl << endl;

	system("pause");
    return 0;
}