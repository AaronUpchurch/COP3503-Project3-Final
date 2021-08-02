//=================================== Includes =================================================//
#include "SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1/include/SFML/Graphics.hpp"
#include "Particle.h"
#include <iostream>
#include <chrono>
#include "bhTree.h"
#include "Rand.h"
#include "bhTree.h"
#include <Windows.h>              
#include <queue>
#include <chrono>
#include <iomanip>
#include <string>

//========================= Physics Constants ========================================//

#define G 0.000000000006674 // Gravitational constant used in force equation 
#define forAllParticles for(int i = 0; i < numOfParticles; ++i) // standard 
#define particleMass 5000000000000000 // Mass of all particles

//============================== Simulation Constants ===============================================//

#define cyclesToDo -1 // number of iterations the program should complete
#define numOfParticles 1000 // number of particles in the system 


//==================================== namespaces ===========================================//

using namespace sf;
using namespace std;

//=================================== SFML Functions ==============================================//

float* Transformable::getX() {
    return &m_position.x;
} // links particle x to circle's x
float* Transformable::getY() {
    return &m_position.y;
} // links particle's y to circle's y
void Transformable::updatePos() {
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
} // updates position of circle

//==================================== Function Declarations ==========================================//

void initializeWindow(RenderWindow& window, View& innerView, View& outerView);
void alterWindow(Keyboard& k, RenderWindow& window, View& innerView, View& outerView, CircleShape* circles, Particle** particles, bool& show, bool& clear, float& theta, int& thickness, bool& resetBoo, bool& pause, Mouse& mouse, vector<bool*> inits, int& index);
void clickToAddMass(Particle** particles, CircleShape* circles, Mouse& m, RenderWindow& window, View& innerView, bool antimatter);
void initializeWindowBorder(RectangleShape& windowBorder);
void initializeParticles(Particle** particles, CircleShape* circles);
void calcGrav(Particle& A, Particle& B);
void BarnesHut(bhTree* tree, Particle** particles, CircleShape* circles);
void deactivateOOB(Particle* particles[]);
void changeCircleRadius(float amount, CircleShape* circles);
void drawCirlces(CircleShape* circles, RenderWindow& window);
void changeColorBasedOnVelocity(Particle** particles, CircleShape* circles, float& avgXVel, float& avgYVel);
void drawExtras(RenderWindow& window, View& innerView, View& outerView, vector<Shape*> innerShapes, vector<Shape*> backgroundShapes, vector<Text*> texts);
void funParticleInitialization(Particle** particles, CircleShape* circles);
void getAvgVel(Particle** particles, float& avgXVel, float& avgYVel);
void clampView(RenderWindow& window, View& innerView, CircleShape& circle);
void clickToAddParticles(Particle** particles, CircleShape* circles, Mouse& m, RenderWindow& window, View& innerView, int& index, bool still);
void galaxyMergeInitialization(Particle** particles, CircleShape* circles);
void quadgalaxyMergeInitialization(Particle** particles, CircleShape* circles);
void updateTexts(vector<Text*> texts, float theta, double& totalTime, int& currentCycle, float& xVel, float yVel, int index);
void updateTracker(CircleShape& tracker, CircleShape* circles, int& particleIndexToObserve);
void updateDirectionalModel(RectangleShape& velocityDirectionModel, Particle* particles, int& particleIndexToTrack);
void initializeText(vector<Text*>& texts, Font& font);
void initializeTracker(CircleShape& tracker);
void initializeVelocityModel(CircleShape& velocityModel, RectangleShape& velocityDirectionModel);
void randomeInitializeParticles(Particle** particles, CircleShape* circles);
void incWeight(Particle** particles, float amount);

//=================================== Window/View Functions ==============================================//

void initializeWindow(RenderWindow& window, View& innerView, View& outerView) {
    window.setSize({ 2000,1300 });
    window.setView(innerView);
    innerView.setCenter(maxX / 2 + windowX / 2, maxY / 2 + windowY / 2);
    window.setPosition({ 0,0 });
    innerView.setViewport(sf::FloatRect(0.25f, 0.25, 0.5f, 0.5f));
    outerView.setViewport(FloatRect(0.0f, 0.0f, 1.0f, 1.0f));


    innerView.zoom(40);
    window.setView(innerView);
}
void updateTexts(vector<Text*> texts,float theta,double& totalTime, int& currentCycle, float& xVel, float yVel, int index) {
    texts.at(0)->setString("Total Time: " + to_string(totalTime) + "s");
    texts.at(1)->setString("Average Iterations Per Second: " + to_string(currentCycle / totalTime));
    texts.at(2)->setString("Total Iterations: " + to_string(currentCycle));
    texts.at(4)->setString("X Velocity: " + to_string(xVel) + "\n" + "Y Velocity: " + to_string(yVel));
    texts.at(5)->setString("Theta: " + to_string(theta));
    texts.at(6)->setString("Particle ID: " + to_string(index));

}
void updateDirectionalModel(RectangleShape& velocityDirectionModel, Particle** particles, int& particleIndexToObserve) {
    velocityDirectionModel.setRotation(atan(particles[particleIndexToObserve]->yVel / particles[particleIndexToObserve]->xVel) * 180 / 3.14);
    if (particles[particleIndexToObserve]->xVel < 0) { // if oppiste signs
        velocityDirectionModel.rotate(180); /// rotate an extra 90 degreess, makes up for atan's positve boundaries
    }
    velocityDirectionModel.setSize({ 50,5 });
}
void alterWindow(Keyboard& k, RenderWindow& window, View& innerView, View& outerView, CircleShape* circles, Particle** particles, bool& show, bool& clear, float& theta, int& thickness, bool& resetBool, bool& pause, Mouse& mouse, vector<bool*> inits, int& index) {
    if (k.isKeyPressed(k.Q)) {
        show = !show;
        while (k.isKeyPressed(k.Q)) {
            //wait
        }
    }
    
    if (k.isKeyPressed(k.Space)) {
        pause = !pause;
        while (k.isKeyPressed(k.Space)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.Up)) { // shifts window 
        if (!pause) {
            innerView.move({ 0,-innerView.getSize().y / 20 });
        }
        else {
            innerView.move({ 0,-innerView.getSize().y / 80 });
        }

        window.setView(innerView);

    }
    if (k.isKeyPressed(k.Down)) {
        if (!pause) {
            innerView.move({ 0,innerView.getSize().y / 20 });
        }
        else {
            innerView.move({ 0,innerView.getSize().y / 80 });
        }
        window.setView(innerView);

    }
    if (k.isKeyPressed(k.Left)) {
        if (!pause) {
            innerView.move({ -innerView.getSize().y / 20,0 });
        }
        else {
            innerView.move({ -innerView.getSize().y / 80 ,0 });
        }
        window.setView(innerView);

    }
    if (k.isKeyPressed(k.Right)) {
        if (!pause) {
            innerView.move({ innerView.getSize().y / 20,0 });
        }
        else {
            innerView.move({ innerView.getSize().y / 80,0 });
        }
        window.setView(innerView);

    }
    if (k.isKeyPressed(k.A)) {
        innerView.zoom(1.1);
        thickness = circles[0].getRadius() * 2;
        window.setView(innerView);
        changeCircleRadius(1.1, circles);


    }
    if (k.isKeyPressed(k.S)) {
        innerView.zoom(.90);
        thickness = circles[0].getRadius() * 2;
        window.setView(innerView);
        changeCircleRadius(.9, circles);


    }
    if (k.isKeyPressed(k.Z)) {
        for (int i = 0; i < numOfParticles; ++i) {
            circles[i].setRadius(circles[i].getRadius() * 1.5);
        }
        while (k.isKeyPressed(k.Z)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.X)) {
        for (int i = 0; i < numOfParticles; ++i) {
            circles[i].setRadius(circles[i].getRadius() * .5);
        }
        while (k.isKeyPressed(k.X)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.Num1)) {
        for (int i = 0; i < inits.size(); ++i) {
            *inits.at(i) = false;
        }
        *inits.at(0) = true;
        resetBool = true;
    }
    if (k.isKeyPressed(k.Num2)) {
        for (int i = 0; i < inits.size(); ++i) {
            *inits.at(i) = false;
        }
        *inits.at(1) = true;
        resetBool = true;
    }
    if (k.isKeyPressed(k.Num3)) {
        for (int i = 0; i < inits.size(); ++i) {
            *inits.at(i) = false;
        }
        *inits.at(2) = true;
        resetBool = true;
    }
    if (k.isKeyPressed(k.Num4)) {
        for (int i = 0; i < inits.size(); ++i) {
            *inits.at(i) = false;
        }
        *inits.at(3) = true;
        resetBool = true;
    }
    if (k.isKeyPressed(k.W)) {
        incWeight(particles, 1.5);
        while (k.isKeyPressed(k.Q)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.E)) {
        incWeight(particles, .5);
        while (k.isKeyPressed(k.Q)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.R)) {
        clear = !clear;
        while (k.isKeyPressed(k.Q)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.P)) {
        theta += .25;
        resetBool = true;
        while (k.isKeyPressed(k.P)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.O)) {
        theta -= .25;
        resetBool = true;
        while (k.isKeyPressed(k.O)) {
            //wait
        }
    }
    if (k.isKeyPressed(k.I)) {
        index += 1;
        while (k.isKeyPressed(k.I)) {
            //wait
        }
    }
    


}
void clickToAddMass(Particle** particles, CircleShape* circles, Mouse& m, RenderWindow& window, View& innerView, bool antimatter) {

    if (m.isButtonPressed(Mouse::Button::Left)) {
        window.setView(innerView);
        //circles[0].setPosition(Vector2f(m.getPosition()));
        *particles[0]->x = window.mapPixelToCoords(Vector2i(m.getPosition())).x;
        *particles[0]->y = window.mapPixelToCoords(Vector2i(m.getPosition())).y;

        particles[0]->mass = particleMass * (double)5000;
        if (antimatter) {
            particles[0]->mass *= -1;
        }
        circles[0].updatePos();

        circles[0].setFillColor(Color::Transparent);
    }
}
void clickToAddParticles(Particle** particles, CircleShape* circles, Mouse& m, RenderWindow& window, View& innerView, int& index, bool still) {

    int times = 10;
    if (index > numOfParticles - times) {
        index = 0;
    }
    if (still) {
        times = 1;
    }
    if (m.isButtonPressed(Mouse::Button::Left)) {
        window.setView(innerView);
        for (int i = 0; i < times; ++i) {
            *particles[index]->x = window.mapPixelToCoords(Vector2i(m.getPosition())).x;
            *particles[index]->y = window.mapPixelToCoords(Vector2i(m.getPosition())).y;
            if (still) {
                particles[index]->xVel = 0;
                particles[index]->yVel = 0;
            }
            circles[index].updatePos();
            ++index;
        }
    }
}
void initializeWindowBorder(RectangleShape& windowBorder) {
    windowBorder.setFillColor(Color::Transparent);
    windowBorder.setOutlineColor(Color(200, 200, 200));
    windowBorder.setOutlineThickness(1);
    windowBorder.setPosition({ 250.f,250.f });
    windowBorder.setSize({ 500,500 });
}
void drawExtras(RenderWindow& window, View& innerView, View& outerView, vector<Shape*> innerShapes, vector<Shape*> backgroundShapes, vector<Text*> texts) {
    window.setView(innerView); // draws outline around the particle window
    for (int i = 0; i < innerShapes.size(); ++i) {
        window.draw(*innerShapes.at(i));
    }
    window.setView(outerView);
    for (int i = 0; i < texts.size(); ++i) {
        window.draw(*texts.at(i));
    }
    for (int i = 0; i < backgroundShapes.size(); ++i) {
        window.draw(*backgroundShapes.at(i));
    }
    window.setView(innerView);

}
void clampView(RenderWindow& window, View& innerView, CircleShape& circle) {
    innerView.move(circle.getPosition().x - innerView.getCenter().x + circle.getRadius(), circle.getPosition().y - innerView.getCenter().y - circle.getRadius());
    window.setView(innerView);
}
void updateTracker(CircleShape& tracker, CircleShape* circles, int& particleIndexToObserve) {

    tracker.setPosition(circles[particleIndexToObserve].getPosition());
    tracker.setRadius(circles[particleIndexToObserve].getRadius() * 10);
    tracker.setOutlineThickness(tracker.getRadius() / 5);
}

//================================= Particle Functions ===================================================//

void initializeParticles(Particle** particles, CircleShape* circles) {
    forAllParticles{ // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(),circles[i].getY(),particleMass, Rand(100).val - 50, Rand(100).val-50);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2)) / 50;
    }
}
void calcGrav(Particle& A, Particle& B) {
    float r = sqrt((A.x - B.x) * (A.x - A.x) + (A.y - B.y) * (A.y - B.y));
    if (r == 0) {
        return;
    }
    float force = (A.mass * B.mass * G) / (r * r);
    if (r <= softenRad) {
        force = (G * A.mass * B.mass * r) / pow(r * r + softenFactor * softenFactor, 1.5);
    }

    float xForce = ((A.x - B.x) / r * force);
    float yForce = ((A.y - B.y) / r * force);

    A.applyGravity(xForce, yForce);
    xForce *= -1;
    yForce *= -1;
    B.applyGravity(xForce, yForce);
}
void BarnesHut(bhTree* tree, Particle** particles, CircleShape* circles) {
    forAllParticles{
            if (particles[i]->active) { // inserts particles that are not out of bounds 
                tree->root = tree->insert(tree->root, particles[i]); // make sure rand position cant be at max window, if so, infinite recursion
            }

    // will infinitiely recur if two particles with same x and y coords

    }
        forAllParticles{
                       if (particles[i]->active) {
                           particles[i]->move();
                           circles[i].updatePos();
                       }
    }
    tree->applyGravAll(tree->root); // applies gravity to all particles 
}
void deactivateOOB(Particle* particles[]) {
    for (int i = 0; i < numOfParticles; ++i) {
        if (particles[i]->x < 0 || particles[i]->y < 0 || *particles[i]->x > maxX || *particles[i]->y > maxY) {
            particles[i]->active = false;
        }
    }
}
void getAvgVel(Particle** particles, float& avgXVel, float& avgYVel) {
    forAllParticles{
        avgXVel += abs(particles[i]->xVel);
        avgYVel += abs(particles[i]->yVel);
    }
    avgXVel /= numOfParticles;
    avgYVel /= numOfParticles;
}
void funParticleInitialization(Particle** particles, CircleShape* circles) {
    forAllParticles{ // initializes all particles
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(),circles[i].getY(),50000000000000000, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2)) / 5; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2)) / 5;
    }
    circles[0].setRadius(70);
    particles[0]->mass = 5000000000000000 * 1000;
    circles[0].setPosition(windowX / 2 + maxX / 2, windowY / 2 + maxY / 2);
    particles[0]->xVel = 0;
    particles[0]->yVel = 0;
}
void galaxyMergeInitialization(Particle** particles, CircleShape* circles) {
    for (int i = 0; i < numOfParticles / 2; ++i) { // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(), circles[i].getY(), particleMass, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2)) / 50;
    }
    int distanceSeperate = maxX / 100;
    for (int i = numOfParticles / 2; i < numOfParticles; ++i) { // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2 + distanceSeperate, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(), circles[i].getY(), particleMass, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2 + distanceSeperate)) / 50;
    }
}
void quadgalaxyMergeInitialization(Particle** particles, CircleShape* circles) {
    int distanceSeperate = maxX / 50;
    for (int i = 0; i < numOfParticles / 4; ++i) { // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(), circles[i].getY(), particleMass, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2)) / 50;
    }
    for (int i = numOfParticles / 4; i < numOfParticles / 2; ++i) { // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2 + distanceSeperate, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(), circles[i].getY(), particleMass, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2 + distanceSeperate)) / 50;
    }
    for (int i = numOfParticles / 2; i < numOfParticles * 3 / 4; ++i) { // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2 + distanceSeperate);
        particles[i] = new Particle(circles[i].getX(), circles[i].getY(), particleMass, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2 + distanceSeperate)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2)) / 50;
    }
    for (int i = numOfParticles * 3 / 4; i < numOfParticles; ++i) { // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2 + distanceSeperate, Rand((windowY - 1)).val + maxY / 2 + distanceSeperate);
        particles[i] = new Particle(circles[i].getX(), circles[i].getY(), particleMass, 0, 0);
        particles[i]->xVel = (*particles[i]->y - ((windowY / 2) + maxX / 2 + distanceSeperate)) / 50; // makes circluar pattern //
        particles[i]->yVel = -(*particles[i]->x - ((windowX / 2) + maxY / 2 + distanceSeperate)) / 50;
    }
}
void randomeInitializeParticles(Particle** particles, CircleShape* circles) {
    forAllParticles{ // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(),circles[i].getY(),particleMass, Rand(100).val - 50, Rand(100).val - 50);
    }
};
void incWeight(Particle** particles, float amount) {
    forAllParticles{
        particles[i]->mass *= amount;
    }
}



//==================================== Circles Functions ===================================================//

void changeCircleRadius(float amount, CircleShape* circles) {

    forAllParticles{
        circles[i].setRadius(circles[i].getRadius() * amount);
    }
}
void drawCirlces(CircleShape* circles, RenderWindow& window) {
    forAllParticles{
        window.draw(circles[i]);
    }
}
void changeColorBasedOnVelocity(Particle** particles, CircleShape* circles, float& avgXVel, float& avgYVel) {
    forAllParticles{ // changes color of particles 
        float totv = abs(particles[i]->xVel) + abs(particles[i]->yVel);
        if (totv < .3 * (avgXVel + avgYVel)) {
            circles[i].setFillColor(Color::Red);
        }
        else if (totv < .6 * (avgXVel + avgYVel)) {
            circles[i].setFillColor(Color::Yellow);
        }
        else if (totv < .9 * (avgXVel + avgYVel)) {
            circles[i].setFillColor(Color::Green);
        }
        else if (totv < 1.2 * (avgXVel + avgYVel)) {
            circles[i].setFillColor(Color::Cyan);
        }
        else {
        circles[i].setFillColor(Color::Magenta);
        }
    }
}

//==================================== Misc Functions ======================================================//

void initializeText(vector<Text*>& texts, Font& font) {
    for (int i = 0; i < texts.size(); ++i) {
        texts.at(i)->setFont(font);
        texts.at(i)->setCharacterSize(24);
        texts.at(i)->setFillColor(Color::White);
    }
    texts.at(0)->setPosition({ 10,50 });
    texts.at(1)->setPosition({ 10,100 });
    texts.at(2)->setPosition({ 10,150 });
    texts.at(3)->setString( "Pause: Space           Pan Camera: Arrow Pad       Increase Particle Size: Z       Increase Theta: P\n"
                            "Predefined Sets: 1-4   Zoom In: S                  Decrease Particle Size: X       Decrease Theta: O\n"
                            "Show Algorithm: Q      Zoom Out: A                 Increase Particle Mass: W\n"
                            "Show Path: R           Track New Particle: I       Decrease Particle Mass: E"
                            );
    texts.at(3)->setPosition({ 40,800 });
    texts.at(3)->setCharacterSize(22);
    texts.at(4)->setPosition({ 775,50 });
    texts.at(5)->setPosition({ 10,250 });
    texts.at(6)->setPosition({ 800,275 });
    texts.at(7)->setString("Particle Count: " + to_string(numOfParticles));
    texts.at(7)->setPosition({ 10,200 });
  
}
void initializeTracker(CircleShape& tracker) {
    tracker.setFillColor(Color::Transparent);
    tracker.setOutlineColor(Color::White);
    tracker.setOutlineThickness(20);
}
void initializeVelocityModel(CircleShape& velocityModel, RectangleShape& velocityDirectionModel) {

    velocityModel.setRadius(20);
    velocityModel.setPosition({ 850,150 });
    velocityModel.setOutlineColor(Color::White);
    velocityModel.setScale({ 1.3,2 });
    velocityModel.setOutlineThickness(2);
    velocityModel.setFillColor(Color(100, 100, 100));

    velocityDirectionModel.setScale({ 1.3, 2 });
    velocityDirectionModel.setFillColor(Color(200, 200, 200));
    velocityDirectionModel.setSize({ 50,5 });
    velocityDirectionModel.setOrigin({ 0,2.5 });
    velocityDirectionModel.setPosition({ 875,190 });
}

//====================================Main Function =========================================================//

int main()
{
    //============================== Window and Views ==========================================//

    RenderWindow window(VideoMode(maxX, maxY), "Gravity Gators");
    View innerView(FloatRect(200.f, 200.f, 300.f, 200.f)); // inner view to show particles
    View outerView(FloatRect(0, 0, 1000, 1000)); // outer view to show buttons, time, velocity, ect
    initializeWindow(window, innerView, outerView); // MUST DO THIS, ELSE COMPUTER CRASHES??? 

    //============================== Keyboard and Mouse ====================================//

    Mouse mouse;
    Keyboard k;

    //======================== Textual Graphics ==========================================//

    Font font; // font for all text
    font.loadFromFile("font.ttf");

    Text totalTimeText; // text to display total time
    Text cyclesPerSecondText; // text to display cycles per seconde
    Text iterationCountText; // text to display iteration count
    Text controlInstructions; // text to display controls instructions
    Text trackerVelocity;
    Text thetaText;
    Text particleIndexText;
    Text particleCount;

    vector<Text*> texts; // holds all texts
    texts.push_back(&totalTimeText);
    texts.push_back(&cyclesPerSecondText);
    texts.push_back(&iterationCountText);
    texts.push_back(&controlInstructions);
    texts.push_back(&trackerVelocity);
    texts.push_back(&thetaText);
    texts.push_back(&particleIndexText);
    texts.push_back(&particleCount);
    initializeText(texts, font);

      //=========================== Visual Graphics =============================================//

    RectangleShape windowBorder; // border around inner view
    initializeWindowBorder(windowBorder);

    CircleShape tracker; // Circle that tracks particle
    initializeTracker(tracker);
    
    CircleShape velocityModel; // model of particle being tracked
    RectangleShape velocityDirectionModel; // dispalys velocity vector of particle being tracked
    initializeVelocityModel(velocityModel, velocityDirectionModel);
    
    vector<Shape*> backgroundShapes; // holds all backround shapes
    vector<Shape*> innerShapes; // holds all inner shapes
    innerShapes.push_back(&tracker);
    backgroundShapes.push_back(&windowBorder);
    backgroundShapes.push_back(&velocityModel);
    backgroundShapes.push_back(&velocityDirectionModel);


    //============================== Particles and Circles =================================//

    CircleShape* circles = new CircleShape[numOfParticles]; // circles used to print to window 
    Particle* particles[numOfParticles]; // stores x,y,xVel,yVel,mass 

    //funParticleInitialization(particles, circles); // any one of these initializes all the particles
    initializeParticles(particles, circles);
    
    
    
    //============================== Variables ==========================================//

    float avgXVel = 0; // average x velocity of system
    float avgYVel = 0; // average y velocity of system
    int cycleCounter = 0; // current cycles
    int particleIndexToObserve = 1; // index of particle to track
    bool pause = false; // if simulation is paused
    int currentCycle = 0; // current simulations iteration
    int thickness = 20; // thickness of showRectangles's lines
    double totalTime = 0; // total time since start of simulation
    auto start = std::chrono::system_clock::now(); // starting time
    bool resetBool = false;
    bool showRectangles = false;
    bool initOne = true;
    bool initTwo = false;
    bool initThree = false;
    bool initFour = false;
    bool clear = true;
    float theta = 1;
    vector<bool*> inits;
    inits.push_back(&initOne);
    inits.push_back(&initTwo);
    inits.push_back(&initThree);
    inits.push_back(&initFour);
    

    //================================= Program ===================================================//

    while (true)
    {
        //============================== Closes Window If Clicked =================================//

        Event event; 
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        //============================== Constantly Running =============================================//

        alterWindow(k, window, innerView, outerView, circles,particles, showRectangles, clear, theta, thickness,resetBool, pause, mouse, inits,particleIndexToObserve); // 

        drawCirlces(circles, window); // draws all circles onto the window
        

        if (resetBool) {
            if (initOne) {
                initializeParticles(particles, circles);
            }
            else if (initTwo) {
                galaxyMergeInitialization(particles, circles);
            }
            else if (initThree) {
                quadgalaxyMergeInitialization(particles, circles);
            }
            else if (initFour) {
                randomeInitializeParticles(particles, circles);
            }
            totalTime = 0;
            currentCycle = 0;
            resetBool = false;
        }
        drawExtras(window, innerView, outerView, innerShapes, backgroundShapes, texts); // draws border around inner window

        //clampView(window,innerView, circles[particleIndexToObserve]);
        window.display();
        if (clear) {
            window.clear();
        }

        std::chrono::duration<double> diff = chrono::system_clock::now() - start;
        start = chrono::system_clock::now();

        if (!pause) { // Pauses time if paused
            totalTime += diff.count();
            ++currentCycle;
        }

        if (currentCycle == cyclesToDo) {
            return 0;
        }

        //============================== Running If Not Paused =============================================//

        if (!pause) {

            bhTree* tree = new bhTree(theta); // makes a new Barnes-Hill tree

            BarnesHut(tree, particles, circles); // simulates all particles using the barnes-hill tree

            deactivateOOB(particles); // deactivates out of bounds particles

            getAvgVel(particles, avgXVel, avgYVel);




            if (showRectangles) {
                tree->showRectangles(window, thickness); // shows Barnes-Hill algortithm calculations
            }

            updateTracker(tracker, circles, particleIndexToObserve);

            changeColorBasedOnVelocity(particles, circles, avgXVel, avgXVel); // changes oclor of particles based on average velocoties

            //clickToAddMass(particles, circles, mouse, window,innerView,false);

            clickToAddParticles(particles, circles, mouse, window, innerView, cycleCounter, false); // adds particles to simulation if inner view is clicked
            // maybe add to it becomes a contant stream

            updateDirectionalModel(velocityDirectionModel, particles, particleIndexToObserve);

            updateTexts(texts,theta,totalTime, currentCycle, particles[particleIndexToObserve]->xVel, particles[particleIndexToObserve]->yVel*-1, particleIndexToObserve); // changes texts to display correct number
        }
    }

    return 0;


}
