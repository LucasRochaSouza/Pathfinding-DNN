#pragma once

#include <math.h>
#include <limits.h>
#include <ostream>
#include <fstream>
#include <chrono>
#include <future>

#include "2d_graph.h"
#include "Cluster.h"
#include "pathFinder.h"
#include "astar_fast.h"
#include "Graph2D.h"

struct test_data {
	int expandedNodes;
	int distance;
	int clusters_traveled;
	float executionTime;
};

class HPA
{
private:

	A_star_fast astar_map;

	BinaryImage global_grid;

	vector<Cluster> clusters;

	void createInterClusterEdge(int x1, int y1, int x2, int y2);
	void createInterClusterEdge(point p1, point p2);

	bool connectNodes(Cluster c, BinaryImage map);
	void connectToBorder_DHPA(point pos, Cluster c, Graph_t graph);
	void connectToBorder(point pos, Cluster c, Graph_t graph);
	bool insert_start_finish(point src, point dst, Graph_t graph);
	point select_best_node(point src, Cluster c);

	void add_edge_graph(Node n1, Node n2, int weight);

	void compareCluster(Cluster& c1, Cluster& c2);

public:
	HPA() {};

	Graph_t global_graph;

	BinaryImage binary_texture;
	Grid astar_grid;
	void buildClusters(int horizontal, int vertical, BinaryImage source);
	test_data test_info;

	Cluster findCluster(point pos);
	void calculate_path_job(point src, point dst, vector<point>& path);
	std::vector<point> HPA_Star_MT(point src, point dst);
	std::vector<point> HPA_Star(point src, point dst);
	void abstractMaze(BinaryImage source, int vertical, int horizontal);
	Cluster findCluster(point pos1, point pos2);

	vector<point> abstract_search(int src, int dst);
};

