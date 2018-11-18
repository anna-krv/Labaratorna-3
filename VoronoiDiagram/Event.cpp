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


Event::Event(const sf::Vector2f& v)
	:pointOfEvent(v) {}

bool Event::operator<(const Event& other) const {
	return pointOfEvent.y <= other.pointOfEvent.y;
}

void Event::handle() {
	animationPointsFortune.push_back(tmpGoodPointsFortune);
	sf::Vertex v1(sf::Vector2f(0, pointOfEvent.y+1), sf::Color::Red);
	tmpGoodFortune = { v1, sf::Vertex(sf::Vector2f(490, pointOfEvent.y+1), sf::Color::Red) };
	animationFortune.push_back(tmpGoodFortune);
}

sf::Vector2f Event::getPointOfEvent() const {
	return pointOfEvent;
}

pointEvent::pointEvent(const sf::Vector2f& site)
	:Event(site) {}

circleEvent::circleEvent(const sf::Vector2f& newVoronoiVertex)
	: Event(newVoronoiVertex) {}