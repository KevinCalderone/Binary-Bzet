#include <cstdlib>
#include <stdio.h>

#include "BinaryBzet.h"

int main()
{
	// TEST CODE
	BinaryBzet a((1<<13),(1<<20),516);
	cout << a.getBzetString() << endl;


	// Shift test
	BinaryBzet b(1<<2);
	cout << endl << b.getBzetString() << endl << b.getBzetString().size() << endl;
	a.shift(1);
	cout << b.getBzetString() << endl << b.getBzetString().size() << endl;

	BinaryBzet L;
	L.testSET();

	system("pause");
    return 0;
}