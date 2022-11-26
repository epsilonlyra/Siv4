#include "field_class.hpp"
#include<memory>

using namespace fc;

std::string out_of_boundary_change ::  what() { return std::string("Impossible to change an element out of boundaries\n"); }



//---------------------------------------------------------------------
// Grid methods

Grid :: Grid() : ptr(std :: make_unique< std ::  unique_ptr<double []>[] >(N)) {
    // intitializes Grid filled with zeros
    for (int i = 0; i < N; i++) ptr[i] = std :: make_unique<double []>(N);
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) ptr[x][y] = 0;
}

double Grid ::  val(int x, int y) const {
    // returns value at x,y if x,y are in boundaries; returns 0 otherwise
    if ((x < 0) || (x >= N)) return -10;
    if ((y < 0) || (y >= N)) return -10;
    return ptr[x][y];
}

void  Grid :: change_val(int x, int y, double new_value) {
    // changes value at xy, if x,y are in boundaries; throws an error otherwise
    if ((x < 0) || (x >= N)) throw out_of_boundary_change();
    if ((y < 0) || (y >= N)) throw out_of_boundary_change();

    if ((x <= 0) || (x >= N - 1)) return ;
    if ((y <= 0) || (y >= N - 1)) return ;
    if (x == 78 && ((y <= (fc :: N) / 2 - 5 )|| (y >= (fc :: N ) / 2 + 5 ))) return;

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

Grid :: ~Grid() = default;

Grid :: Grid(const Grid& other) : Grid() {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] = other.ptr[x][y];
        }
}



//---------------------------------------------------------------------
// Scalar Field methods

double Scalar_Field ::  phi(int x, int y) const {
    return phi_grid.val(x, y);
}

double Scalar_Field ::  dot_phi(int x, int y) const {
    return dot_phi_grid.val(x, y);
}


Scalar_Field :: Scalar_Field(Grid init_phi, Grid init_dot_phi) : phi_grid(init_phi), dot_phi_grid(init_dot_phi) {}

Scalar_Field :: Scalar_Field() : Scalar_Field :: Scalar_Field( Grid(), Grid()) {}

void  Scalar_Field  :: evolve(double dt) {

    phi_grid += dot_phi_grid*(dt);

    dot_phi_grid += ((phi_grid.partial_y()).partial_y())* dt;
    dot_phi_grid += ((phi_grid.partial_x()).partial_x())* dt;
}

void Scalar_Field :: create_disturbance (int x, int y, int width, int length,  int amplitude) {
     for (int i = 0; i < width; i++) {
             for (int j = 0; j < length; j++) {
                 try {
                    phi_grid.change_val(x - width/ 2 + i, y - length /2 +  j, amplitude);
                 }

                 catch (const out_of_boundary_change& error) {}
             }
    }
}





