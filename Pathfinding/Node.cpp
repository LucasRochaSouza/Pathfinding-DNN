#include "Node.h"

Node::Node()
{
    position.x = 0;
    position.y = 0;
    g = 0;
    h = 0;
    HeapIndex = 0;
    parent = nullptr;
    visited = false;
}

Node::Node(int x, int y)
{
    this->position.x = x;
    this->position.y = y;
    g = 0;
    h = 0;
    HeapIndex = 0;
    parent = nullptr;
    visited = false;
    walkable = false;
}

Node::Node(point p) : Node(p.x, p.y)
{
}

Node::Node(int x, int y, bool walkable) : Node(x, y)
{
    this->walkable = walkable;
}

Node& Node::operator=(const Node other)
{
    this->position.x = other.position.x;
    this->position.y = other.position.y;
    this->g = other.g;
    this->h = other.h;
    this->parent = other.parent;
    this->HeapIndex = other.HeapIndex;
    this->visited = other.visited;
    return *this;
}

Node& Node::operator=(const Node* other)
{
    this->position.x = other->position.x;
    this->position.y = other->position.y;
    this->g = other->g;
    this->h = other->h;
    this->parent = other->parent;
    this->HeapIndex = other->HeapIndex;
    this->visited = other->visited;
    return *this;
}
/*
bool Node::operator==(const Node other)
{
    if (this->x == other.x &&
        this->y == other.y) {
        return true;
    }
    else {
        return false;
    }
}
*/