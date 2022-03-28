#pragma once
#include <iostream>

#include <vector>
#include "AbstractNode.h"
#include "Node.h"
#include "imageProcessing.h"

class Cluster
{
public:
	Cluster(BinaryImage grid, point left, point right, int index);
	Cluster();

	void setRightBotton(point pos);
	void setLeftTop(point pos);

	//DistanceMatrix distances;

	point getRightBotton();
	point getLeftTop();

	void setWidth(int w) { 
		width = w; 
	};
	void setHeight(int h) {
		height = h;
	};

	int getWidth() {
		return width; 
	};
	int getHeight() {
		return height;
	};

	BinaryImage getGrid() { return grid; };

	void addBorder(Node grid_position);
	void addBorder(int, int);

	bool contains(point pos);

	Cluster& operator=(const Cluster&);
	bool operator==(const Cluster& other);

	std::vector<AbstractNode> nodes;

protected:
	int width;
	int height;
	int cluster_index;
	point LeftCorner;
	point RightCorner;

	BinaryImage grid;
};

