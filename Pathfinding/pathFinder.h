#pragma once
#include "graph_adj_list.h"
#include "imageProcessing.h"
#include "Node.h"
#include "Heap.h"
#include "distancematrix.h"
#include <string>
#include "Grid.h"
#include <iostream>
#include <chrono>
#include <queue>
#include <ostream>
#include <fstream>

//vector<point> find_path(point startNode, point targetNode, BinaryImage grid);
void BFS(int source, std::vector<int>& dist, graph_adj_list<int> adj_list);
std::vector<point> find_path(Node startNode, Node targetNode, Grid grid);
std::vector<point> find_path(Node startNode, Node targetNode, Grid grid, point top, point bot);
vector<point> find_path_(Node startNode, Node targetNode, Grid grid, DistanceMatrix dm);
// iPair ==>  Integer Pair 
typedef pair<int, int> iPair;

std::ostream& operator<<(std::ostream& os, const point& pos);


// This class represents a directed graph using 
// adjacency list representation 
class Graph_t
{
    int size;    // No. of vertices 
    int vertex_cont;
    int width;
    // In a weighted graph, we need to store vertex 
    // and weight pair for every edge 
    list< pair<int, int> >* adj;

public:
    Graph_t() {};
    Graph_t(int size, int width) {
        this->size = size;
        this->width = width;
        adj = new list<iPair>[size];
    };  // Constructor 

    // function to add an edge to graph 
    void addEdge(int u, int v, int w) {
        
        for(auto edge : adj[u]) {
            if (edge.first == v)
                return;
        }

        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    };

    void addEdge(point u, point v, int w) {
        addEdge(u.y * width + u.x, v.y * width + v.x, w);
    }

    void removeVertex(int v) {
        for (auto vertex : *adj) {
            int ver = (vertex).first;
            if (ver == v) {
                adj[ver].remove(vertex);
            }
        }
    }

    Graph_t copy_vector(Graph_t other) {
        Graph_t result = other;
        return result;
    }

    void removeVertex(point v) {
        removeVertex(v.y * width + v.x);
    }

    vector<point> shortestPath(point src, point dst) {
        return shortestPath(src.y * width + src.x, dst.y * width + dst.x);
    }

    void save_graph(std::string file_name) {
        std::ofstream out;
        out.open(file_name);

        out << "digraph G { \n";
        int cont = 0;
        for (int i = 0; i < size; i++) {

            if (adj[i].size() > 0) {

                int x = i % width;
                int y = i / width;

                point p(x, y);

                out << cont;

                out << " [label=\"" << p << "\"];\n";
                cont++;
            }
        }

        for (int j = 0; j < size; j++) {
            for (auto it = adj[j].begin(); it != adj[j].end(); it++) {
                int v = (*it).first;

                int x = j % width;
                int y = j / width;
                point p1(x, y);

                x = v % width;
                y = v / width;

                point p2(x, y);

                out << p1 << "->" << p2 << " ;\n";

            }
        }
        out << "}";
    }

    vector<point> shortestPath(int src, int dst)
    {
        // Create a priority queue to store vertices that 
        // are being preprocessed. This is weird syntax in C++. 
        // Refer below link for details of this syntax 
        priority_queue< iPair, vector <iPair>, greater<iPair> > pq;

        // Create a vector for distances and initialize all 
        // distances as infinite (INF) 
        vector<int> dist(size, INT_MAX);
        vector<int> parent(size, -1);
        vector<point> path;
        // Insert source itself in priority queue and initialize 
        // its distance as 0. 
        pq.push(make_pair(0, src));
        dist[src] = 0;

        /* Looping till priority queue becomes empty (or all
          distances are not finalized) */
        while (!pq.empty())
        {
            // The first vertex in pair is the minimum distance 
            // vertex, extract it from priority queue. 
            // vertex label is stored in second of pair (it 
            // has to be done this way to keep the vertices 
            // sorted distance (distance must be first item 
            // in pair) 
            int u = pq.top().second;
            pq.pop();

            // 'i' is used to get all adjacent vertices of a vertex 
            list< pair<int, int> >::iterator i;
            for (i = adj[u].begin(); i != adj[u].end(); ++i)
            {
                // Get vertex label and weight of current adjacent 
                // of u. 
                int v = (*i).first;
                int weight = (*i).second;

                //  If there is shorted path to v through u. 
                if (dist[v] > dist[u] + weight)
                {
                    // Updating distance of v 
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                    parent[v] = u;
                    if (v == dst) {
                        int currentVertex = v;
                        while (true) {
                            int x = currentVertex % width;
                            int y = currentVertex / width;

                            path.push_back(point(x, y));

                            if (parent[currentVertex] == -1) {

                                return path;
                            }
                            else {
                                currentVertex = parent[currentVertex];
                            }
                        }
                    }
                }
            }
        }
        return path;
    }
};


