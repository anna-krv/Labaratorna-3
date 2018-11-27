//Made by Anna Kravets, K-22

#include "VoronoiDiagram.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <E:\2\SFML-2.5.0\include\SFML\Main.hpp>
#include <iostream>
#include <cmath>
using namespace std;

Event::Event(double y_, sf::Vector2f point_, Arch* myArch_)
	:y(y_), point(point_), myArch(myArch_), valid(true) {}

double Event::getY() const {
	return y;
}