#include <iostream>
#include <cmath>


const int N = 10000;

struct base_exception {
	virtual std::string what() = 0;
};

struct out_of_boundary_change final : base_exception {
	std::string what() override { return std::string("Impossible to change an element out of boundaries\n"); }
};

class Point {
private:
	double x;
	double y;

public:
	Point(double x, double y): x(x), y(y) {}

	double distance(const Point& other) const {
		return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
	}
};

class Grid {
private:
	double** ptr;

public:
	Grid() : ptr(new double* [N]) {
		// intitializes Grid filled with zeros
		for (int i = 0; i < N; i++) ptr[i] = new double[N];
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++) ptr[x][y] = 0;
	}

	double val(int x, int y) const {
		// returns value at x,y if x,y are in boundaries; returns 0 otherwise
		if ((x < 0) || (x >= N)) return 0;
		if ((y < 0) || (y >= N)) return 0;
		return ptr[x][y];
	}

	void change_val(int x, int y, double new_value) {
		// changes value at xy, if x,y are in boundaries; throws an error otherwise
		if ((x < 0) || (x >= N)) throw out_of_boundary_change();
		if ((y < 0) || (y >= N)) throw out_of_boundary_change();
		ptr[x][y] = new_value;
	}

	Grid partial_x() {
		Grid temp;
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++) {
				temp.change_val(x, y, (val(x + 1, y) - val(x - 1, y)) / 2);
			}
		return temp;
	}

	Grid partial_y() {
		Grid temp;
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++) {
				temp.change_val(x, y, (val(x, y + 1) - val(x, y - 1)) / 2);
			}
		return temp;
	}

	Grid& operator+=(const Grid& other) {
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++) {
				ptr[x][y] += other.ptr[x][y];
			}
		return *this;
	}

	Grid operator+(const Grid& other) {
		Grid temp(*this);
		temp += other;
		return *this;
	}

	Grid& operator*(double alpha) {
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++) {
				ptr[x][y] *= alpha;
			}
		return *this;
	}

	// rule of five ???
	// destructor (1/5)
	~Grid() {
		for (int x = 0; x < N; x++) {
			delete[] ptr[x];
		}
		delete[] ptr;
	}

	// copy constructor (2/5)
	Grid(const Grid& other) : Grid() {
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++) {
				ptr[x][y] = other.ptr[x][y];
			}
	}

	
};


class Scalar_Field {
private:
	Grid phi;
	Grid dot_phi;
	const Grid c;

	double phi(int x, int y) const {
		return phi.val(x, y);
	}

	double dot_phi(int x, int y) const {
		return dot_phi.val(x, y);
	}

public:
	Scalar_Field(Grid init_phi, Grid init_dot_phi) : phi(init_phi), dot_phi(init_dot_phi) {} 

	void evolve(double dt) {
		phi += dot_phi * dt;
		dot_phi += ((phi.partial_x()).partial_x() + (phi.partial_y()).partial_y())* dt;
	}
};