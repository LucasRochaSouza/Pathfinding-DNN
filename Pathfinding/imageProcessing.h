#pragma once
#include "CImg.h"
#include <vector>
#include "BinaryImage.h"

int histogram(cimg_library::CImg<unsigned char> image);

BinaryImage splitImage(BinaryImage source, int x1, int x2, int y1, int y2);
std::vector<BinaryImage> verticalSlice(BinaryImage source, int n);
std::vector<BinaryImage> horizontalSlice(BinaryImage source, int n);
std::vector<BinaryImage> split(BinaryImage source, int vertical, int horizontal);
cimg_library::CImg<DWORD> convertToImgRGB_DWORD(BinaryImage binaryImage);
cimg_library::CImg<unsigned char> convertToImg(BinaryImage binaryImage);
cimg_library::CImg<unsigned char> convertToImgRGB(BinaryImage binaryImage);
BinaryImage basicBinarization(cimg_library::CImg<unsigned char> image);
