//Made by Anna Kravets, K-22

#include "VoronoiDiagram.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <E:\2\SFML-2.5.0\include\SFML\Main.hpp>
#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <cmath>
#include <memory>
#include <string>
using namespace std;

//*********EdgeNode********

EdgeNode::EdgeNode() :edge(), twin(nullptr), site(nullptr), 
              finished(false) {}

EdgeNode::EdgeNode(const sf::Vector2f& p1_,
	const sf::Vector2f& site_):twin(nullptr), finished(false) {
	edge.p1 = p1_;
	site = make_shared<sf::Vector2f>(site_);
}

EdgeNode::EdgeNode(const sf::Vector2f& p1_,
	const sf::Vector2f& p2_, const sf::Vector2f& site_)
           :edge(p1_,p2_),twin(make_shared<EdgeNode>(p2_,p1_)),
		   site(make_shared<sf::Vector2f>(site_)),
		   finished(true){}

void EdgeNode::finish(const sf::Vector2f& p2_) {
	edge.p2 = p2_;
	twin = make_shared<EdgeNode>(edge.p2, edge.p1);
	finished = true;
}



//**************DCEL******************

DCEL::DCEL():edges() {}

void DCEL::addEdge(const EdgeNode& newEdge) {
	edges.push_back(newEdge);
}


