#pragma once
#include "SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>
#define windowX 4000 // window size length
#define windowY 4000 // window size height
#define maxX 1000000 // length of simulation space
#define maxY 1000000 // height of simulation space
using namespace std;
using namespace sf;
// Particle class to record physical aspects of each particle
class Particle {
public:
	bool active; // if out of bounds, sets to not active
	float* x; // pointer to x position of particle
	float* y;// pointer to y position of particle
	double mass; // mass of the particle
	float xVel; // x velovity of the particle
	float yVel; // y velocity of the particle
	bool moveMe; // boolean to fix particle in space
	Particle(float* x_, float* y_, double mass_, float xVel_, float yVel_) : moveMe(true), active(true), x(x_), y(y_), mass(mass_), xVel(xVel_), yVel(yVel_) { // particle constructor
	}
	void move() { // moves particle based on its velocities
		if (moveMe) {
			*x += xVel;
			*y += yVel;
		}

	}
	void applyGravity(float& xforce, float& yforce) { // changes velocites of particles based on the graviational forces

		xVel = xVel + (xforce / mass);
		yVel = yVel + (yforce / mass);
	}
};
