#include <cstdlib>
#include <stdio.h>

#include "BinaryBzet.h"

int main()
{
	BinaryBzet a(1<<2);

	cout << a.getBzetString() << endl << a.getBzetString().size() << endl;

	a.shift(1);

	cout << a.getBzetString() << endl << a.getBzetString().size() << endl;

	// Test code
	system("pause");
    return 0;
}