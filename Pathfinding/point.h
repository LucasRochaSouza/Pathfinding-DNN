#pragma once
struct point {
	int x;
	int y;

	point() : x(0), y(0) {};
	point(const int& x, const int& y) : x(x), y(y) {};
	point(const point& other) {
		x = other.x;
		y = other.y;
	};

	point& operator=(const point& other) {
		x = other.x;
		y = other.y;
		return *this;
	};

	bool operator==(const point& other) const {
		return (x == other.x && y == other.y);
	};

	bool operator<(const point& other) {
		if (x < other.x)
			return true;
		else if (x == other.x && y == other.y)
			return true;

		return false;
	};
};