#pragma once
#include <chrono>
#include <iostream>
#include <thread>

#include "fast_queue.h"
#include "BinaryImage.h"
#include "point.h"

typedef int (*heuristic_function)(int, int, int, int);

#define OPEN  1
#define CLOSED 2

class fast_astar
{
	struct _node {
		int x, y;							// posicao x, y do mapa
		int index = -1;						// posicao do vetor, essa posicao eh calculada com a transformacao linear
											// R2 -> R1 f(x, y) = y*w + x, onde w eh uma constante igual a largura do mapa
		int N = 0, S = 0, E = 0, W = 0;		// posicao do vetor de seus vizinhos para 4 direcoes
		int NE = 0, NW = 0, SE = 0, SW = 0;	// e para as outras 4				
		
		int h;
		int g = 0;
		_node* operator=(const _node* other) {
			*this = *other;
			return this;
		}

	};

public:
	// O construtor inicializa o grid para comecar a fazer calculos
	// Tem muito pre processamento envolvido e nao eh critico que o construtor seja rapido
	// Executado uma vez por mapa
	fast_astar(BinaryImage texture, int x_dst, int y_dst) {

		grid = texture;

		xs = grid.width;
		ys = grid.height;

		arr_size = xs * ys;

		pnt = new _node[arr_size];
		node_status = new int[arr_size];

		// Para todos os pixels da imagem binaria
		for (int x = 0, i = 0; x < xs; x++)
			for (int y = 0; y < ys; y++, i++) {
				_node n;
				 // Se o caminho nao for bloqueado processa os nodos
				if (grid.get(x, y)) {
					n.x = x;
					n.y = y;
					n.index = i;
					int xx;
					int yy;

					xx = 0;
					yy = -1;
					if (valid_neighbour(x + xx, y + yy)) 
						n.N = R2_R1(x + xx, y + yy, xs);
					xx = 0;
					yy = 1;
					if (valid_neighbour(x + xx, y + yy)) 
						n.S = R2_R1(x + xx, y + yy, xs);
					xx = -1;
					yy = 0;
					if (valid_neighbour(x + xx, y + yy)) 
						n.W = R2_R1(x + xx, y + yy, xs);
					xx = 1;
					yy = 0;
					if (valid_neighbour(x + xx, y + yy)) 
						n.E = R2_R1(x + xx, y + yy, xs);
					xx = -1;
					yy = -1;
					if (valid_neighbour(x + xx, y + yy))
						n.NW = R2_R1(x + xx, y + yy, xs);
					xx = 1;
					yy = -1;
					if (valid_neighbour(x + xx, y + yy))
						n.NE = R2_R1(x + xx, y + yy, xs);
					xx = 1;
					yy = 1;
					if (valid_neighbour(x + xx, y + yy))
						n.SW = R2_R1(x + xx, y + yy, xs);
					xx = -1;
					yy = 1;
					if (valid_neighbour(x + xx, y + yy))
						n.SE = R2_R1(x + xx, y + yy, xs);

				}
				// Aloca os nodos
				pnt[i] = n;
			}
	}

	// Deconstructor deve limpar todos os ponteiros
	~fast_astar() {

	}

	int R2_R1(int x, int y, int w) {
		return x * w + y;
	}

	void calculate(int src, int dst, heuristic_function h_func) {
		// Inicializacoes
		fast_queue<int> queue;
		_node* current_element;

		int dst_x = dst % xs;
		int dst_y = dst / xs;

		//int* g_vector;
		bool* visited;

		//g_vector = new int[arr_size];
		visited = new bool[arr_size];

		for (int i = 0; i < arr_size; i++) {
			//g_vector[i] = INT_MAX;
			visited[i] = false;
		}

		//g_vector[src] = 0;

		int current_index;
		int index;

		//int end = 0;
		// Comeca pelo ponto de destino para nao precisar inverter o caminho

		queue.allocate(arr_size);

		current_element = &pnt[src];
		current_element->g = 0;

		queue.add(src, 0);
		
		//pnt[src].h = 0;

		visited[src] = true;
		node_status[src] = OPEN;
		// Enquanto houver pelo menos um elemento na fila e ainda nao estiver encontrado o caminho

		int i, h, e = -1;

		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		int nodes = 0;


		while (queue.num != 0) {
			nodes++;

			current_index = queue.top();

			//std::thread queue_thread(&fast_queue<int>::top, queue);

			if (node_status[current_index] == CLOSED) {
				continue;
			}

			if (current_index == dst) {
				end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;

				std::cout << elapsed_seconds.count() * 1000 << endl;
				cout << nodes;
				return;
			}

			current_element = &pnt[current_index];
			
			int g = current_element->g;
			
			i = current_element->N;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->S;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->W;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->E;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->NE;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->NW;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->SE;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}

			i = current_element->SW;
			if (i != 0) {
				int stats = node_status[i];
				_node neighbour = pnt[i];
				int new_g = g + 1;
				if (neighbour.g < new_g) {
					neighbour.g = g + 1;
					visited[neighbour.index] = true;
					neighbour.h = h_func(neighbour.x, neighbour.y, current_element->x, current_element->y);
					queue.add(neighbour.index, neighbour.g + neighbour.h);
				}
			}
			node_status[current_index] = CLOSED;
			//queue_thread.join();
		}
		return;
	}

private:
	// Memorias read-only (nao devem ser modificadas durante a busca para funcionar em multithread)
	// dimensoes
	int xs;
	int ys;
	size_t arr_size;

	_node* pnt;		// ponteiro com todos os vertices 
	int* node_status;

	BinaryImage grid;

	//*************************************************************************************//

	inline bool valid_neighbour(int x, int y) {

		if (x >= xs * ys || y >= xs * ys) {
			return false;
		}
		if (x < 0 || y < 0) {
			return false;
		}
		if (grid.get(x, y) == false) {
			return false;
		}

		return true;
	}

};