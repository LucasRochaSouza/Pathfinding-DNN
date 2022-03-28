#pragma once
#include "Graph2D.h"
#include "point.h"
#include "BinaryImage.h"
#include <unordered_map>
#include <vector>

class AbstractNode
{
protected: 
    int weight;
    int cluster_index;

    int pos_convert(int x, int y, int row) {
        return y * row + x;
    }

public:
    point position;
    std::unordered_map<int, int> distance_data;

    AbstractNode(point p, int cluster, int weight) {
        position = p;
        cluster_index = cluster;
        this->weight = weight;
    }

    AbstractNode(int x, int y, int cluster, int weight) {
        position.x = x;
        position.y = y;
        cluster_index = cluster;
        this->weight = weight;
    }

    void calculate_distance(point relative_position, BinaryImage grid) {
        
        Graph2D graph(grid);
        std::vector<int> dist;

        int w = grid.width;
        int h = grid.height;

        graph.BFS(relative_position.x, relative_position.y, dist);

        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                if (dist[pos_convert(x, y, w)] != INT_MAX && dist[pos_convert(x, y, w)] != 0) {
                    //distance_data.insert(position(x, y, w), dist[position(x, y, w)]);
                    distance_data[pos_convert(x, y, w)] = dist[pos_convert(x, y, w)];
                }
            }
        }
        
    }
};

