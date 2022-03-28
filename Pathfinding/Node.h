#pragma once
#include "AbstractNode.h"
#include "point.h"

class Node
{
public:
	point position;

	bool visited;
	bool walkable;

	int HeapIndex;

	~Node() {
		parent = nullptr;
	}

	Node();
	Node(int x, int y);
	Node(point p);
	Node(int x, int y, bool walkable);

	int f_cost() { return g + h; };
	int g_cost() { return g; };
	int h_cost() { return h; };

	void set_g(float g) { this->g = g; };
	void set_h(float h) { this->h = h; };

	void set_parent(Node* parent) { this->parent = parent; };

	Node* getParent() { return parent; };

	Node& operator=(const Node);
	Node& operator=(const Node*);

	int compare(Node other) {

		int compare = other.f_cost() - f_cost();
		return compare;
	}
	Node* parent;
protected:
	int g, h;
};

