#include "imageProcessing.h"

using namespace std;

int histogram(cimg_library::CImg<unsigned char> image) {
    int width = image.width();
    int height = image.height();
    int sum = 0;

    cimg_forXY(image, x, y) {
        int grayValue = image(x, y, 0, 0);
        sum = sum + grayValue;
    }

    return (int) (sum / (width * height));
}

BinaryImage splitImage(BinaryImage source, int x1, int x2, int y1, int y2) {
    BinaryImage result(x2 - x1, y2 - y1);
    for (int x = x1, i = 0; x < x2; x++, i++) {
        for (int y = y1, j = 0; y < y2; y++, j++) {
            if (source.get(x, y)) {
                result.set(i, j);
            }
        }
    }
    return result;
}

vector<BinaryImage> verticalSlice(BinaryImage source, int n) {
    int height = source.height;
    vector<BinaryImage> imageChunks;
    for (int i = 0; i < n; i++) {
        int y1 = i * height / n;
        int y2;
        if (i == n - 1) {
            y2 = height;
        }
        else {
            y2 = i * height / n + height / n;
        }

        BinaryImage chunk = splitImage(source, 0, source.width, y1, y2);
        imageChunks.push_back(chunk);
    }
    return imageChunks;
}

vector<BinaryImage> horizontalSlice(BinaryImage source, int n) {
    int width = source.width;
    vector<BinaryImage> imageChunks;
    for (int i = 0; i < n; i++) {
        int x1 = i * width / n;
        int x2;
        if (i == n - 1) {
            x2 = width;
        }
        else {
            x2 = i * width / n + width / n;
        }

        BinaryImage chunk = splitImage(source, x1, x2, 0, source.height);
        imageChunks.push_back(chunk);
    }
    return imageChunks;
}

vector<BinaryImage> split(BinaryImage source, int vertical, int horizontal) {

    vector<BinaryImage> imageChunks;

    auto vec = verticalSlice(source, vertical);

    int i = 0;

    for (BinaryImage b : vec) {
        auto vec2 = horizontalSlice(b, horizontal);
        for (BinaryImage bm : vec2) {
            imageChunks.push_back(bm);
            //i++;
            //auto img = convertToImg(bm);
            //string temp = "split" + to_string(i) + ".jpg";
            //img.save(temp.c_str());
        }
    }
    return imageChunks;
}

bool valid_neighbour_(BinaryImage& image, int x, int y) {
    int w = image.width;
    int h = image.height;

    if (x == w || y == h) {
        return false;
    }
    if (x < 0 || y < 0) {
        return false;
    }
    if (image.get(x, y) == false) {
        return false;
    }

    return true;
}

cimg_library::CImg<DWORD> convertToImgRGB_DWORD(BinaryImage binaryImage)
{
    int width = binaryImage.width;
    int height = binaryImage.height;
    cimg_library::CImg<DWORD> image(width, height, 1, 3, 0);


    DWORD white = 0xFFFFFFFF;
    DWORD black = 0x00000000;

    cimg_forXY(image, x, y) {
        auto temp = image(x, y, 0, 0);
        if (binaryImage.get(x, y)) {
            image(x, y, 0) = white;
        }
        else {
            image(x, y, 0) = black;
        }
    }
    return image;
}

BinaryImage basicBinarization(cimg_library::CImg<unsigned char> image) {
    int hist = histogram(image);
    BinaryImage bm(image.width(), image.height());
    cimg_forXY(image, x, y) {
        if (image(x, y, 0, 0) < hist) 
            bm.reset(x, y);
        else
            bm.set(x, y);
    }
    return bm;
}

cimg_library::CImg<unsigned char> convertToImgRGB(BinaryImage binaryImage)
{
    int width = binaryImage.width;
    int height = binaryImage.height;
    cimg_library::CImg<unsigned char> image(width, height, 1, 3, 0);


    unsigned char white[3] = { 255, 255, 255 };
    unsigned char black[3] = { 0, 0, 0 };

    cimg_forXY(image, x, y) {
        auto temp = image(x, y, 0, 0);
        if (binaryImage.get(x, y)) {
            image.draw_point(x, y, white);
        }
        else {
            image.draw_point(x, y, black);
        }
    }
    return image;
}

cimg_library::CImg<unsigned char> convertToImg(BinaryImage binaryImage)
{
    int width = binaryImage.width;
    int height = binaryImage.height;
    cimg_library::CImg<unsigned char> image(width, height, 1, 1);


    cimg_forXY(image, x, y) {
        auto temp = image(x, y, 0, 0);
        if (binaryImage.get(x, y)) {
            image.set_linear_atXY(255, x, y);
        }
        else {
            image.set_linear_atXY(0, x, y);
        }
    }
    return image;
}