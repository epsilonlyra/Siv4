#ifndef H_FIELD_CLASS
#define H_FIELD_CLASS

#include<iostream>
#include <cmath>


struct base_exception {
    std :: string virtual what() = 0;
};

struct out_of_boundary_change final : base_exception {
    std::string what() override;
};



namespace fc  {
    const int N = 500;


    class Grid {
        private:
            double** ptr;

        public:
            Grid();
                // intitializes Grid filled with zeros


            double val(int x, int y) const;
                // returns value at x,y if x,y are in boundaries; returns 0 otherwise


            void change_val(int x, int y, double new_value);
                // changes value at xy, if x,y are in boundaries; throws an error otherwise

            Grid& operator*=(const double t);

            Grid& operator+=(const Grid& other);

            void increment(const Grid& other, double dt);
    };

    class Scalar_Field {
        private:
            Grid phi_grid;
            Grid dot_phi_grid;
            Grid phi_xx;
            Grid phi_yy;
            const Grid c;

            void get_phi_xx();
            void get_phi_yy();

            void apply_borders();

        public:
            double phi(int x, int y) const;

            double dot_phi(int x, int y) const;

            Scalar_Field(Grid init_phi, Grid init_dot_phi);

            void update_phi(int x, int y, int side, int amplitude);

            void evolve(double dt);
    };

}

#endif
