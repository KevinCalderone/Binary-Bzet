//#include <cstdlib>
#include <stdio.h>
#include "BinaryBzet.h"
#include <sstream>
#include <time.h>

enum operations {
	AND=1, OR=7, NAND=14, NOR=1, NOT=10
};

int main()
{
	u32 omega;
	u32 power = 26;
	u32 maxpower = 27;

	u32 index1 = rand()%(1<<power);
	u32 index2 = index1 + rand()%((1<<maxpower)-index1) + 1;
	u32 stride = rand()%(index2/2);
	double min = 10000;
	double max = 0;
	string worst;
	string best;
	cout << "HOW MANY TESTS WOULD YOU LIKE TO PERFORM? ";
	cin >> omega;
	cout << endl << "TESTS REMAINING:" << endl;

	clock_t init, final;
	init=clock();
	BinaryBzet a(index1,index2,stride);
	final=clock()-init;
	double first =  (double)final / ((double)CLOCKS_PER_SEC);
	stringstream ss1;
	ss1 << index1;
	stringstream ss2;
	ss2 << index2;
	stringstream ss3;
	ss3 << stride;
	string temp = "(" + ss1.str() + "," + ss2.str() + "," + ss3.str() + ")";
	init = clock();
	BinaryBzet b(temp);
	final=clock()-init;
	double second = (double)final / ((double)CLOCKS_PER_SEC);
	double factor = first / second;
	if(factor<min){
		min = factor;
		worst = temp;}
	if(factor>max){
		max = factor;
		best = temp;}
	double dfactor;
	cout << omega-- << endl;
	while(omega>0){
		cout << omega << endl;
		index1 = rand()%(1<<power);
		index2 = index1 + rand()%((1<<maxpower)-index1) + 1;
		stride = rand()%(index2/2);
		init=clock();
		BinaryBzet a(index1,index2,stride);
		final=clock()-init;
		first =  (double)final / ((double)CLOCKS_PER_SEC);
		stringstream ss1;
		ss1 << index1;
		stringstream ss2;
		ss2 << index2;
		stringstream ss3;
		ss3 << stride;
		string temp = "(" + ss1.str() + "," + ss2.str() + "," + ss3.str() + ")";
		init = clock();
		BinaryBzet b(temp);
		final=clock()-init;
		second = (double)final / ((double)CLOCKS_PER_SEC);
		dfactor = first/second;
		if(dfactor<min){
			min = dfactor;
			worst = temp;}
		if(dfactor>max){
			max = dfactor;
			best = temp;}
		factor = (factor + dfactor)/2.0;
		omega--;
	}
	cout << endl << endl;
	cout << "FASTER BY FACTOR OF: " << factor << endl << endl;
	cout << "HAD WORST FACTOR OF: " << min << "     FROM: " << worst << endl;
	cout << "HAD BEST FACTOR OF:  " << max << "     FROM: " << best << endl;
	cout << endl << endl;
	system("pause");
    return 0;
}