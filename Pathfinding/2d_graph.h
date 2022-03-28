#pragma once
#pragma once
#include <queue>
#include <list>
#include <vector>
#include <algorithm>

#include "graph_adj_list.h"

using namespace std;
template <class T>

class graph_2d : public graph_adj_list<T> {
public:
    int width;
    int height;
    // Construtor

    graph_2d() {}

    graph_2d(int w, int h) : graph_adj_list<T>(w* h) {
        this->width = w;
        this->height = h;
    };

    bool add_edge(int x, int y, T item) {
        return __super::add_edge(converPosition(x, y), item);
    }

    std::vector<T> get_neighbours(int x, int y) {
        return __super::get_neighbours(converPosition(x, y));
    }

    void BFS(int x, int y, std::vector<int>& dist) {
        return __super::BFS(converPosition(x, y), dist);
    }

    int get_size() {
        return width * height;
    }

protected:
    int converPosition(int x, int y) {
        return y * width + x;
    }

};