//Made by Anna Kravets, K-22
#include "VoronoiDiagram.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

double wSize = 590., hSize = 480;
sf::RenderWindow window(sf::VideoMode(wSize, hSize), "Enjoy Geometry!");
double scale[2] = { 4., 1.5 };
int iScale = 0;

vector<vector<sf::ConvexShape>> animation = {};
vector<vector<sf::CircleShape>> animationPoints = {};
vector<sf::ConvexShape> tmpGood = {};
vector<sf::CircleShape> tmpGoodPoints = {};


//TODO: debug Fortune Algo with points
/*
if (i == 0)
myPoints[i].setPosition(30, 20);
else if (i==1)
myPoints[i].setPosition(480, 60);
else if(i==2)
myPoints[i].setPosition(250, 230);
*/


void fill(sf::CircleShape myPoints[], int nElem) {
	srand(time(NULL));
	for (int i = 0; i < nElem; i++) {
		myPoints[i].setRadius(2);
		myPoints[i].setOrigin(2, 2);
		myPoints[i].setPosition(wSize/8+rand() % (int)(wSize*3/4), hSize/8+rand() % (int)(hSize*3/4));
		myPoints[i].setFillColor(sf::Color::Black);
		//Animation
		tmpGoodPoints.push_back(myPoints[i]);
		points.push_back(myPoints[i]);
	}
}


sf::ConvexShape makeConvex(vector<vector<double>> v, sf::Color col) {
	sf::ConvexShape convex;
	convex.setFillColor(col);
	convex.setPointCount(v.size());
	for (int i = 0; i < v.size(); i++) {
		convex.setPoint(i, sf::Vector2f(v.at(i)[0], v.at(i)[1]));
	}
	return convex;
}

bool onOneSide(double a, double b, double c,
	double x1, double y1, double x2, double y2) {
	return (a*x1 + b*y1 + c)*(a*x2 + b*y2 + c) >= -0.00001;
}

bool checkIfIn(const vector<vector<double>> myVec, vector<double> checkV) {
	for (int i = 0; i < myVec.size(); i++) {
		vector<double> tmp = myVec[i];
		if (abs(tmp[0] - checkV[0]) < 0.0001 &&
			abs(tmp[1] - checkV[1]) < 0.0001)
			return true;
	}
	return false;
}

sf::Vector2f findIntersection(double x1, double y1, double x2, double y2, double a, double b, double c) {
	double x, y, delta;
	delta = (y2 - y1)*b - a*(x1 - x2);
	if (abs(delta) < 0.00001)
		throw exception();
	x = ((x1*y2 - y1*x2)*b + c*(x1 - x2)) / delta;
	y = (c*(y1 - y2) - a*(x1*y2 - y1*x2)) / delta;
	return sf::Vector2f(x, y);
}

bool checkIfOnSegment(double x1, double y1,
	double x2, double y2, double x, double y) {
	return (abs(abs(x2 - x1) - abs(x2 - x) - abs(x - x1)) < 0.000001 &&
		abs(abs(y2 - y1) - abs(y2 - y) - abs(y - y1)) < 0.000001);
}

vector<vector<double>> checkLine(int i, sf::Color col, vector<vector<double>> v,
	vector<double> iPoint, double a, double b, double c) {
	vector<vector<double>> newPoints;
	int firstBreak = -1, secondBreak = -1;
	for (unsigned int i = 0; i < v.size(); i++) {
		unsigned int iNew = (i + 1) % v.size();
		try {
			sf::Vector2f intersection = findIntersection(v[i][0],
				v[i][1], v[iNew][0], v[iNew][1], a, b, c);
			double x = intersection.x, y = intersection.y;
			if (checkIfOnSegment(v[i][0],
				v[i][1], v[iNew][0], v[iNew][1], x, y)) {
				//in order not to push the same point twice=)
				if (!checkIfIn(newPoints, { x, y }) &&
					!(abs(x - v[i][0])<0.00001 && abs(y - v[i][1])<0.00001))
					newPoints.push_back(vector<double>{x, y});
				if (firstBreak == -1)
					firstBreak = i;
				else
					secondBreak = i;
			}
		}
		catch (exception) {
			continue;
		}
	}
	if (newPoints.size() != 2)
		return v;
	vector<vector<double>> newV;
	if (firstBreak > secondBreak) {
		int z = firstBreak;
		firstBreak = secondBreak; secondBreak = z;
		vector<double> myTmp;
		myTmp.push_back(newPoints[0][0]);
		myTmp.push_back(newPoints[0][1]);
		newPoints[0][0] = newPoints[1][0];
		newPoints[0][1] = newPoints[1][1];
		newPoints[1][0] = myTmp[0];
		newPoints[1][1] = myTmp[1];
	}
	if (onOneSide(a, b, c, v[firstBreak][0],
		v[firstBreak][1], iPoint[0], iPoint[1])) {
		for (int i = secondBreak + 1; i != v.size() + firstBreak + 1; i++)
			newV.push_back(v[i%v.size()]);
		if (!checkIfIn(newV, newPoints[0]))
			newV.push_back(newPoints[0]);
		if (!checkIfIn(newV, newPoints[1]))
			newV.push_back(newPoints[1]);
	}
	else {
		for (int i = firstBreak + 1; i <= secondBreak; i++)
			newV.push_back(v[i]);
		if (!checkIfIn(newV, newPoints[1]))
			newV.push_back(newPoints[1]);
		if (!checkIfIn(newV, newPoints[0]))
			newV.push_back(newPoints[0]);
	}
	//Animation
	vector<sf::ConvexShape> tmp = tmpGood;
	tmp.push_back(makeConvex(v, col));
	animation.push_back(tmp);
	tmpGoodPoints[i].setScale(scale[iScale], scale[iScale]);
	tmpGoodPoints[i].setFillColor(sf::Color::Red);
	animationPoints.push_back(tmpGoodPoints);
	iScale = (1 + iScale) % 2;
	return newV;
}


vector<vector<double>> myVec(sf::Color col, int i,
	const sf::CircleShape myPoints[], int nElem) {
	vector<vector<double>> v;
	//first init
	v.push_back(vector<double>{0., 0.});
	v.push_back(vector<double>{wSize, 0.});
	v.push_back(vector<double>{wSize, hSize});
	v.push_back(vector<double>{0., hSize});
	//coordinates of i-th point
	double x = myPoints[i].getPosition().x,
		y = myPoints[i].getPosition().y;
	vector<double> iPoint{ x,y };
	for (int ii = 0; ii < nElem; ii++) {
		if (ii == i)
			continue;
		//coordinates of ii-th point
		double x1 = myPoints[ii].getPosition().x,
			y1 = myPoints[ii].getPosition().y;
		v = checkLine(i, col, v, iPoint,
			x - x1, y - y1,
			(x1*x1 + y1*y1 - x*x - y*y) / 2.);
	}
	return v;
}


sf::ConvexShape findConvex(int i, const sf::CircleShape myPoints[], int nElem) {
	sf::Color col = (sf::Color(rand() % 255, rand() % 255, rand() % 255));
	vector<vector<double>> v = myVec(col, i, myPoints, nElem);
	sf::ConvexShape convex = makeConvex(v, col);
	//Animation
	tmpGood.push_back(convex);
	animation.push_back(tmpGood);
	tmpGoodPoints[i].setScale(1., 1.);
	animationPoints.push_back(tmpGoodPoints);
	return convex;
}

vector<sf::ConvexShape> buildDiagram(const sf::CircleShape myPoints[], int nElem) {
	vector<sf::ConvexShape> myDiagram;
	for (int i = 0; i < nElem; i++)
		myDiagram.push_back(findConvex(i, myPoints, nElem));
	return myDiagram;
}