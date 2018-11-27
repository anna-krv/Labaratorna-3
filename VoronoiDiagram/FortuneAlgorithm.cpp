//Made by Anna Kravets, K-22

#include "VoronoiDiagram.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>
using namespace std;

//for Fortune Animation
vector<vector<sf::Vertex>> sweepLine = {};
vector<sf::CircleShape> pointsFortune = {};
vector<vector<vector<sf::Vertex>>> FortuneBeachLine = {};
vector<vector<vector<sf::Vertex>>> FortuneEdges = {};
vector<Segment*> FortuneFinishedEdges = {};


//for animation

void updateAnimation(Arch* root, double y) {
	sf::Vertex v1(sf::Vector2f(0, y), sf::Color::Red);
	sf::Vertex v2(sf::Vector2f(wSize, y), sf::Color::Red);
	sweepLine.push_back({ v1,v2 });
	FortuneBeachLine.push_back(saveCurFortuneBeachLine(root, y));
	FortuneEdges.push_back(saveCurFortuneFinishedEdges());
}

vector <vector<sf::Vertex>> saveCurFortuneFinishedEdges() {
	vector <vector<sf::Vertex>> myVector;
	for (int i = 0; i < FortuneFinishedEdges.size(); i++) {
		myVector.push_back(FortuneFinishedEdges[i]->build());
	}
	return myVector;
}

//saves current arch and segments corresponding to them
vector < vector<sf::Vertex> > saveCurFortuneBeachLine(Arch* root, double y) {
	Arch* a = root;
	vector < vector<sf::Vertex> > myVector;
	while (a) {
		//update y and calculate vertexes
		a->y = y;
		myVector.push_back(a->buildVertexes());
		//push segments
		if (abs(a->focus.y - y) > 0.000001) {
			sf::Vector2f end_;
			double x_, y_;
			if (a->prev) {
				x_ = a->prev->xBreak;
				y_ = (x_*x_ - 2 * x_*a->focus.x + a->focus.x*a->focus.x + a->focus.y*a->focus.y - y*y) / 2 / (a->focus.y - y);
				myVector.push_back(a->s0->buildSeg(sf::Vector2f(x_, y_)));
			}
			if (a->next) {
				x_ = a->xBreak;
				y_ = (x_*x_ - 2 * x_*a->focus.x + a->focus.x*a->focus.x + a->focus.y*a->focus.y - y*y) / 2 / (a->focus.y - y);
				myVector.push_back(a->s1->buildSeg(sf::Vector2f(x_, y_)));
			}
		}
		a = a->next;
	}
	return myVector;
}


//pre: xBreak is set for a->prev
bool myCheck(Arch* a, sf::Vector2f o) {
	//check that two breakpoints do really merge!!!!
	double x1 = a->prev->focus.y - a->focus.y;
	return (o.x - a->prev->xBreak) / x1 > 0;
}


//find the lowest point on circle through a,b,c
bool circle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, double* y, sf::Vector2f* o) {
	double d2 = a.y - b.y, d3 = a.y - c.y;
	//if a and c correspond to the same archs
	if (abs(d2) < 0.00001 || abs(d3) < 0.00001)
		return false;
	double F1 = (a.x*a.x + a.y*a.y - b.x*b.x - b.y*b.y) / 2. / d2;
	double F2 = (c.x*c.x + c.y*c.y - a.x*a.x - a.y*a.y) / 2. / d3;
	double G1 = (a.x - b.x) / d2;
	double G2 = (a.x - c.x) / d3;
	//if points are colinear
	if (abs(G1 - G2) < 0.000001)
		return false;
	o->x = (F1 + F2) / (G1 - G2);
	o->y = F1 - o->x*G1;
	*y = o->y + sqrt(pow((o->x - a.x), 2.) + pow((o->y - a.y), 2.));
	return true;
}


//pre: first is on the left, second is on the right
//pre: they really Do intersect at point whith suitable coordinates
//pre: first->prev->xBreak is already calculated
//post: first->xBreak is calculated
double xCoordIntersection(Arch* first, Arch*second, double y) {
	pair<double, double> myPair = intersection(first->focus, second->focus, y);
	if (abs(first->focus.y - y)<0.00001)
		first->xBreak = first->focus.x;
	else if (abs(second->focus.y - y)<0.00001)
		first->xBreak = second->focus.x;
	else if (!first->prev || myPair.first - first->prev->xBreak>0.0001)
		first->xBreak = myPair.first;
	else
		first->xBreak = myPair.second;
	return first->xBreak;
}

//Where do two archs intersect?
//focuses of archs are v1 and v2
//line comes through y
//post: returns pair(sf::Vector2f(-1.f, -1.f), sf::Vector2f(-1.f, -1.f))
//if archs do not intersect at all
//post: pair.first has smaller x-coordinate
pair<double, double> intersection(sf::Vector2f v1, sf::Vector2f v2, double y) {
	double d1, d2;
	if (abs(v1.y - y) < 0.00001)
		return make_pair(v1.x, v1.x);
	if (abs(v2.y - y) < 0.00001)
		return make_pair(v2.x, v2.x);
	d1 = 1. / 2. / (v1.y - y);
	d2 = 1. / 2. / (v2.y - y);
	double a, b, c;
	a = d1 - d2;
	b = 2 * v2.x*d2 - 2 * v1.x*d1;
	c = (v1.x*v1.x + v1.y*v1.y - y*y)*d1 - (v2.x*v2.x + v2.y*v2.y - y*y)*d2;
	double d = b*b - 4. * a*c;
	//if they do not intersect
	if ((abs(a) < 0.0000001 && abs(b) < 0.000000001) || abs(d)<0.00000001) {
		return make_pair(INF, INF);
	}
	if (abs(a) < 0.00000001) {
		return make_pair(-c / b, -c / b);
	}
	d = sqrt(d);
	double x1 = (-b - d) / 2. / a, x2 = (-b + d) / 2. / a;
	if (x1 > x2) {
		double z = x1; x1 = x2; x2 = z;
	}
	return make_pair(x1, x2);
}

//Will a new arch with focus in v intersect a????
bool intersect(sf::Vector2f v, Arch* a) {
	if (abs(a->focus.y - v.y) <= 0.001)
		return false;
	double x1, x2;
	if (a->prev)
		x1 = xCoordIntersection(a->prev, a, v.y);
	if (a->next)
		x2 = xCoordIntersection(a, a->next, v.y);
	if ((!a->prev || v.x >= x1 || abs(x1 + 1)<0.0001) && (!a->next || v.x <= x2 || abs(x2 + 1)<0.0001)) {
		return true;
	}
	return false;
}


bool fun(sf::CircleShape s1, sf::CircleShape s2) {
	return s1.getPosition().y < s2.getPosition().y;
}

void mySort(sf::CircleShape myPoints[], int nElem) {
	sort(myPoints, myPoints + nElem, fun);
}

priority_queue<sf::Vector2f, vector<sf::Vector2f>, gt> createQueue(sf::CircleShape myPoints[], int nElem) {
	priority_queue<sf::Vector2f, vector<sf::Vector2f>, gt> eventsQueue;
	mySort(myPoints, nElem);
	for (int i = 0; i < nElem; i++) {
		eventsQueue.push(myPoints[i].getPosition());
		pointsFortune.push_back(myPoints[i]);
	}
	return eventsQueue;
}

VoronoiDiagram::VoronoiDiagram(sf::CircleShape myPoints[],
	int nElem) :root(nullptr) {
	siteEvents = createQueue(myPoints, nElem);
}


void VoronoiDiagram::processSite() {
	sf::Vector2f v = siteEvents.top();
	siteEvents.pop();
	beachLineInsert(v);
	//for animation
	updateAnimation(root, v.y);
}


void VoronoiDiagram::checkCircleEvent(Arch* a, double y0) {
	//invalidate any old event
	if (a->e && (a->e->getY() - y0) < 0.00001)
		a->e->valid = false;
	a->e = nullptr;
	//easssyyy
	if (!a->prev || !a->next)
		return;
	double y;
	sf::Vector2f o;
	if (circle(a->prev->focus, a->focus, a->next->focus, &y, &o) && y > y0 && myCheck(a, o)) {
		//create a new event
		a->e = new Event(y, o, a);
		circleEvents.push(a->e);
	}
}

void VoronoiDiagram::processCircle() {
	//get the next circleEvent from the queue
	Event *myEvent = circleEvents.top();
	if (myEvent->valid) {
		//start a new edge
		Segment* s = new Segment(myEvent->point);
		//remove the associated arch from the BeachLine
		Arch* a = circleEvents.top()->myArch;
		if (a->prev) {
			a->prev->next = a->next;
			//new edge
			a->prev->s1 = s;
		}
		if (a->next) {
			a->next->prev = a->prev;
			//new edge
			a->next->s0 = s;
		}
		//finish edges that belong to a
		if (a->s0)
			a->s0->finish(myEvent->point);
		if (a->s1)
			a->s1->finish(myEvent->point);
		//Recheck circle events on both sides of myEvent->point
		if (a->prev)
			checkCircleEvent(a->prev, circleEvents.top()->getY());
		if (a->next)
			checkCircleEvent(a->next, circleEvents.top()->getY());
		//for animation
		updateAnimation(root, circleEvents.top()->getY());
	}
	//release memory
	circleEvents.pop();
	delete myEvent;
}

void VoronoiDiagram::beachLineInsert(sf::Vector2f v) {
	if (!root) {
		root = new Arch(v);
	}
	//find the current arch at above v.x coordinate
	for (Arch* a = root; a; a = a->next) {
		if (intersect(v, a)) {
			//duplicate a if necessary
			if (a->next && !intersect(v, a->next)) {
				a->next->prev = new Arch(a->focus, a, a->next);
				a->next = a->next->prev;
			}
			else
				a->next = new Arch(a->focus, nullptr, a->next);
			//set its edge
			a->next->s1 = a->s1;
			//add v between a and a->next
			a->next->prev = new Arch(v, a, a->next);
			a->next = a->next->prev;
			//set breaks
			a->xBreak = v.x;
			//a points to the new arch
			a = a->next;
			a->xBreak = v.x;
			//calculate intersection point
			sf::Vector2f p;
			p.x = v.x;
			p.y = (v.x*v.x - 2 * v.x*a->prev->focus.x + a->prev->focus.x*a->prev->focus.x + a->prev->focus.y*a->prev->focus.y - v.y*v.y) / 2 / (a->prev->focus.y - v.y);
			//add new half-edges connected to a's 
			a->prev->s1 = new Segment(p); a->s0 = new Segment(p);
			a->next->s0 = new Segment(p); a->s1 = new Segment(p);
			//check circle events around new arch
			checkCircleEvent(a->prev, v.y);
			checkCircleEvent(a->next, v.y);
			break;
		}
	}
}

void VoronoiDiagram::finishEdges() {
	double l = 2 * (hSize + wSize);
	for (Arch* a = root; a->next; a = a->next) {
		if (a->s1) {
			sf::Vector2f end_;
			double x_ = xCoordIntersection(a, a->next, l);
			double y_ = (x_*x_ - 2 * x_*a->focus.x + a->focus.x*a->focus.x + a->focus.y*a->focus.y - l*l) / 2 / (a->focus.y - l);
			end_.x = x_; end_.y = y_;
			a->s1->finish(end_);
		}
	}
	//for animation
	updateAnimation(root, l);
}

void VoronoiDiagram::FortuneBuildDiagram() {
	while (!siteEvents.empty()) {
		if (!circleEvents.empty() && circleEvents.top()->getY() <= siteEvents.top().y)
			processCircle();
		else
			processSite();
	}
	while (!circleEvents.empty())
		processCircle();
	finishEdges();
}