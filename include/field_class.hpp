#ifndef H_FIELD_CLASS
#define H_FIELD_CLASS

#include<iostream>
#include <cmath>
#include<memory>



    struct base_exception {
        std :: string virtual what() = 0;
    };

    struct out_of_boundary_change final : base_exception {
        std::string what() override;
    };




namespace fc  {

    const int N = 300;

    class Grid {
        private:

            std :: unique_ptr <std :: unique_ptr <double []> []> ptr;

        public:
            Grid();
                // intitializes Grid filled with zeros


            double val(int x, int y) const;
                // returns value at x,y if x,y are in boundaries; returns 0 otherwise


            void change_val(int x, int y, double new_value);
                // changes value at xy, if x,y are in boundaries; throws an error otherwise


            Grid partial_x();

            Grid partial_y();

            Grid& operator+=(const Grid& other);

            Grid operator+(const Grid& other);

            Grid& operator*(double alpha);


            // destructor (1/5)

            ~Grid();

            // copy_constructor (2/5)
            Grid(const Grid& other);




    };

    class Scalar_Field {
        private:
            Grid phi_grid;
            Grid dot_phi_grid;
            const Grid c;




        public:
            double phi(int x, int y) const;

            double dot_phi(int x, int y) const;

            Scalar_Field(Grid init_phi, Grid init_dot_phi);

            Scalar_Field();

            void create_disturbance (int x, int y, int width, int lenght,  int amplitude);

            void evolve(double dt);

            void apply_boundaries();


    };

}

#endif
