//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <vector>
#include "CImg.h"
#include "Node.h"
#include "BinaryImage.h"

const unsigned int A_star_space = 0xFFFFFFFF;
const unsigned int A_star_wall = 0x00000000;
//---------------------------------------------------------------------------
class A_star_fast {
public:
    // variables
    unsigned int** map; // map[ys][xs]
    int xs, ys; // map esolution   xs*ys<0xFFFFFFFE !!!
    int* px, * py, ps; // output points px[ps],py[ps] after compute()

    // internals
    A_star_fast();
    ~A_star_fast();
    void _freemap(); // release map memory
    void _freepnt(); // release px,py memory

    // inteface
    void resize(int _xs, int _ys); // realloc map to new resolution
    void set(BinaryImage binaryImage); // copy bitmap to map
    cimg_library::CImg<unsigned char> get();
    std::vector<point> compute(int x0, int y0, int x1, int y1);
    std::vector<point> compute(int x0, int y0, int x1, int y1, int &nodes); // compute path from x0,y0 to x1,y1 output to px,py
    std::vector<point> compute(int x0, int y0, int x1, int y1, float a);
};
//---------------------------------------------------------------------------
