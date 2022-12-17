#ifndef T_FIELD_CLASS
#define T_FIELD_CLASS
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>

#include <vector>
#include <cmath>

using namespace fc;


//---------------------------------------------------------------------
// Grid methods

template<int N>
Grid<N> :: Grid() : ptr(std :: make_unique< std ::  unique_ptr<double []>[] >(N)) {
    // intitializes Grid filled with zeros
    for (int i = 0; i < N; i++) ptr[i] = std :: make_unique<double []>(N);
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) ptr[x][y] = 0;
}

template<int N>
double Grid<N> ::  val(int x, int y) const {
    // returns value at x,y if x,y are in boundaries; returns 0 otherwise
    if ((x < 0) || (x >= N)) return 0;
    if ((y < 0) || (y >= N)) return 0;
    return ptr[x][y];
}

template<int N>
void  Grid<N> :: change_val(int x, int y, double new_value) {
    // changes value at xy, if x,y are in boundaries; throws an error otherwise
    if ((x < 0) || (x >= N)) throw out_of_boundary_change();
    if ((y < 0) || (y >= N)) throw out_of_boundary_change();

    ptr[x][y] = new_value;

}

template<int N>
void Grid<N> :: create_disturbance (int x, int y, int width, int length,  double amplitude) {
    // sets the grid value equal to amplitude in an area from x-width/2 to x+width/2 and from y-height/2 to y+height/2
     for (int i = 0; i < width; i++) {
             for (int j = 0; j < length; j++) {
                 try {
                    change_val(x - width/ 2 + i, y - length /2 +  j, amplitude);
                 }

                 catch (const out_of_boundary_change& error) {}
             }
    }
}

template<int N>
Grid<N>&  Grid<N> :: operator+=(const Grid& other) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] += other.ptr[x][y];
        }

    return *this;
}

template<int N>
Grid<N> Grid<N> :: operator+(const Grid& other) {
    Grid temp(*this);
    temp += other;

    return *this;
}

template<int N>
Grid<N>& Grid<N> :: operator*= (double alpha) {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] *= alpha;
        }
    return *this;
}

template<int N>
Grid<N> Grid<N> :: operator*(const double alpha) const {
    Grid<N> temp(*this);
    temp *= alpha;
    return temp;
}

template<int N>
Grid <N>:: Grid (const Grid<N>& other) : Grid<N>() {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] = other.ptr[x][y];
        }
}

template<int N>
void  Grid<N> :: clear() {
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            ptr[x][y] = 0;
        }
}

//---------------------------------------------------------------------
// Scalar Field methods

template<int N>
double Scalar_Field<N> ::  phi(int x, int y) const {
    return phi_curr.val(x, y);
}

template<int N>
Scalar_Field<N> :: Scalar_Field(Grid<N> init_phi, Grid<N> init_dot_phi) : phi_prev(init_phi), phi_curr(init_phi+init_dot_phi*dt) {}

template<int N>
Scalar_Field<N> :: Scalar_Field() : Scalar_Field<N> :: Scalar_Field<N>( Grid<N>(), Grid<N>()) {}

template<int N>
void Scalar_Field<N> :: get_lapl() {
    lapl.clear();
    for (int x = 0; x < N; x++)
        for (int y = 1; y < N - 1; y++) {
            lapl.change_val(x, y, (phi(x,y+1) + phi(x, y-1) + phi(x+1, y) + phi(x-1, y) - phi(x,y)*4)/(dx*dx));
        }
}

template<int N>
void  Scalar_Field<N> :: evolve(std :: vector<ReflectingWall<N>> reflectingwalls, std :: vector<AbsorbingWall<N>> absorbingwalls, std :: vector<Impulse_Source<N>> impulsesources, std :: vector<Harmonic_Source<N>> harmonicsources) {
    get_lapl();

    temp.clear();
    temp += phi_curr;

    phi_curr *= 2;
    phi_curr += phi_prev*(-1);
    phi_curr += lapl*dt*dt;

    phi_prev.clear();
    phi_prev += temp;
    apply_boundaries(reflectingwalls, absorbingwalls);
    apply_condition(impulsesources, harmonicsources);
}

template<int N>
void Scalar_Field<N> :: create_disturbance (int x, int y, int width, int length,  int amplitude) {
    // sets the field value equal to amplitude in an area from x-width/2 to x+width/2 and from y-height/2 to y+height/2
     for (int i = 0; i < width; i++) {
             for (int j = 0; j < length; j++) {
                 try {
                    phi_curr.change_val(x - width/ 2 + i, y - length /2 +  j, amplitude);
                 }

                 catch (const out_of_boundary_change& error) {}
             }
    }
}

template<int N>
void  Scalar_Field<N> :: apply_boundaries(std :: vector<ReflectingWall<N>> rwalls, std :: vector<AbsorbingWall<N>> awalls) {
    for (auto wall : rwalls) {
        wall.apply_condition(phi_curr, phi_prev, dx, dt);
    }
    for (auto wall : awalls) {
        wall.apply_condition(phi_curr, phi_prev, dx, dt);
    }
}

template<int N>
void  Scalar_Field<N> :: apply_condition(std :: vector<Impulse_Source<N>> impulsesources, std :: vector<Harmonic_Source<N>> harmonicsources) {
    for (auto source : impulsesources) {
        source.apply_condition(phi_curr);
    }
    for (auto source : harmonicsources) {
        source.apply_condition(phi_curr);
    }
}

//---------------------------------------------------------------------
// Wall_methods

template <int N>
void Wall<N> :: draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(line, 2, sf :: Lines);
}

template <int N>
Wall<N> :: Wall(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical) : Drawable(),  wall_coordinate(wall_coordinate), start_coordinate(start_coordinate), end_coordinate(end_coordinate), vertical(vertical) {
     if (vertical) {
         line [0] =  sf::Vertex(sf::Vector2f(wall_coordinate, start_coordinate));
         line[1] = sf::Vertex(sf::Vector2f(wall_coordinate, end_coordinate));
    }

    else {
        line[0] =  sf::Vertex(sf::Vector2f(start_coordinate, wall_coordinate));
        line[1] =  sf::Vertex(sf::Vector2f( end_coordinate, wall_coordinate));
    }
 }




//---------------------------------------------------------------------
// Reflecting Wall methods
template<int  N>
void  ReflectingWall<N> ::  apply_condition (Grid<N>& phi_curr, Grid<N>& phi_prev, double dx, double dt) {
    if (Wall<N> :: vertical) {
        for (int y = Wall<N> :: start_coordinate; y < Wall<N> :: end_coordinate; y++) {
            phi_curr.change_val(Wall<N> :: wall_coordinate, y, 0);
        }
    }
    else {
        for (int x = Wall<N> :: start_coordinate; x < Wall<N> :: end_coordinate; x++){
            phi_curr.change_val(x,  Wall<N> :: wall_coordinate, 0);
        }
    }
 }

 template<int N>
ReflectingWall<N> :: ReflectingWall(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical) :
                                    Wall<N>(wall_coordinate, start_coordinate, end_coordinate,   vertical) {}


//---------------------------------------------------------------------
// Absorbing  Wall methods
template<int N>
void  AbsorbingWall<N> ::  apply_condition (Grid<N>& phi_curr, Grid<N>& phi_prev, double dx, double dt) {
    if (Wall<N> :: vertical) {
        int x = Wall<N> :: wall_coordinate;
        for (int y = Wall<N> :: start_coordinate;  y <  Wall<N> :: end_coordinate; y++) {
            double new_val = (1 - dt/ dx) *  phi_prev.val(x, y) + (dt / dx * phi_prev.val(x + orientation, y));
            phi_curr.change_val(x, y, new_val);
        }
    }
    else {
        int y = Wall<N> :: wall_coordinate;
        for (int x = Wall<N> :: start_coordinate;  x < Wall<N> ::  end_coordinate; x++) {
            double new_val = (1 - dt/ dx) *  phi_prev.val(x, y) + (dt / dx * phi_prev.val(x, y - orientation));
            phi_curr.change_val(x, y, new_val);
        }
    }
 }

template<int N>
AbsorbingWall<N> :: AbsorbingWall(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical, int orientation) :
    Wall<N> (wall_coordinate, start_coordinate, end_coordinate, vertical), orientation(orientation) {}



//---------------------------------------------------------------------
// Source methods
template<int N>
Source<N> :: Source(int x, int y) : x(x), y(y) {}

template<int N>
void Source<N> :: apply_condition (Grid<N>& grid) {}


//---------------------------------------------------------------------
// Impulse_Source methods

template<int N>
void Impulse_Source<N> :: disturb(Grid<N>& grid) {
    grid.create_disturbance(Source<N>::x, Source<N>::y, 5, 5, amplitude);
}

template<int N>
void Impulse_Source<N> :: apply_condition(Grid<N>& grid) {
    counter++;
    if (counter >= period) {
        disturb(grid);
        counter = 0;
    }
}

template<int N>
Impulse_Source<N> ::  Impulse_Source  (int x, int y, int period) : Source<N>::Source(x, y), period(period), amplitude(10) {}

template<int N>
Impulse_Source<N> ::  Impulse_Source  (int x, int y, int period, double amplitude) : Source<N>::Source(x, y), period(period), amplitude(amplitude) {}


//---------------------------------------------------------------------
// Harmonic_Source methods

template<int N>
void Harmonic_Source<N> :: apply_condition(Grid<N>& grid) {
    // setting disturbance
    double harmonic_value = amplitude * std::sin(phase);
    grid.create_disturbance(Source<N>::x, Source<N>::y, 5, 5, harmonic_value);

    // incrementing phase
    phase += 2 * M_PI / period;
}

template<int N>
Harmonic_Source<N> :: Harmonic_Source(int x, int y, double period, double amplitude, double phase) : 
    Source<N>::Source(x, y), period(period), amplitude(amplitude), phase(phase) {}

#endif
