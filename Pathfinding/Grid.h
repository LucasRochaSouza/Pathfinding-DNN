#pragma once
#include <list>
#include <string>
#include "Node.h"
#include "imageProcessing.h"

class Grid
{
public:
	int Width;
	int Height;

	std::vector<std::vector<Node*>> grid;

    Grid() {
        Width = 0;
        Height = 0;
    }

	Grid(std::string fileName) {
        cimg_library::CImg<unsigned char> texture(fileName.c_str());


        Width = texture.width();
        Height = texture.height();

        grid.resize(Width, std::vector<Node*>(Height));

        binarization(texture);

	}

    Grid(BinaryImage bm) {
        Width = bm.width;
        Height = bm.height;

        grid.resize(Width, std::vector<Node*>(Height));

        binarization(bm);
    }

    void binarization(BinaryImage img) {
        for (int x = 0; x < Width; x++) {
            for (int y = 0; y < Height; y++) {
                Node* temp = new Node(x, y, img.get(x, y));
                grid[x][y] = temp;
            }
        }
    }

	void binarization(cimg_library::CImg<unsigned char> img) {
		int threshold = histogram(img);

        cimg_forXY(img, x, y) {
            Node* temp;
            if (img(x, y, 0, 0) < threshold) {
                temp = new Node(x, y, false);
                grid[x][y] = temp;
            }
            else {
                temp = new Node(x, y, true);
                grid[x][y] = temp;
            }
        }
	}
    /*
    void copyGrid(const Grid& other) {
        this->Width = other.Width;
        this->Height = other.Height;
        this->grid.resize(Width, std::vector<Node_*>(Height));
        for (auto vec : other.grid) {
            transform(vec.begin(), vec.end(), back_inserter(vec), cloneFunctor<Node_>());
        }
    }
    */
    Grid& operator=(const Grid& other) {
        this->Width = other.Width;
        this->Height = other.Height;
        this->grid.resize(Width, std::vector<Node*>(Height));
        for (int i = 0; i < Width; i++) {
            for (int j = 0; j < Height; j++) {
                this->grid[i][j] = new Node(*other.grid[i][j]);
            }
        }
        return *this;
    }

    std::vector<Node*> GetNeighbours(Node node) 
    {
        std::vector<Node*> neighbours;

        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                if (x == 0 && y == 0)
                    continue;

                int checkX = node.position.x + x;
                int checkY = node.position.y + y;

                if (checkX >= 0 && checkX < Width && checkY >= 0 && checkY < Height &&
                    grid[checkX][checkY]->walkable)
                {
                    neighbours.push_back(grid[checkX][checkY]);
                }
            }
        }

        return neighbours;
    }

    std::vector<Node*> GetNeighbours_2(Node node) const
    {
        std::vector<Node*> neighbours;

        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                if (x == 0 && y == 0)
                    continue;

                int checkX = node.position.x + x;
                int checkY = node.position.y + y;

                if (checkX >= 0 && checkX < Width && checkY >= 0 && checkY < Height &&
                    grid[checkX][checkY]->walkable)
                {
                    neighbours.push_back(new Node(*grid[checkX][checkY]));
                }
            }
        }

        return neighbours;
    }
};

