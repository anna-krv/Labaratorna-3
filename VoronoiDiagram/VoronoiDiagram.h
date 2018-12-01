//Made by Anna Kravets, K-22

#pragma once
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

//number of points
const int maxNum = 30;
extern sf::RenderWindow window;
//window's size
extern double wSize, hSize;

//************Fortune Algorithm*************

class Event;
class Arch;
class Segment;
class VoronoiDiagram;

class Event {
public:
	Event(double y_, sf::Vector2f point_, Arch* myArch_);
	double getY() const;
	sf::Vector2f point;
	Arch* myArch;
	bool valid;
private:
	double y;
};

//Greater Than
struct gt {
	bool operator()(sf::Vector2f v1, sf::Vector2f v2) {
		return (v1.y > v2.y) || (v1.y == v2.y && v1.x > v2.x);
	}
	bool operator()(Event *e1, Event* e2) {
		return e1->getY() > e2->getY();
	}
};

class VoronoiDiagram {
public:
	VoronoiDiagram(sf::CircleShape myPoints[], int nElem);
	//Fortune algorithm
	void FortuneBuildDiagram();
private:
	//first item in the beachline
	Arch *root;
	priority_queue<sf::Vector2f, vector<sf::Vector2f>, gt> siteEvents;
	priority_queue<Event*, vector<Event*>, gt> circleEvents;
	//looks for a new circle event for Arch a
	//at y-coordinate>y0
	void checkCircleEvent(Arch* a, double y0);
	void processSite();
	void processCircle();
	void beachLineInsert(sf::Vector2f v);
	void finishEdges();
};


class Arch {
public:
	Arch(sf::Vector2f focus_, Arch* prev_ = nullptr, Arch* next_ = nullptr);
	sf::Vector2f focus;
	Arch *prev, *next;
	Event* e;
	Segment *s0, *s1;
	double xBreak; //the right break for arch 
	vector<sf::Vertex> buildVertexes();
	double y; //the directrissa of arch
private:
	const static int accuracy = 1000;
};


//models a segment that connects 2 points
class Segment {
public:
	Segment(sf::Vector2f start_);
	Segment(sf::Vector2f start_, sf::Vector2f end_);
	void finish(sf::Vector2f end_);
	vector<sf::Vertex> buildSeg(sf::Vector2f end_);
	//when finished
	vector<sf::Vertex> build();
private:
	bool finished = false;
	sf::Vector2f start, end;
};

const float INF = FLT_MAX;

//helpful functions for Fortune's algorithm
pair<double, double> intersection(sf::Vector2f v1, sf::Vector2f v2, double y);
bool intersect(sf::Vector2f v, Arch* a);
bool circle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, double* y, sf::Vector2f* o);
//pre: first is on the left, second is on the right
//pre: first->prev->xBreak is already calculated
double xCoordIntersection(Arch* first, Arch* second, double y);

//for Fortune Animation
void updateAnimation(Arch* root, double y);
extern vector<vector<sf::Vertex>> sweepLine;
extern vector<sf::CircleShape> points;
extern vector<vector<vector<sf::Vertex>>> FortuneBeachLine;
extern vector<Segment*> FortuneFinishedEdges;
extern vector<vector<vector<sf::Vertex>>> FortuneEdges;

vector <vector<sf::Vertex>> saveCurFortuneFinishedEdges();
vector < vector<sf::Vertex> > saveCurFortuneBeachLine(Arch* root, double y);


//************Naive Algorithm**************

//for Animation
extern vector<vector<sf::ConvexShape>> animation;
extern vector<vector<sf::CircleShape>> animationPoints;
extern vector<sf::ConvexShape> tmpGood;
extern vector<sf::CircleShape> tmpGoodPoints;
extern double scale[2];
extern int iScale;


//fills array with nElem circles at random positions
void fill(sf::CircleShape myPoints[], int nElem);

//builds convex from vector of coordinates, with Color col
sf::ConvexShape makeConvex(vector<vector<double>> v, sf::Color col);

//checks if (x1, y1) and (x2, y2) are on one side 
//corresponding the line ax+by+c=0
bool onOneSide(double a, double b, double c,
	double x1, double y1, double x2, double y2);

//pre: myVec consists of vectors<double> of size 2
//pre: checkV.size()==2
//checks if myVec contains checkV
bool checkIfIn(const vector<vector<double>> myVec, vector<double> checkV);

//finds the intersection between 
//line that contains 2 points: (x1, y1) and (x2, y2) 
//and line ax+by+c=0
//post:throws exception if lines are parallel
sf::Vector2f findIntersection(double x1, double y1, double x2, double y2,
	double a, double b, double c);

//checks if (x,y) lies on segment
//that connects (x1,y1) and (x2, y2)
bool checkIfOnSegment(double x1, double y1,
	double x2, double y2, double x, double y);

vector<vector<double>> checkLine(int i, sf::Color col, vector<vector<double>> v,
	vector<double> iPoint, double a, double b, double c);


vector<vector<double>> myVec(sf::Color col, int i,
	const sf::CircleShape myPoints[], int nElem);

sf::ConvexShape findConvex(int i, const sf::CircleShape myPoints[], int nElem);

//naive algorithm
vector<sf::ConvexShape> buildDiagram(const sf::CircleShape myPoints[], int nElem);