#ifndef T_FIELD_CLASS
#define T_FIELD_CLASS


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
Grid <N> Grid<N> :: partial_x() {
    Grid temp = Grid();
    for (int x = 1; x < N - 1; x++)
        for (int y = 0; y < N; y++) {
            temp.change_val(x, y, (val(x + 1, y) - val(x - 1, y)) / 2);
        }

    return temp;
}

template<int N>
Grid <N> Grid<N> ::  partial_y() {
    Grid temp;
    for (int x = 0; x < N; x++)
        for (int y = 1; y < N - 1; y++) {
            temp.change_val(x, y, (val(x, y + 1) - val(x, y - 1)) / 2);
        }

    return temp;
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
    Grid<N> temp;
    temp.clear();
    temp += (*this); 
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
        for (int y = 0; y < N; y++) {
            lapl.change_val(x, y, (phi(x,y+1) + phi(x, y-1) + phi(x+1, y) + phi(x-1, y) - phi(x,y))/(dx*dx));
        }
} 

template<int N>
void  Scalar_Field<N> :: evolve() {
    get_lapl();

    temp.clear();
    temp += phi_curr;

    phi_curr *= 2;
    phi_curr += phi_prev*(-1);
    phi_curr += lapl*dt*dt;

    phi_prev.clear();
    phi_prev += temp;

    apply_boundaries();
}

template<int N>
void Scalar_Field<N> :: create_disturbance (int x, int y, int width, int length,  int amplitude) {
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
void  Scalar_Field<N> :: apply_boundaries() {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if ((x == 0) || (x == N - 10)) phi_curr.change_val(x, y, 0);
            if ((y == 0) || (y == N - 10)) phi_curr.change_val(x, y, 0);
            if (x == 76 && ((y <= (N) / 2 - 5 )|| (y >= N/ 2 + 5 ))) phi_curr.change_val(x, y, 0);

            // for some reason this hase to be at least two pixels in width
            if (x == 77 && ((y <= ( N) / 2 - 5 )|| (y >= (N ) / 2 + 5 ))) phi_curr.change_val(x, y, 0);
            if (x == 76 && ((y <= (N) / 2 - 5 )|| (y >= ( N ) / 2 + 5 ))) phi_curr.change_val(x, y, 0);
        }
    }

}

#endif
