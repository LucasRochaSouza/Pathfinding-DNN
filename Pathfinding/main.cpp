#define cimg_use_png 1
#include "distance_data.h"
#include "imageProcessing.h"

#define MAZE_FILE "maze.png"
#define DATASET_NAME "dataset"

int main() {
	cimg_library::CImg<unsigned char> texture(MAZE_FILE);
	auto bm = basicBinarization(texture);

	createData(bm, 6, DATASET_NAME);

	return 0;
}

