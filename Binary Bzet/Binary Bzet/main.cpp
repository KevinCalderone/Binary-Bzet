//#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"
#include <sstream>
#include <time.h>
#include <fstream>
#include "Test.h"

int main()
{
	BinaryBzet a("(0,10,1)");
	cout << a.getBzetString() << endl;
	system("pause");
    return 0;
}