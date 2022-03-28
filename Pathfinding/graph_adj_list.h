#pragma once
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include "CImg.h"

using namespace std;

template <class T>

class graph_adj_list {
private:
    int size;

    std::vector<std::vector<T>> adj_list;

public:
    graph_adj_list() {};

    // Construtor
    graph_adj_list(int size) {
        this->size = size;
        this->adj_list = vector<vector<T>>(size, vector<T>(0));
    }

    bool add_edge(int source, T destination) {
        if (source < 0) {
            return false;
        }

        if (adj_list.size() <= source) {
            return false;
        }

        if (contains__(adj_list[source], destination) == true)
        {
            return false;
        }

        adj_list[source].push_back(destination);
        return true;
    }

    void BFS(int source, std::vector<int>& dist) {

        queue<int> queue;

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

    vector<T>& operator[](int index) {
        return adj_list[index];
    }

    const std::vector<T> get_neighbours(int src) {
        return adj_list[src];
    }
    
    int get_size() {
        return size;
    }

protected:
    bool contains__(const vector<T>& list, T value) {
        /*
        for (int i = 0; i < list.size(); i++) {
            if (list[i] == value) {
                return true;
            }
        }
        return false;
    }
    */
        return false;
    }
};