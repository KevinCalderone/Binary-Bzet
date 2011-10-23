#include <cstdlib>
#include <stdio.h>

#include "BinaryBzet.h"

int main()
{
	BinaryBzet a(1<<15);
	BinaryBzet b();

	cout << endl << a.getBzetString() << endl << a.getBzetString().size() << endl;

	// Test code
	printf("I do nothing right now :(\n");
	system("pause");
    return 0;
}