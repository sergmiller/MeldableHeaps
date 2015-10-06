#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cstdlib>
#include <string>
#include <random>
#include <vector>
#include <random>
#include "betaHeap.h"
using namespace std;

int main()
{
	//ifstream cin("input.txt");
	//ofstream cout("output.txt");
	size_t steps = 10000;
	//cin >> steps;
	
	//std::default_random_engine gen;
	//std::uniform_int_distribution<int> myrand(0, 1000);
	
	cout << "ALL TESTING:" << endl;
	TestGenerator myTestGen;
	//myTestGen.BuildRandomTest(steps);
	//myTestGen.RunRealTest();
	myTestGen.TestLogTime();
	system("pause");
	return 0;
}
