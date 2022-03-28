#pragma once

#include <iostream>
#include <fstream>
#include <Thread>
#include <string>
#include <vector>
#include <stdio.h>
#include "BinaryImage.h"
#include "point.h"
#include "Graph2D.h"

using namespace std;

void createData(BinaryImage img, int threadNum, string fileName);
void generateDistanceDataThread(BinaryImage img, int threadNum, string fileName);
void generateDistanceData(BinaryImage map, int start, int finish, string fileName);
void mergeFiles(string fileName, vector<string> files);
int position(int i, int j, int row);