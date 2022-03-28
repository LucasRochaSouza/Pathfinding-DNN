#pragma once
#include "Graph2D.h"
#include "imageProcessing.h"
#include "distance_data.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <unordered_map>

using namespace std;
class container_4d {
public:
	vector<int> distances;

	container_4d() {
		d1 = d2 = d3 = d4 = 0;
		distances.resize(d1 * d2 * d3 * d4);
	}

	container_4d(int size) {
		d1 = d2 = d3 = d4 = size;
		distances.resize(d1 * d2 * d3 * d4);
		size_t size_a = 0;
		for (int i : distances) {
			size_a += sizeof(i);
		}
		cout << size_a;
	}

	int operator() (int x1, int y1, int x2, int y2) {
		return distances[x1 + y1 * d1 + x2 * d1 * d2 + y2 * d1 * d2 * d3];
	}

	void set(int x1, int y1, int x2, int y2, int value) {
		distances.at(x1 + y1 * d1 + x2 * d1 * d2 + y2 * d1 * d2 * d3) = value;
	}

private:
	int d1;
	int d2;
	int d3;
	int d4;
};

class DistanceMatrix
{
public:
	container_4d distances;
	DistanceMatrix() {
		distances = container_4d(0);
	}
	DistanceMatrix(Graph2D graph) {

		distances = container_4d(graph.width);

		std::vector<int> dist;

		for (int x1 = 0; x1 < graph.width; x1++) {
			for (int y1 = 0; y1 < graph.height; y1++) {
				graph.BFS(x1, y1, dist);
				for (int x2 = 0; x2 < graph.width; x2++) {
					for (int y2 = 0; y2 < graph.height; y2++) {
						distances.set(x1, y1, x2, y2, dist[position(x2, y2, graph.width)]);
					}
				}
			}
		}

	};
};