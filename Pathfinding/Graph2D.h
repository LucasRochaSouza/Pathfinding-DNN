#pragma once
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include "BinaryImage.h"
#include "point.h"

bool valid_neighbours(BinaryImage& image, int x, int y);
bool contains(const std::vector<int>& list, int value);

class Graph2D {
private:
    std::vector<std::vector<int>> Graph;

    // Converte uma posição bidimensional em unidimensional x * coluna + y
    int position(int x, int y);
public:
    int width;
    int height;

    Graph2D() {  };

    // Construtor
    Graph2D(int width, int height);
    // Cria uma grafo a partir de uma imagem binária
    Graph2D(BinaryImage image);
    // Adicionar Aresta
    bool add_edge(int source, int destination);
    bool add_edge(int xs, int ys, int xd, int yd);
    // Método de acesso
    std::vector<int> operator[](int);
    // BFS para distâncias
    void BFS(int source, std::vector<int>& dist);
    void BFS(int xs, int ys, std::vector<int>& dist);
    // BFS para caminhos
    std::vector<int> BFS(int source, int dest);
    std::vector<int> BFS(int, int, int, int);

    void printGraph();

};