#include <cstdlib>
#include <stdio.h>

#include "BinaryBzet.h"

int main()
{
	// TEST CODE
	BinaryBzet a(5,19,3);
	cout << a.getBzetString() << endl;

	// Shift test
	BinaryBzet b(1<<2);
	b.shift(5);
	// Need to have way of outputting m_bzet to see difference

	BinaryBzet L;
	L.testSET();

	system("pause");
    return 0;
}