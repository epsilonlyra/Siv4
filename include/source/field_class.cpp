#include "field_class.hpp"

using namespace fc;

std::string out_of_boundary_change ::  what() { return std::string("Impossible to change an element out of boundaries\n"); }

//---------------------------------------------------------------------
// Grid methods

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

Grid& Grid :: operator*=(const double t) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] *= t;
        }
    return *this;
}

Grid&  Grid :: operator+=(const Grid& other) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] += other.ptr[x][y];
        }
    return *this;
}

void Grid :: increment(const Grid& other, double dt) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] += other.ptr[x][y] * dt;
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

void Scalar_Field :: get_phi_xx() {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            phi_xx.change_val(x, y, phi_grid.val(x-1, y) - 2 * phi_grid.val(x,y) + phi_grid.val(x+1,y));
        }
}

void Scalar_Field :: get_phi_yy() {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            phi_yy.change_val(x, y, phi_grid.val(x, y-1) - 2 * phi_grid.val(x,y) + phi_grid.val(x,y+1));
        }
}

void Scalar_Field :: apply_borders() {
    // sets phi=0 and dot_phi=0 near borders
    for (int x = 0; x < N; x++) {
        phi_grid.change_val(0, x, 0);
        phi_grid.change_val(N-1, x, 0);
        phi_grid.change_val(x, 0, 0);
        phi_grid.change_val(x, N-1, 0);

        dot_phi_grid.change_val(0, x, 0);
        dot_phi_grid.change_val(N-1, x, 0);
        dot_phi_grid.change_val(x, 0, 0);
        dot_phi_grid.change_val(x, N-1, 0);
    }
}

void  Scalar_Field  :: evolve(double dt) {
    // calculating derivatives
    get_phi_xx();
    get_phi_yy();
    
    // boundary conditions
    apply_borders();

    // incremetning the values
    // assuming c = 1 (need to fix in the future)
    phi_grid.increment(dot_phi_grid, dt);
    dot_phi_grid.increment(phi_xx, dt);
    dot_phi_grid.increment(phi_yy, dt);
}

void Scalar_Field :: update_phi(int x, int y, int side, int amplitude) {
     for (int i = 0; i < side; i++) {
             for (int j = 0; j < side; j++) {
                 try {
                    phi_grid.change_val(x - side / 2 + i, y - side /2 +  j, amplitude);
                 }

                 catch (const out_of_boundary_change& error) {}
             }
    }
}



