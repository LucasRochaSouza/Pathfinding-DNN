#pragma once

#include "distance_data.h"

using namespace std;

int position(int i, int j, int row) {
    return i * row + j;
}

inline void write_hd(vector<int> dist, int w, int h, int i, int j, ostream& output) {
    int countX = 0;
    int countY = 0;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (dist[position(x, y, w)] != INT_MAX && !(i == x && j == y) && dist[position(x, y, w)] != 0) {
                if (i * w + j > x * w + y)
                    output << i << "," << j << "," << x << "," << y << "," << dist[position(x, y, w)] << endl;
            }
        }
    }
}

void generateDistanceData(BinaryImage map, int start, int finish, string fileName) {
    ofstream data(fileName, ofstream::out);
    vector<int> dist;

    Graph2D graph(map);

    thread hdd_job;
    bool first = true;
    for (int i = start; i < finish; i++) {
        for (int j = 0; j < map.height; j++) {
            if (map.get(i, j) == true) {
                graph.BFS(i, j, dist);
                if (first == false) {
                    hdd_job.join();
                }
                hdd_job = thread(write_hd, dist, map.width, map.height, i, j, ref(data));
                first = false;
            }
        }
    }
    hdd_job.join();
}

void createData(BinaryImage img, int threadNum, string fileName)
{
    generateDistanceDataThread(img, threadNum, fileName);
    vector<string> files;
    for (int i = 0; i < threadNum; i++) {
        files.push_back(fileName + to_string(i) + ".csv");
    }

    mergeFiles(fileName + ".csv", files);

    for (int i = 0; i < threadNum; i++) {
        remove(files[i].c_str());
    }
}

void generateDistanceDataThread(BinaryImage img, int threadNum, string fileName) {
    int num = ceil((float)img.width / (float)threadNum);

    vector<thread> threadList;

    threadList.reserve(threadNum);

    for (int i = 0; i < threadNum; i++) {
        if (i == threadNum - 1) {
            threadList.push_back(thread(generateDistanceData, img, i * num, img.width, fileName + to_string(i) + ".csv"));
        }
        else {
            threadList.push_back(thread(generateDistanceData, img, i * num, i * num + num, fileName + to_string(i) + ".csv"));
        }
    }

    for (int i = 0; i < threadList.size(); i++) {
        threadList[i].join();
    }
}

void mergeFiles(string fileName, vector<string> files) {
    ofstream outFile(fileName, ios_base::binary);

    for (string name : files) {

        ifstream file(name, ios_base::binary);
        outFile << file.rdbuf();
        file.close();
    }

    outFile.close();
}