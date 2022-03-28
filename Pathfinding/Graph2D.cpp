#include "Graph2D.h"
using namespace std;

// Abstração de estruturas de dados "feias" do c++

Graph2D::Graph2D(BinaryImage image) {
    width = image.width;
    height = image.height;

    Graph = vector<vector<int>>(width * height, vector<int>(0));

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (image.get(x, y) == true) {
                if (valid_neighbours(image, x - 1, y - 1)) {
                    add_edge(x, y, x - 1, y - 1);
                }
                if (valid_neighbours(image, x - 1, y)) {
                    add_edge(x, y, x - 1, y);
                }
                if (valid_neighbours(image, x, y - 1)) {
                    add_edge(x, y, x, y - 1);
                }
                if (valid_neighbours(image, x, y + 1)) {
                    add_edge(x, y, x, y + 1);
                }
                if (valid_neighbours(image, x + 1, y)) {
                    add_edge(x, y, x + 1, y);
                }
                if (valid_neighbours(image, x - 1, y + 1)) {
                    add_edge(x, y, x - 1, y + 1);
                }
                if (valid_neighbours(image, x + 1, y - 1)) {
                    add_edge(x, y, x + 1, y - 1);
                }
                if (valid_neighbours(image, x + 1, y + 1)) {
                    add_edge(x, y, x + 1, y + 1);
                }
            }
        }
    }
}

Graph2D::Graph2D(int width, int height) {
    this->width = width;
    this->height = height;
    this->Graph = vector<vector<int>>(width * height, vector<int>(0));
}

bool Graph2D::add_edge(int xs, int ys, int xd, int yd) {
    return add_edge(position(xs, ys), position(xd, yd));
}

bool Graph2D::add_edge(int source, int destination) {
    if (source < 0 || destination < 0) {
        return false;
    }

    if (Graph.size() <= source || Graph.size() <= destination) {
        return false;
    }

    if (contains(Graph[source], destination) == true)
    {
        return false;
    }

    if (contains(Graph[destination], source) == true)
    {
        return false;
    }

    Graph[source].push_back(destination);
    Graph[destination].push_back(source);
    return true;
}

int Graph2D::position(int x, int y) {
    if (x < 0 || y < 0) {
        return -1;
    }
    return y * width + x;
}

vector<int> Graph2D::operator[](int i) {
    return Graph[i];
}

void Graph2D::BFS(int xs, int ys, vector<int>& dist) {
    BFS(position(xs, ys), dist);
}

vector<int> Graph2D::BFS(int xs, int ys, int xd, int yd) {
    return BFS(position(xs, ys), position(xd, yd));
}

void Graph2D::BFS(int source, vector<int>& dist) {
    queue<int> queue;
    int size = height * width;

    dist = vector<int>(size);

    vector<bool> visited(size);


    for (int i = 0; i < size; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
    }

    visited[source] = true;
    dist[source] = 0;
    queue.push(source);

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        for (int i = 0; i < Graph[u].size(); i++) {
            if (visited[Graph[u][i]] == false) {
                visited[Graph[u][i]] = true;
                dist[Graph[u][i]] = dist[u] + 1;
                queue.push(Graph[u][i]);
            }
        }
    }
}

/*
bool isOutOfGrid(Position src, Position topLeft, Position bottonRight) {
    return (src.getX() < topLeft.getX() || src.getX() > bottonRight.getX()) ||
        (src.getY() < topLeft.getY() || src.getY() > bottonRight.getY());
}


vector<Position> Graph2D::Astar(Position source, Position destination, BinaryImage map, Position leftTop, Position bottonRight) {

    vector<Node> openList;
    vector<Node> closedList;

    Node startNode(source);
    Node endNode(destination);

    openList.push_back(startNode);

    vector<Position> path;

    while (!openList.empty()) {
        Node temp = openList.front();
        Node &node = *new Node(openList.front());
        for (int i = 1; i < openList.size(); i++) {
            if (openList[i].f_cost() < node.f_cost() || openList[i].f_cost() == node.f_cost()) {
                if (openList[i].h_cost() < node.h_cost()) {
                    node = openList[i];
                }
            }
        }

        openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());
        closedList.push_back(node);

        if (node == endNode) {
            Node current = node;
            while (true) {
                path.push_back(current.position);
                if (current.getParent() == nullptr) {
                    return path;
                }
                else {
                    current = current.getParent();
                }
            }
        }

        auto vetor = node.getNeighbours(map);

        for (Node neighbour : vetor) {

            if (isOutOfGrid(neighbour.position, leftTop, bottonRight)) {
                continue;
            }

            int newCost = node.g_cost() + distance2(node.position, neighbour.position);
            if (newCost < neighbour.g_cost() || count(openList.begin(), openList.end(), neighbour) == 0) {
                int g = newCost;
                int h = distance(neighbour.position, destination);

                neighbour.parent = &node;

                neighbour.set_g(g);
                neighbour.set_h(h);

                if (count(openList.begin(), openList.end(), neighbour) == 0) {
                    openList.push_back(neighbour);
                }
            }
        }
    }
}

*/
bool isNotVisited(int x, vector<int>& path)
{
    int size = path.size();
    for (int i = 0; i < size; i++)
        if (path[i] == x)
            return false;
    return true;
}

vector<int> Graph2D::BFS(int source, int dest) {
    queue<vector<int>> queue;
    vector<int> path;

    path.push_back(source);
    queue.push(path);

    while (!queue.empty()) {
        path = queue.front();
        queue.pop();

        int last = path[path.size() - 1];

        if (last == dest) {
            return path;
        }

        for (int i = 0; i < Graph[last].size(); i++) {
            if (isNotVisited(Graph[last][i], path)) {
                vector<int> newpath(path);
                newpath.push_back(Graph[last][i]);
                queue.push(newpath);
            }
        }
    }
}

bool contains(const vector<int>& list, int value) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == value) {
            return true;
        }
    }
    return false;
}

bool valid_neighbours(BinaryImage& image, int x, int y) {
    int w = image.width;
    int h = image.height;

    if (x == w || y == h) {
        return false;
    }
    if (x < 0 || y < 0) {
        return false;
    }
    if (image.get(x, y) == false) {
        return false;
    }

    return true;
}

/*
def Dijkstra(x1, y1, map, width, height):

    src = Position(x1, y1)

    dist = [[0 for x in range(width)] for y in range(height)]

    visited = [bitarray(width) for i in range(height)]

    for x in range (width):
        for y in range(height):
            visited[x][y] = False
            dist[x][y] = 99999999

    dist[x1][y1] = 0

    for i in range ((width*height) - 1):
            ver_x, ver_y = minDist(dist, visited, width, height)
            visited[ver_x][ver_y] = True

            neighbours = Neighbours(map, ver_x, ver_y, width, height)

            for V in neighbours:
                x = V.x
                y = V.y
                if visited[x][y] == False and dist[x][y] > dist[ver_x][ver_y] + 1 and dist[ver_x][ver_y] != 99999999:
                    dist[x][y] = dist[ver_x][ver_y] + 1
    return dist
*/