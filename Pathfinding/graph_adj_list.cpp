/*
#include "graph_adj_list.h"
using namespace std;

// Abstração de estruturas de dados "feias" do c++

bool contains(const vector<int>& list, int value);

graph_adj_list::graph_adj_list(cimg_library::CImg<unsigned char> image) {
    width = image.width();
    height = image.height();

    int histogram = 155;

    adj_list = vector<vector<int>>(width * height, vector<int>(0));

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int pixel = image(x, y, 0, 0);
            if (pixel > histogram) {
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {

                        int neighbour_x = x + i;
                        int neighbour_y = y + j;

                        if (valid_neighbour(neighbour_x, neighbour_y)) {
                            if (image(neighbour_x, neighbour_y, 0, 0) > histogram) {
                                add_edge(x, y, neighbour_x, neighbour_y);
                            }
                            
                        }
                    }
                }
            }
        }
    }
}

graph_adj_list::graph_adj_list(int width, int height) {
    this->width = width;
    this->height = height;
    this->adj_list = vector<vector<int>>(width * height, vector<int>(0));
}

bool graph_adj_list::add_edge(int xs, int ys, int xd, int yd) {
    return add_edge(position(xs, ys), position(xd, yd));
}

bool graph_adj_list::add_edge(int source, int destination) {
    if (source < 0 || destination < 0) {
        return false;
    }

    if (adj_list.size() <= source || adj_list.size() <= destination) {
        return false;
    }

    if (contains(adj_list[source], destination) == true)
    {
        return false;
    }

    if (contains(adj_list[destination], source) == true)
    {
        return false;
    }

    adj_list[source].push_back(destination);
    adj_list[destination].push_back(source);
    return true;
}

int graph_adj_list::position(int x, int y) {
    if (x < 0 || y < 0) {
        return -1;
    }
    return y * width + x;
}

void graph_adj_list::BFS(int xs, int ys, vector<int>& dist) {
    return BFS(position(xs, ys), dist);
}

std::vector<int> graph_adj_list::getNeighbours(int src)
{
    return adj_list[src];
}

std::vector<int> graph_adj_list::getNeighbours(int x, int y)
{
    return adj_list[position(x, y)];
}

void graph_adj_list::BFS(int source, vector<int>& dist) {
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
        for (int i = 0; i < adj_list[u].size(); i++) {
            if (visited[adj_list[u][i]] == false) {
                visited[adj_list[u][i]] = true;
                dist[adj_list[u][i]] = dist[u] + 1;
                queue.push(adj_list[u][i]);
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

bool graph_adj_list::valid_neighbour(int x, int y) {

    if (x == width || y == height) {
        return false;
    }
    if (x < 0 || y < 0) {
        return false;
    }

    return true;
}*/