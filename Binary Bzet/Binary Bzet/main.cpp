#include <cstdlib>
#include <stdio.h>

#include "BinaryBzet.h"

int main()
{

	// TEST CODE
	BinaryBzet a((1<<13),(1<<20),516);
	cout << a.getBzetString() << endl;

	system("pause");
    return 0;
}