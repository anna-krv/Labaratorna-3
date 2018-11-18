//Made by Anna Kravets, K-22
#include "VoronoiDiagram.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
//#include <E:\2\SFML-2.5.0\include\SFML\Main.hpp>
#include <iostream>
#include <algorithm>
#include <deque>
#include <memory>

//for Fortune Animation
vector<vector<sf::Vertex>> animationFortune = {};
vector<vector<sf::CircleShape>> animationPointsFortune = {};
vector<sf::Vertex> tmpGoodFortune = {};
vector<sf::CircleShape> tmpGoodPointsFortune = {};
vector<vector<Parabola>> parabolas = {};
vector<Parabola> tmpParabolas = {};


bool fun(sf::CircleShape s1, sf::CircleShape s2) {
	return s1.getPosition().y < s2.getPosition().y;
}

void mySort(sf::CircleShape myPoints[], int nElem) {
	sort(myPoints, myPoints + nElem, fun);
}

deque<Event> createQueue(sf::CircleShape myPoints[], int nElem) {
	deque<Event> eventsQueue;
	mySort(myPoints, nElem);
	for (int i = 0; i < nElem; i++) {
		eventsQueue.push_back(myPoints[i].getPosition());
		tmpGoodPointsFortune.push_back(myPoints[i]);
	}
	return eventsQueue;
}

vector<sf::ConvexShape> FortuneBuildDiagram(sf::CircleShape myPoints[], int nElem) {
	vector<sf::ConvexShape> myDiagram;
	deque<Event> eventsQueue = createQueue(myPoints, nElem);
	shared_ptr<Event> curEvent;
	BeachSearchTree beachLine;
	DCEL edges;
	int i = 0;
	while (!eventsQueue.empty()) {
		//for animation
		eventsQueue.front().handle();
		tmpParabolas.clear();
		for (int ii = 0; ii <= i; ii++) {
			Parabola p(myPoints[ii].getPosition(),
				myPoints[i].getPosition().y+1);
			tmpParabolas.push_back(p);
		}
		parabolas.push_back(tmpParabolas);
		i++;
		//real algorithm
		curEvent = make_shared<Event>(eventsQueue.front());
		shared_ptr<const pointEvent> isPointEvent(dynamic_cast<const pointEvent*>(curEvent.get()));
		if (isPointEvent) {
			eventsQueue.pop_front();
			beachLine.handlePointEvent(*isPointEvent, eventsQueue, edges);
		}
		else {
			shared_ptr<const circleEvent> isCircleEvent(dynamic_cast<const circleEvent*>(curEvent.get()));
			eventsQueue.pop_front();
			beachLine.handleCircleEvent(*isCircleEvent, eventsQueue, edges);
		}
	}
	return myDiagram;
}