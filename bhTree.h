#pragma once
//==================== Physics Constants =========================//
#define G 0.000000000006674
#define softenRad 5000000000// very important, make it reall cool
#define softenFactor 1000 // very important, make it reall cool, if 5000/1000 cool image

//==================== Includes =========================//
#include "Particle.h"
#include <math.h>
#include <queue>
using namespace std;
//==================== Gravit Calculator Function =========================//
void calcGrav(float& ax, float& ay, double& amass, Particle& b) {

	float r = std::sqrt((ax - *b.x) * (ax - *b.x) + (ay - *b.y) * (ay - *b.y));
	if (r == 0) {
		return;
	}
	float force = (amass * b.mass * G) / (r * r);
	if (r <= softenRad) {
		force = (G * amass * b.mass * r) / std::pow(r * r + softenFactor * softenFactor, 1.5);
	}

	float xForce = ((ax - *b.x) / r * force);
	float yForce = ((ay - *b.y) / r * force);

	b.applyGravity(xForce, yForce);
}

//==================== Barnes-Hill Tree =========================//
class bhTree {

	//==================== Tree Node =========================//
	class Node {
	public:
		float x0; // cooridnates of the node's rectangle 
		float x1;
		float y0;
		float y1;

		float centerOfMassX; // masses of the particle
		float centerOfMassY;
		double totalMass;

		Node* ne; // pointers to node's inner four sub-spaces
		Node* se;
		Node* sw;
		Node* nw;

		Particle* particle; // pointer to the node's particle

		Node() : x0(0), x1(maxX), y0(0), y1(maxY), centerOfMassX(0), centerOfMassY(0), totalMass(0), particle(nullptr), ne(nullptr), se(nullptr), sw(nullptr), nw(nullptr) {};
		Node(float x0_, float x1_, float y0_, float y1_) : x0(x0_), x1(x1_), y0(y0_), y1(y1_) {};

	};

public:
	Node* root; // root of the tree
	float theta; // measure of how accurate the tree is 

	~bhTree() { // destructor 
		delete root;
		theta = 0;
		root = nullptr;

	}
	bhTree(float theta_) : theta(theta_) { // constructor 
		root = new Node();
	}
	Node* insert(Node* root, Particle* particle) { // inserts particles into the tree

		if (abs(root->x1 - root->x0) < 1 || abs(root->y1 - root->y0) < 1) { // prevents inf recursion due to tiny distances 
			root->particle = particle;
			return root;
		}
		if (root->particle == nullptr && root->ne == nullptr && root->nw == nullptr && root->se == nullptr && root->sw == nullptr) {
			root->particle = particle;
			return root;
		}
		if (root->ne != nullptr || root->se != nullptr || root->sw != nullptr || root->nw != nullptr) { // internal node //
			root->centerOfMassX = ((root->centerOfMassX * root->totalMass) + (particle->mass * *particle->x)) / (root->totalMass + particle->mass); // recalculates center of mass;
			root->centerOfMassY = ((root->centerOfMassY * root->totalMass) + (particle->mass * *particle->y)) / (root->totalMass + particle->mass); // recalculates center of mass;
			root->totalMass += particle->mass;
			if (*particle->x > ((root->x1 + root->x0) / 2)) {
				if (*particle->y > ((root->y1 + root->y0) / 2)) {
					if (root->se == nullptr) {
						root->se = insert(new Node(((root->x0 + root->x1) / 2), root->x1, ((root->y1 + root->y0) / 2), root->y1), particle);
						return root;
					}
					else {
						root->se = insert(root->se, particle);
						return root;
					}

				}
				else {
					if (root->ne == nullptr) {
						root->ne = insert(new Node(((root->x0 + root->x1) / 2), root->x1, root->y0, ((root->y1 + root->y0) / 2)), particle);
						return root;
					}
					else {
						root->ne = insert(root->ne, particle);
						return root;
					}
				}
			}
			else {
				if (*particle->y > ((root->y1 + root->y0) / 2)) {
					if (root->sw == nullptr) {
						root->sw = insert(new Node(root->x0, ((root->x0 + root->x1) / 2), ((root->y1 + root->y0) / 2), root->y1), particle);
						return root;
					}
					else {
						root->sw = insert(root->sw, particle);
						return root;
					}
				}
				else {
					if (root->nw == nullptr) {
						root->nw = insert(new Node(root->x0, ((root->x0 + root->x1) / 2), root->y0, ((root->y1 + root->y0) / 2)), particle);
						return root;
					}
					else {
						root->nw = insert(root->nw, particle);
						return root;
					}
				}
			}
		}
		else {
			if (root->particle->x == particle->x && root->particle->y == particle->y) {
				return root;
			}
			Particle* temp = root->particle;  // external node, has to insert both particles now deeper into the tree //
			root->particle = nullptr;




			if (*particle->x > ((root->x1 + root->x0) / 2)) {  // insert first particle //
				if (*particle->y > ((root->y1 + root->y0) / 2)) {

					root->se = insert(new Node(((root->x0 + root->x1) / 2), root->x1, ((root->y1 + root->y0) / 2), root->y1), particle);

				}
				else {
					root->ne = insert(new Node(((root->x0 + root->x1) / 2), root->x1, root->y0, ((root->y1 + root->y0) / 2)), particle);

				}
			}
			else {
				if (*particle->y > ((root->y1 + root->y0) / 2)) {
					root->sw = insert(new Node(root->x0, ((root->x0 + root->x1) / 2), ((root->y1 + root->y0) / 2), root->y1), particle);

				}
				else {
					root->nw = insert(new Node(root->x0, ((root->x0 + root->x1) / 2), root->y0, ((root->y1 + root->y0) / 2)), particle);

				}
			}
			if (*temp->x > ((root->x1 + root->x0) / 2)) { // inserts seconf particle, must check if first particle was inserted in samesame quadrant as this one should be //
				if (*temp->y > ((root->y1 + root->y0) / 2)) {
					if (root->se != nullptr) {
						root->se = insert(root->se, temp);
					}
					else {
						root->se = insert(new Node(((root->x0 + root->x1) / 2), root->x1, ((root->y1 + root->y0) / 2), root->y1), temp);
					}
				}
				else {
					if (root->ne != nullptr) {
						root->ne = insert(root->ne, temp);
					}
					else {
						root->ne = insert(new Node(((root->x0 + root->x1) / 2), root->x1, root->y0, ((root->y1 + root->y0) / 2)), temp);
					}
				}
			}
			else {
				if (*temp->y > ((root->y1 + root->y0) / 2)) {
					if (root->sw != nullptr) {
						root->sw = insert(root->sw, temp);
					}
					else {
						root->sw = insert(new Node(root->x0, ((root->x0 + root->x1) / 2), ((root->y1 + root->y0) / 2), root->y1), temp);
					}
				}
				else {
					if (root->nw != nullptr) {
						root->nw = insert(root->nw, temp);
					}
					else {
						root->nw = insert(new Node(root->x0, ((root->x0 + root->x1) / 2), root->y0, ((root->y1 + root->y0) / 2)), temp);
					}
				}
			}
			root->centerOfMassX = ((root->centerOfMassX * root->totalMass) + (particle->mass * *particle->x)) / (root->totalMass + particle->mass); // recalculates center of mass;
			root->centerOfMassY = ((root->centerOfMassY * root->totalMass) + (particle->mass * *particle->y)) / (root->totalMass + particle->mass); // recalculates center of mass;
			root->totalMass += particle->mass;
		}
		return root;
	}
	void preOrderPrint(Node* root, int& n, bool print) { // prints tree 
		if (root == nullptr) {
			return;
		}
		if (root->particle != nullptr) {
			++n;
			if (print) {
				cout << "Particle" << endl;
				cout << "x: " << root->particle->x << endl;
				cout << "y: " << root->particle->y << endl;
				cout << "mass: " << root->particle->mass << endl;
				cout << "xVel: " << root->particle->xVel << endl;
				cout << "yVel: " << root->particle->yVel << endl;
			}
		}
		else {
			if (print) {
				cout << "Internal Node" << endl;
				cout << "x0: " << root->x0 << endl;
				cout << "yo: " << root->y0 << endl;
				cout << "x1: " << root->x1 << endl;
				cout << "y1: " << root->y1 << endl;
				cout << "ne" << endl;
			}
			preOrderPrint(root->ne, n, print);
			if (print) {
				cout << "se" << endl;
			}
			preOrderPrint(root->se, n, print);
			if (print) {
				cout << "sw" << endl;
			}
			preOrderPrint(root->sw, n, print);
			if (print) {
				cout << "nw" << endl;
			}
			preOrderPrint(root->nw, n, print);
		}
	}
	void applyGrav(Node* root, Node* particleNode) { // applies gravity to a node in the tree
		if (root == nullptr) {
			return;
		}
		if (root->particle != nullptr && root->particle != particleNode->particle) { // if external node and not particleNode
			calcGrav(*root->particle->x, *root->particle->y, root->particle->mass, *particleNode->particle);
		}
		else {
			if ((root->x1 - root->x0) / sqrt((root->centerOfMassX - *particleNode->particle->x) * (root->centerOfMassX - *particleNode->particle->x) + (root->centerOfMassY - *particleNode->particle->y) * (root->centerOfMassY - *particleNode->particle->y)) < theta) {
				calcGrav(root->centerOfMassX, root->centerOfMassY, root->totalMass, *particleNode->particle);
			}
			else {
				if (root->ne != nullptr) {
					applyGrav(root->ne, particleNode);
				}
				if (root->nw != nullptr) {
					applyGrav(root->nw, particleNode);
				}
				if (root->sw != nullptr) {
					applyGrav(root->sw, particleNode);
				}
				if (root->se != nullptr) {
					applyGrav(root->se, particleNode);
				}

			}
		}
	}
	void applyGravAll(Node* node) { // apply gravity on all external nodes //
		if (node == nullptr) {
			return;
		}
	
		if (node->se != nullptr) {
			applyGravAll(node->se);
		}
		if (node->sw != nullptr) {
			applyGravAll(node->sw);
		}
		if (node->ne != nullptr) {
			applyGravAll(node->ne);
		}
		if (node->nw != nullptr) {
			applyGravAll(node->nw);
		}
		if (node->particle != nullptr) { // if internal //
			applyGrav(root, node);
		}

	}
	void showRectangles(RenderWindow& window, int thickness) { // draws the rectangles used in the calculations
		queue<Node*> q;

		RectangleShape rect;
		rect.setFillColor(Color::Transparent);
		rect.setOutlineColor(Color::Green);
		rect.setOutlineThickness(thickness);
		q.push(root);
		while (!q.empty()) {

			rect.setPosition(Vector2f(q.front()->x0, q.front()->y0));
			rect.setSize(Vector2f(q.front()->x1 - q.front()->x0, q.front()->x1 - q.front()->x0));
			window.draw(rect);
			if (q.front()->ne != nullptr) {
				if (q.front()->ne->particle == nullptr) {
					q.push(q.front()->ne);
				}
			}
			if (q.front()->nw != nullptr) {
				if (q.front()->nw->particle == nullptr) {
					q.push(q.front()->nw);
				}
			}if (q.front()->sw != nullptr) {
				if (q.front()->sw->particle == nullptr) {
					q.push(q.front()->sw);
				}
			}if (q.front()->se != nullptr) {
				if (q.front()->se->particle == nullptr) {
					q.push(q.front()->se);
				}
			}
			q.pop();
		}
	}

};
