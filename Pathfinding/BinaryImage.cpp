#include "BinaryImage.h"

BinaryImage::BinaryImage() {
    width = 0;
    height = 0;
}

BinaryImage::BinaryImage(int width, int height) {
    this->width = width;
    this->height = height;

    bit_array.resize(width, std::vector<bool>(height, 0));
 
}

bool BinaryImage::get(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return bit_array[x][y];
    }
    else {
        return false;
    }
}

void BinaryImage::set(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        bit_array[x][y] = true;
    }
    
}

void BinaryImage::reset(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        bit_array[x][y] = false;
    }
}

BinaryImage& BinaryImage::operator=(const BinaryImage &temp)
{
    width = temp.width;
    height = temp.height;
    bit_array = temp.bit_array;
    return *this;
}
