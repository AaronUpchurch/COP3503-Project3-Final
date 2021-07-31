#pragma once
#include "SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>
#define windowX 4000
#define windowY 4000
#define maxX 1000000
#define maxY 1000000
using namespace std;
using namespace sf;
// Particles Class 
class Particle {
public:
	bool active;
	float* x;
	float* y;
	double mass;
	float xVel;
	float yVel;
	bool moveMe;
	Particle(float* x_, float* y_, double mass_, float xVel_, float yVel_) : moveMe(true), active(true), x(x_), y(y_), mass(mass_), xVel(xVel_), yVel(yVel_) {
	}
	void move() {
		if (moveMe) {
			*x += xVel;
			*y += yVel;
		}

	}
	void applyGravity(float& xforce, float& yforce) {

		xVel = xVel + (xforce / mass);
		yVel = yVel + (yforce / mass);
	}
};
