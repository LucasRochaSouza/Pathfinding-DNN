#include "benchmarks.h"

using namespace cimg_library;
using namespace std;

float percentage_error(int aprox, int real) {

	float aprox_f = float(aprox);
	float real_f = float(real);


	return abs((aprox_f - real_f) / real_f);
}

void print_path(cimg_library::CImg<DWORD>& texture, std::vector<point> path)
{
	DWORD white[3] = { 255, 255, 255 };
	DWORD black[3] = { 0, 0, 0 };
	DWORD RED[3] = { 255, 0, 0 };

	for (point p : path) {
		texture(p.x, p.y, 0) = 0x00FF00FF00;
	}
}

vector<point> unblocked_nodes(BinaryImage grid) {
	vector<point> result;
	for (int x = 0; x < grid.width; x++)
		for (int y = 0; y < grid.height; y++) {
			if (grid.get(x, y)) {
				result.push_back(point(x, y));
			}
		}
	return result;
}

vector<point> sampling(float reduction, vector<point> all_points) {
	int size = reduction * all_points.size();

	std::random_device rd;

	std::mt19937 prng(rd());

	std::shuffle(all_points.begin(), all_points.end(), prng);

	auto first = all_points.begin();
	auto last = all_points.begin() + size;

	auto result = vector<point>(first, last);

	return result;
}


void create_test_data(cimg_library::CImg<unsigned char> texture, int x, int y) {
	BinaryImage bm = basicBinarization(texture);

	HPA hpa;

	auto all_points = unblocked_nodes(bm);

	auto samples_src = sampling(0.05, all_points);
	auto samples_dst = sampling(0.05, all_points);

	ofstream node_file, time_file;

	node_file.open("data_nodes_4x4.csv");
	time_file.open("data_time_4x4.csv");

	node_file << "x \t y \t D1\n";
	time_file << "x \t y \t D1\n";

	hpa.abstractMaze(bm, x, y);

	for (int i = 0; i < 316; i++) {
		for (int j = 0; j < 316; j++) {
			auto path = hpa.HPA_Star(samples_src[i], samples_dst[j]);
			if (hpa.test_info.expandedNodes == -1) {
				continue;
			}
			node_file << hpa.test_info.expandedNodes << "\t" << hpa.test_info.distance << "\t1\n";
			time_file << hpa.test_info.executionTime << "\t" << hpa.test_info.distance << "\t1\n";

		}
	}
	node_file.close();
	time_file.close();
}

void compute_distance_error(cimg_library::CImg<unsigned char> texture) {
	BinaryImage bm = basicBinarization(texture);

	Grid g(bm);

	HPA hpa;

	A_star_fast a_star;
	
	a_star.set(bm);

	int paths = 0;
	auto all_points = unblocked_nodes(bm);

	auto samples_src = sampling(1, all_points);
	auto samples_dst = sampling(1, all_points);

	ofstream distance_file;

	distance_file.open("distance_error_2x2.csv", ios::app);

	hpa.abstractMaze(bm, 2, 2);
	int n = 0;
	float acc_error = 0;
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			auto path_hpa = hpa.HPA_Star(samples_src[i], samples_dst[j]);
			auto path_astar = find_path(samples_src[i], samples_dst[j], g);

			if (hpa.test_info.expandedNodes == -1) {
				continue;
			}

			int length_hpa = path_hpa.size();
			int length_astar = path_astar.size();

			float error = percentage_error(length_hpa, length_astar);
			acc_error += error;

			distance_file << length_hpa << "\t" << length_astar << "\t" << error << "\n";
			cout << error*100 << "\n";
			cout << (acc_error / n)*100 << "\n";
			n++;
		}
	}

	distance_file << "Caminhos nao encontrados: " << paths << endl;
	distance_file << "Erro medio acumulado: " << acc_error / n << endl;

	distance_file.close();
}

void distance_error_dnn(BinaryImage grid) {

	Grid g(grid);

	DistanceMatrix dm(grid);

	int paths = 0;
	auto all_points = unblocked_nodes(grid);

	auto samples_src = sampling(0.15, all_points);
	auto samples_dst = sampling(0.15, all_points);

	ofstream distance_file;

	distance_file.open("distance_error_75x75.csv");

	int n = 0;
	int different_lenghts = 0;
	float acc_error = 0;
	for (int i = 0; i < samples_src.size(); i++) {
		for (int j = 0; j < samples_dst.size(); j++) {
			auto manhattan = find_path(samples_src[i], samples_dst[j], g);
			auto dnn = find_path_(samples_src[i], samples_dst[j], g, dm);

			int length_manhattan = manhattan.size();
			int length_dnn = dnn.size();

			float error = percentage_error(length_dnn, length_manhattan);
			acc_error += error;

			distance_file << length_manhattan << "\t" << length_dnn << "\t" << error << "\n";

			cout << error * 100 << "\n";
			cout << (acc_error / n) * 100 << "%\n";
			cout << different_lenghts << endl;

			if (length_manhattan - length_dnn != 0) {
				different_lenghts++;
			}

			n++;
		}
	}

	distance_file << "Erro medio acumulado: " << acc_error / n << endl;

	distance_file.close();

}