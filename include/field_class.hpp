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

    template<int N >
    class Grid final {
        private:

            std :: unique_ptr <std :: unique_ptr <double []> []> ptr;

        public:
            Grid();
                // intitializes Grid filled with zeros


            double val(int x, int y) const;
                // returns value at x,y if x,y are in boundaries; returns 0 otherwise


            void change_val(int x, int y, double new_value);
                // changes value at xy, if x,y are in boundaries; throws an error otherwise

            void clear();

            Grid partial_x();

            Grid partial_y();

            Grid& operator+=(const Grid& other);

            Grid operator+(const Grid& other);

            Grid operator*(double alpha) const;
            
            Grid& operator*= (double alpha);

            // copy_constructor (2/5)
            Grid(const Grid& other);
    };


    template <int N>
    class Scalar_Field final  {
        private:
            Grid<N> phi_curr;
            Grid<N> phi_prev;
            Grid<N> lapl;
            Grid<N> temp;
            double const dt=0.02;
            double const dx=1;

        public:
            double phi(int x, int y) const;

            Scalar_Field(Grid<N> init_phi, Grid<N> init_dot_phi);

            Scalar_Field();

            void get_lapl();

            void create_disturbance (int x, int y, int width, int lenght,  int amplitude);

            void evolve();

            void apply_boundaries();
    };

}

#include<field_class.tpp>

#endif
