#pragma once
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;
// a helpful random number generator 
// dont chang me //
class Rand {
public:
	float val;
	Rand(int max) {
		val = .00001 + rand() % max + (float(rand() % 1000) / 1000);
	}
};
