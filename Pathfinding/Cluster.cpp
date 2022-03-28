#include "Cluster.h"

Cluster::Cluster(BinaryImage grid, point left, point right, int index)
{
	this->grid = grid;
	LeftCorner = left;
	RightCorner = right;

	width = grid.width;
	height = grid.height;

	cluster_index = index;

	//dm = Graph2D(grid);

}

Cluster::Cluster()
{
	LeftCorner = point(0, 0);
	RightCorner = point(0, 0);
	width = height = 0;
	
}

void Cluster::setRightBotton(point pos) {
	RightCorner = pos;
}


void Cluster::setLeftTop(point pos) {
	LeftCorner = pos;
}

point Cluster::getRightBotton()
{
	return RightCorner;
}

point Cluster::getLeftTop()
{
	return LeftCorner;
}

void Cluster::addBorder(Node grid_position)
{
	AbstractNode n(grid_position.position, 0, 1);
	int tempX = n.position.x - LeftCorner.x;
	int tempY = n.position.y - LeftCorner.y;
	nodes.push_back(n);
	nodes.back().calculate_distance(point(tempX, tempY), grid);
}

void Cluster::addBorder(int x, int y)
{
	addBorder(point(x, y));
}

bool Cluster::contains(point pos)
{
	return
		pos.x >= LeftCorner.x &&
		pos.x <= RightCorner.x &&
		pos.y >= LeftCorner.y &&
		pos.y <= RightCorner.y;

}

Cluster& Cluster::operator=(const Cluster& other)
{
	this->width = other.width;
	this->height = other.height;
	this->LeftCorner = other.LeftCorner;
	this->RightCorner = other.RightCorner;
	return *this;
}

bool Cluster::operator==(const Cluster& other)
{
	return
		this->LeftCorner == other.LeftCorner &&
		this->RightCorner == other.RightCorner;
}