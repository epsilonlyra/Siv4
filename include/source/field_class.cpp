#include "field_class.hpp"


std::string out_of_boundary_change ::  what() { return std::string("Impossible to change an element out of boundaries\n"); }



Point  :: Point( double x, double y): x(x), y(y) {}

double Point :: distance(const Point& other) const {
    return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}



Grid :: Grid() : ptr(new double* [N]) {
    // intitializes Grid filled with zeros
    for (int i = 0; i < N; i++) ptr[i] = new double[N];
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) ptr[x][y] = 0;
}

double Grid ::  val(int x, int y) const {
    // returns value at x,y if x,y are in boundaries; returns 0 otherwise
    if ((x < 0) || (x >= N)) return 0;
    if ((y < 0) || (y >= N)) return 0;
    return ptr[x][y];
}

void  Grid :: change_val(int x, int y, double new_value) {
    // changes value at xy, if x,y are in boundaries; throws an error otherwise
    if ((x < 0) || (x >= N)) throw out_of_boundary_change();
    if ((y < 0) || (y >= N)) throw out_of_boundary_change();
    ptr[x][y] = new_value;
}

Grid  Grid :: partial_x() {
    Grid temp;
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            temp.change_val(x, y, (val(x + 1, y) - val(x - 1, y)) / 2);
        }
    return temp;
}

Grid Grid ::  partial_y() {
    Grid temp;
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            temp.change_val(x, y, (val(x, y + 1) - val(x, y - 1)) / 2);
        }
    return temp;
}

Grid&  Grid :: operator+=(const Grid& other) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] += other.ptr[x][y];
        }
    return *this;
}

Grid Grid :: operator+(const Grid& other) {
    Grid temp(*this);
    temp += other;
    return *this;
}

Grid& Grid ::  operator*(double alpha) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] *= alpha;
        }
    return *this;
}

// rule of five ???
// destructor (1/5)
Grid :: ~Grid() {
    for (int x = 0; x < N; x++) {
        delete[] ptr[x];
    }
    delete[] ptr;
}

// copy constructor (2/5)
Grid :: Grid(const Grid& other) : Grid() {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] = other.ptr[x][y];
        }
}





double Scalar_Field ::  phi(int x, int y) const {
    return phi_grid.val(x, y);
}

double Scalar_Field ::  dot_phi(int x, int y) const {
    return dot_phi_grid.val(x, y);
}


Scalar_Field :: Scalar_Field(Grid init_phi, Grid init_dot_phi) : phi_grid(init_phi), dot_phi_grid(init_dot_phi) {}

void  Scalar_Field  :: evolve(double dt) {
    phi_grid += dot_phi_grid*(dt);
    dot_phi_grid += ((phi_grid.partial_x()).partial_x() + (phi_grid.partial_y()).partial_y())* dt;
}



