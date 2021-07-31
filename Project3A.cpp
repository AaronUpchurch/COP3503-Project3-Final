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


// !!!!!!!!!! Make sure TA knows how to set up SFML initialization??? //

//========================= Physics Constants ========================================//
#define G 0.000000000006674 // Gravitational constant used in force equation 
#define forAllParticles for(int i = 0; i < numOfParticles; ++i) // standard 
#define particleMass 5000000000000000 // Mass of all particles

//============================== Simulation Constants ===============================================//
#define cyclesToDo -1 // number of iterations the program should complete
#define numOfParticles 2000 // number of particles in the system 

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
void moveWindow(Keyboard& k, RenderWindow& window, View& innerView, View& outerView, CircleShape* circles, int& thickness, bool& pause, Mouse& mouse);
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
void setUpTexts(Text& totalTimeText, Text& cyclesPerSecondText, Text& iterationCountText, double& totalTime, int& currentCycle);
void setupTrakcer(CircleShape& tracker, CircleShape* circles, int& particleIndexToObserve);
void setUpDirectionalModel(RectangleShape& velocityDirectionModel, Particle* particles, int& particleIndexToTrack);


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
void setUpTexts(Text& totalTimeText, Text& cyclesPerSecondText, Text& iterationCountText, double& totalTime, int& currentCycle) {
    totalTimeText.setString("Total Time: " + to_string(totalTime) + "s");
    cyclesPerSecondText.setString("Average Iterations Per Second: " + to_string(currentCycle / totalTime));
    iterationCountText.setString("Total Iterations: " + to_string(currentCycle));
}
void setUpDirectionalModel(RectangleShape& velocityDirectionModel, Particle** particles, int& particleIndexToObserve) {
    velocityDirectionModel.setRotation(atan(particles[particleIndexToObserve]->yVel / particles[particleIndexToObserve]->xVel) * 180 / 3.14);
    if (particles[particleIndexToObserve]->xVel < 0) { // if oppiste signs
        velocityDirectionModel.rotate(180); /// rotate an extra 90 degreess, makes up for atan's positve boundaries
    }
}
void moveWindow(Keyboard& k, RenderWindow& window, View& innerView, View& outerView, CircleShape* circles, int& thickness, bool& pause, Mouse& mouse) {
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
    /*if (pause) {
        if (mouse.isButtonPressed(Mouse::Button::Left)) {
            RectangleShape select;
            select.setFillColor(Color::Transparent);
            select.setOutlineColor(Color(150, 150, 150));
            select.setOutlineThickness(4);
            float startX = mouse.getPosition().x;
            float startY = mouse.getPosition().y;
            while (mouse.isButtonPressed(Mouse::Button::Left)) {
                //wait
            }
            select.setPosition({ startX,startY });
            while (!mouse.isButtonPressed(Mouse::Button::Left)) {
                select.setSize({ mouse.getPosition().x - startX, mouse.getPosition().y - startY });
                window.draw(select);

                window.display();

            }
        }
    }
    */


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
void setupTrakcer(CircleShape& tracker, CircleShape* circles, int& particleIndexToObserve) {

    tracker.setPosition(circles[particleIndexToObserve].getPosition());
    tracker.setRadius(circles[particleIndexToObserve].getRadius() * 10);
    tracker.setOutlineThickness(tracker.getRadius() / 5);
}

//================================== Particle Functions ===================================================//
void initializeParticles(Particle** particles, CircleShape* circles) {
    forAllParticles{ // initializes all particles 
        circles[i].setRadius(10);
        circles[i].setFillColor(Color::White);
        circles[i].setPosition(Rand((windowX - 1)).val + maxX / 2, Rand((windowY - 1)).val + maxY / 2);
        particles[i] = new Particle(circles[i].getX(),circles[i].getY(),particleMass, 0, 0);
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
    circles[0].setRadius(30);
    particles[0]->mass = 5000000000000000 * 1000;
    circles[0].setPosition(windowX / 2 + maxX / 2, windowY / 2 + maxY / 2);
    particles[0]->xVel = 0;
    particles[0]->yVel = 0;
    particles[0]->moveMe = false;
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

//==================================== Circles Functions ===================================================//
void changeCircleRadius(float amount, CircleShape* circles) {
    if (circles[0].getRadius() < 1) {
        circles[0].setRadius(1);
    }
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

//====================================== Main Function =========================================================//
int main()
{
    bool pause = false; // if simulation is paused
    int currentCycle = 0; // current simulations iteration
    int thickness = 10; // thickness of showRectangles's lines
    double totalTime = 0; // total time since start of simulation
    bool showRectangles = false; // shows algorithms's rectangles

    Mouse mouse;
    Keyboard k;
    RenderWindow window(VideoMode(maxX, maxY), "Gravity Gators");
    View innerView(FloatRect(200.f, 200.f, 300.f, 200.f)); // inner view to show particles
    View outerView(FloatRect(0, 0, 1000, 1000)); // outer view to show buttons, time, velocity, ect

    RectangleShape windowBorder; // border around inner view

    Font font; // font for all text
    font.loadFromFile("font.ttf");

    Text totalTimeText; // text to display total time
    totalTimeText.setFont(font);
    totalTimeText.setFillColor(Color::White);
    totalTimeText.setCharacterSize(24);
    totalTimeText.setPosition({ 100,100 });

    Text cyclesPerSecondText; // text to display cycles per seconde
    cyclesPerSecondText.setFont(font);
    cyclesPerSecondText.setFillColor(Color::White);
    cyclesPerSecondText.setCharacterSize(24);
    cyclesPerSecondText.setPosition({ 100,200 });

    Text iterationCountText; // text to display iteration count
    iterationCountText.setFont(font);
    iterationCountText.setFillColor(Color::White);
    iterationCountText.setCharacterSize(24);
    iterationCountText.setPosition({ 100,150 });

    vector<Text*> texts; // holds all texts
    texts.push_back(&totalTimeText);
    texts.push_back(&cyclesPerSecondText);
    texts.push_back(&iterationCountText);

    CircleShape tracker; // Circle that tracks particle
    tracker.setFillColor(Color::Transparent);
    tracker.setOutlineColor(Color::White);
    tracker.setOutlineThickness(20);

    CircleShape velocityModel; // model of particle being tracked
    velocityModel.setRadius(20);
    velocityModel.setPosition({ 800,800 });
    velocityModel.setOutlineColor(Color::White);
    velocityModel.setScale({ 1.3,2 });
    velocityModel.setOutlineThickness(2);
    velocityModel.setFillColor(Color(100, 100, 100));

    RectangleShape velocityDirectionModel; // dispalys velocity vector of particle being tracked
    velocityDirectionModel.setScale({ 1.3, 2 });
    velocityDirectionModel.setFillColor(Color(200, 200, 200));
    velocityDirectionModel.setSize({ 50,5 });
    velocityDirectionModel.setOrigin({ 0,2.5 });
    velocityDirectionModel.setPosition({ 825,840 });

    CircleShape* circles = new CircleShape[numOfParticles]; // circles used to print to window 
    Particle* particles[numOfParticles]; // stores x,y,xVel,yVel,mass 

    vector<Shape*> backgroundShapes; // holds all backround shapes
    vector<Shape*> innerShapes; // holds all inner shapes


    initializeWindowBorder(windowBorder); // sets of windowBorder

    innerShapes.push_back(&tracker);
    backgroundShapes.push_back(&windowBorder);
    backgroundShapes.push_back(&velocityModel);
    backgroundShapes.push_back(&velocityDirectionModel);


    initializeWindow(window, innerView, outerView); // MUST DO THIS, ELSE COMPUTER CRASHES??? 


    //funParticleInitialization(particles, circles); // any one of these initializes all the particles
    initializeParticles(particles, circles);
    //galaxyMergeInitialization(particles, circles);
    //quadgalaxyMergeInitialization(particles, circles);



    float avgXVel = 0; // average x velocity of system
    float avgYVel = 0; // average y velocity of system
    int cycleCounter = 0; // current cycles
    int particleIndexToObserve = 1; // index of particle to track

    auto start = std::chrono::system_clock::now(); // starting time

    while (true)
    {
        Event event; // Closes window if exity button clicked
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        moveWindow(k, window, innerView, outerView, circles, thickness, pause, mouse); // moves the window in cases of user input

        if (!pause) {


            bhTree* tree = new bhTree(1); // makes a new Barnes-Hill tree

            BarnesHut(tree, particles, circles); // simulates all particles using the barnes-hill tree

            deactivateOOB(particles); // deactivates out of bounds particles

            getAvgVel(particles, avgXVel, avgYVel);

            if (showRectangles) {
                tree->showRectangles(window, thickness); // shows Barnes-Hill algortithm calculations
            }

            setupTrakcer(tracker, circles, particleIndexToObserve);

            changeColorBasedOnVelocity(particles, circles, avgXVel, avgXVel); // changes oclor of particles based on average velocoties

            //clickToAddMass(particles, circles, m, window,innerView,true);

            clickToAddParticles(particles, circles, mouse, window, innerView, cycleCounter, false); // adds particles to simulation if inner view is clicked
        }
        drawCirlces(circles, window); // draws all circles onto the window

        setUpTexts(totalTimeText, cyclesPerSecondText, iterationCountText, totalTime, currentCycle);

        setUpDirectionalModel(velocityDirectionModel, particles, particleIndexToObserve);






        drawExtras(window, innerView, outerView, innerShapes, backgroundShapes, texts); // draws border around inner window
        //clampView(window,innerView, circles[particleIndexToObserve]);

        window.display();
        window.clear(); // fix for camaera move 

        std::chrono::duration<double> diff = chrono::system_clock::now() - start;
        start = chrono::system_clock::now();

        if (!pause) {
            totalTime += diff.count();
        }
        if (!pause) {
            ++currentCycle;
        }
        if (currentCycle == cyclesToDo) {
            return 0;
        }


    }

    return 0;


}
