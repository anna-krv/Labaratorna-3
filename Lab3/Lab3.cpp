//Made by Anna Kravets, K-22


#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
//#include <E:\2\SFML-2.5.0\include\SFML\Main.hpp>
#include "E:\2\Lab3\VoronoiDiagram\VoronoiDiagram.h"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

//draws each elem of myVec
template <typename T>
void myDraw(vector<T> myVec) {
	for (auto it = myVec.begin(); it != myVec.end(); ++it) {
		window.draw(*it);
	}
}

template<>
void myDraw(vector<sf::Vertex> myVec) {
	window.draw(&myVec[0], myVec.size(), sf::Lines);
}

template<>
void myDraw(vector<Parabola> myVec) {
	for (auto elem: myVec) {
		elem.draw();
	}
}

//demonstration of algorithm's work for building Voronoi diagram
template <typename T>
void demoAlgorithm1(vector<T> animation, 
	               vector<vector<sf::CircleShape>> animationPoints) {
	window.create(sf::VideoMode(wSize, hSize), "Enjoy Geometry!");
	int i = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
				window.close();
		}
		window.clear(sf::Color(255, 255, 255));
		myDraw(animation[i]);
		myDraw(animationPoints[i]);
		if (i < animation.size() - 1)
			i++;
		sf::sleep(sf::milliseconds(50));
		window.display();
	}
}

//demonstration of algorithm's work for building Voronoi diagram
template <typename T>
void demoAlgorithm2(vector<T> animation, vector<vector<Parabola>> parabolas,
	vector<vector<sf::CircleShape>> animationPoints) {
	window.create(sf::VideoMode(wSize, hSize), "Enjoy Geometry!");
	int i = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
				window.close();
		}
		window.clear(sf::Color(255, 255, 255));
		myDraw(animation[i]);
		myDraw(animationPoints[i]);
		myDraw(parabolas[i]);
		if (i < parabolas.size() - 1)
			i++;
		sf::sleep(sf::milliseconds(1000));
		window.display();
	}
}




int main()
{
	sf::CircleShape myPoints[maxNum];
	fill(myPoints, maxNum);
	vector<sf::ConvexShape> diagramVoronogo;
	//diagramVoronogo= buildDiagram(myPoints, maxNum);
	//demoAlgorithm1(animation, animationPoints);
	diagramVoronogo = FortuneBuildDiagram(myPoints, maxNum);
	demoAlgorithm2(animationFortune, parabolas, animationPointsFortune);
	return 0;
}