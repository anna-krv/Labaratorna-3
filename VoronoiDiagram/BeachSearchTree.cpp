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

//**************Arch****************

Arch::Arch()
	:focus(nullptr) {}

Arch::Arch(const pointEvent& focus_)
	: focus(make_shared<sf::Vector2f>(focus_.getPointOfEvent())) {}

float Arch::x() const{
	return focus->x;
}

float Arch::y() const{
	return focus->y;
}


bool Arch::operator==(const Arch& other) const {
	return (abs(focus->x - other.focus->x) < 0.001 &&
		abs(focus->y - other.focus->y) < 0.001);
}

bool Arch::operator!=(const Arch& other) const {
	return !(*this == other);
}


//***************BeachNode***************

BeachNode::BeachNode(): arch(nullptr),  breakpoint(infPoint),
      parent(nullptr), left(nullptr), right(nullptr), edge(),
	   myCircleEvent(){}

BeachNode::BeachNode(const Arch& arch) {}
BeachNode::BeachNode(const sf::Vector2f breakPoint) {}


//**************BeachSearchTree**************

BeachSearchTree::BeachSearchTree() 
	: root(nullptr) {}

BeachSearchTree::BeachSearchTree(const shared_ptr<BeachNode>& beachNode) 
	:root(beachNode) {}



shared_ptr<BeachNode> BeachSearchTree::searchPlaceForArch(double myX) const {
	shared_ptr<BeachNode> curNode = getLeftmostLeaf;
	/*
	if (*(curNode->arch) != myArch) {
		if (myArch.x() < curNode->arch->x()) {
			if (curNode->left) {
				BeachSearchTree subtree(curNode->left);
				return subtree.searchArch(myArch);
			}
		}
	}
	*/
	return curNode;
}

shared_ptr<BeachNode> BeachSearchTree::searchArch(const Arch& myArch) const {
	return nullptr;
}

void BeachSearchTree::addArch(const Arch& myArch) {
	if (!root)
		root = make_shared<BeachNode>(BeachNode(myArch));
	else {
		shared_ptr<BeachNode> nodeToSplit = searchPlaceForArch(myArch);
	}
}

void BeachSearchTree::deleteArch(const Arch& myArch) {
}

shared_ptr<BeachNode> BeachSearchTree::getLeftmostLeaf() const {
	shared_ptr<BeachNode> res = root;
	while (res->left)
		res = res->left;
	return res;
}


//Basially, it adds a new Node with arch to BeachSearchTree
void BeachSearchTree::handlePointEvent(const pointEvent& myPointEvent,
	deque<Event> eventsQueue, DCEL& edges) {
	if (!root) {
		root = make_shared<BeachNode>(BeachNode(Arch(myPointEvent)));
		return;
	}
	//we will find the arch which lies just above the site




}

void BeachSearchTree::handleCircleEvent(const circleEvent& myCircleEvent,
	deque<Event> eventsQueue, DCEL& edges) {
}

