//Made by Anna Kravets, K-22

#pragma once
#include <vector>
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include "E:\2\Lab3\VoronoiDiagram\VoronoiDiagram.h"
using namespace std;

//for Bucket Sort
const int numberOfBlocks = 20;

//for animation
extern vector<vector<vector<sf::Vertex>>> KirkpatrickEdges;


class ConvexHull {
public:
	ConvexHull(sf::CircleShape myPoints_[], int nElem);
	void bucketSort();
	void KirkpatrickAlgo();
	void findLeftAndRight();
private:
	vector<Segment> edges;
	vector<vector<int>> myPoints;
	vector<vector<int>> left, right;
};