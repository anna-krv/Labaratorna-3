//Made by Anna Kravets, K-22

#pragma once
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <E:\2\SFML-2.5.0\include\SFML\Main.hpp>
#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <memory>
#include <string>
using namespace std;

//************For Fortune Algorithm*************

class Parabola;

const float INF = FLT_MAX;
const sf::Vector2f infPoint=sf::Vector2f(INF, INF);

//for Fortune Animation
extern vector<vector<sf::Vertex>> animationFortune;
extern vector<vector<sf::CircleShape>> animationPointsFortune;
extern vector<sf::Vertex> tmpGoodFortune;
extern vector<sf::CircleShape> tmpGoodPointsFortune;
extern vector<vector<Parabola>> parabolas;
extern vector<Parabola> tmpParabolas;

class Parabola{
private:
	const static int accuracy = 80;
	vector<sf::Vertex> vertexes;
	sf::Vector2f focus;
	double y;
public:
	Parabola(sf::Vector2f focus_, double y_) {
		focus.x = focus_.x;
		focus.y = focus_.y;
		y = y_;
		float wsize = 490.;
		float  step = wsize / accuracy;
		float  x = focus.x - step*(accuracy / 2.);
		for (int i = 0; i < accuracy; i++) {
			float yNew = (x*x - 2. * x*focus.x + focus.x*focus.x + focus.y*focus.y - y*y) / 2. / (focus.y - y);
			if (yNew<=480. && yNew>=0.)
				vertexes.push_back(sf::Vertex(sf::Vector2f(x, yNew), sf::Color::Black));
			x += step;
		}
	}
	void draw();
};

//models a segment that connects 2 points
class Segment {
public:
	Segment() {}
	Segment(sf::Vector2f p1_,
		sf::Vector2f p2_) :p1(p1_), p2(p2_) {	}
	sf::Vector2f p1, p2;
};


class EdgeNode {
public:
	EdgeNode();
	EdgeNode(const sf::Vector2f& p1_,
		const sf::Vector2f& site_);
	EdgeNode(const sf::Vector2f& p1_,
		const sf::Vector2f& p2_, const sf::Vector2f& site_);
	void finish(const sf::Vector2f& p2_);
protected:
	Segment edge;
	shared_ptr<EdgeNode> twin;
	shared_ptr<sf::Vector2f> site;
	bool finished;
};

//Double-Connected Edge List
class DCEL {
public:
	DCEL();
	void addEdge(const EdgeNode& newEdge);
private:
	list<EdgeNode> edges;
};



class Event {
public:
	Event(const sf::Vector2f& v);
	bool operator<(const Event& other) const;
	void handle();
	virtual sf::Vector2f getPointOfEvent() const;
protected:
	sf::Vector2f pointOfEvent;
};

class pointEvent :public Event {
public:
	pointEvent(const sf::Vector2f& site);
};

class circleEvent : public Event {
public:
	circleEvent(const sf::Vector2f& newVoronoiVertex);
};

class Arch {
public:
	Arch();
	Arch(const pointEvent& focus_);
	float x() const;
	float y() const;
	bool operator==(const Arch& other) const;
	bool operator!=(const Arch& other) const;
private:
	shared_ptr<sf::Vector2f> focus;
	shared_ptr<EdgeNode> leftEdge, rightEdge;
};

class BeachSearchTree;

class BeachNode {
public:
	BeachNode();
	BeachNode(const Arch& arch);
	BeachNode(const sf::Vector2f breakPoint);
private:
	shared_ptr<Arch> arch;
	sf::Vector2f breakpoint;
	shared_ptr<BeachNode> parent;
	shared_ptr<BeachNode> left;
	shared_ptr<BeachNode> right;
	shared_ptr<EdgeNode> edge;
	shared_ptr<circleEvent> myCircleEvent;
	friend class BeachSearchTree;
};

class BeachSearchTree {
public:
	BeachSearchTree();
	BeachSearchTree(const shared_ptr<BeachNode>& beachNode);
	void addArch(const Arch& myArch);
	void deleteArch(const Arch& myArch);
	shared_ptr<BeachNode> getLeftmostLeaf() const;
	shared_ptr<BeachNode> getNextLeaf() const;
	void handlePointEvent(const pointEvent& myPointEvent,
		deque<Event> eventsQueue, DCEL& edges);
	void handleCircleEvent(const circleEvent& myCircleEvent,
		deque<Event> eventsQueue, DCEL& edges);
private:
	shared_ptr<BeachNode> root;
	//returns BeachNode that corresponds to an arch 
	//that is above myX coordinate
	shared_ptr<BeachNode> searchPlaceForArch(double myX) const;
	shared_ptr<BeachNode> searchArch(const Arch& myArch) const;
};



//Fortune algorithm
vector<sf::ConvexShape> FortuneBuildDiagram(sf::CircleShape myPoints[], int nElem);



//************For Slow Algorithm**************

//number of points
const int maxNum = 6;
extern sf::RenderWindow window;
//window's size
extern double wSize, hSize;

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

//slow algorithm
vector<sf::ConvexShape> buildDiagram(const sf::CircleShape myPoints[], int nElem);
