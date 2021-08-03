#pragma once
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;
// A helpful random number generator used for particle initialization
class Rand {
public:
	float val;
	Rand(int max) {
		val = .00001 + rand() % max + (float(rand() % 1000) / 1000);
	}
};
