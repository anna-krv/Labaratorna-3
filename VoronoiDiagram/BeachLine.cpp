//Made by Anna Kravets, K-22

#include "VoronoiDiagram.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

//*************Segment*************

Segment::Segment(sf::Vector2f start_)
	:start(start_), end(sf::Vector2f(0.f, 0.f)) {

}

void Segment::finish(sf::Vector2f end_) {
	if (!finished) {
		end = end_;
		finished = true;
	}
	FortuneFinishedEdges.push_back(this);
}

vector<sf::Vertex> Segment::buildSeg(sf::Vector2f end_) {
	if (!this)
		return {};
	sf::Vertex v1(start, sf::Color::Magenta);
	vector<sf::Vertex> myVec = { v1, sf::Vertex(end_, sf::Color::Magenta) };
	return myVec;
}

vector<sf::Vertex> Segment::build() {
	if (!this)
		return {};
	sf::Vertex v1(start, sf::Color::Magenta);
	vector<sf::Vertex> myVec = { v1, sf::Vertex(end, sf::Color::Magenta) };
	return myVec;
}


//**************Arch****************
Arch::Arch(sf::Vector2f focus_, Arch* prev_, Arch* next_)
	:focus(focus_), prev(prev_), next(next_),
	e(nullptr), s0(nullptr), s1(nullptr) {}

vector<sf::Vertex> Arch::buildVertexes() {
	vector<sf::Vertex> vertexes;
	double yNew = 0., x = 0.;
	//if the Arch is just a line
	if (fabs(y - focus.y) < 0.0001) {
		xBreak = focus.x;
		if (prev)
			yNew = (focus.x*focus.x - 2 * focus.x*prev->focus.x + prev->focus.x*prev->focus.x + prev->focus.y*prev->focus.y - y*y) / 2 / (prev->focus.y - y);
		else if (next)
			yNew = (focus.x*focus.x - 2 * focus.x*next->focus.x + next->focus.x*next->focus.x + next->focus.y*next->focus.y - y*y) / 2 / (next->focus.y - y);
		vertexes.push_back(sf::Vertex(sf::Vector2f(focus.x, yNew), sf::Color::Green));
		vertexes.push_back(sf::Vertex(sf::Vector2f(focus.x, y), sf::Color::Green));
		return vertexes;
	}
	if (prev)
		x = prev->xBreak;
	if (next)
		xBreak = xCoordIntersection(this, this->next, y);
	else
		xBreak = wSize;
	if (x < 0)
		x = 0.;
	double  step;
	if (xBreak < wSize)
		step = (xBreak - x) / accuracy;
	else
		step = (wSize - x) / accuracy;
	for (int i = 0; i <= accuracy; i++) {
		yNew = (x*x - 2 * x*focus.x + focus.x*focus.x + focus.y*focus.y - y*y) / 2 / (focus.y - y);
		if (yNew <= hSize && yNew >= 0.)
			vertexes.push_back(sf::Vertex(sf::Vector2f(x, yNew), sf::Color::Green));
		x += step;
	}
	return vertexes;
}