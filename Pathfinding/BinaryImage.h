#pragma once
#include <vector>

enum Passable {
    black,
    white
};

class BinaryImage {
private:

    std::vector<std::vector<bool>> bit_array;
public:
    int width;
    int height;
    BinaryImage();
    BinaryImage(int width, int height);

    bool get(int x, int y);
    void set(int x, int y);

    void reset(int x, int y);

    BinaryImage& operator=(const BinaryImage&);
};

