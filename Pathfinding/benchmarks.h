#pragma once
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include "HPA.h"
#include "imageProcessing.h"
#include "pathFinder.h"
#include "CImg.h"
#include "distancematrix.h"
#include <cstdint>
#include <iostream>

void create_test_data(cimg_library::CImg<unsigned char> texture, int x, int y);
void compute_distance_error(cimg_library::CImg<unsigned char> texture);
void distance_error_dnn(BinaryImage grid);