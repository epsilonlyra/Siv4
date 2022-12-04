#ifndef H_FIELD_CLASS
#define H_FIELD_CLASS

#include<iostream>
#include<memory>
#include<vector>
#include <SFML/Graphics.hpp>

#include "errors.hpp"

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



    template<int N>
    class Wall : public sf :: Drawable {
        protected :
            int wall_coordinate;
            int start_coordinate = 0;
            int end_coordinate = 0;
            bool vertical = false;
            sf::Vertex line[2];


        public:

            Wall(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical);
            void virtual apply_condition (Grid<N>& phi_curr, Grid<N>& phi_prev, double dx, double dt) = 0;

            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };


    template<int N>
    class ReflectingWall final : public Wall<N> {
        public:
            void  apply_condition (Grid<N>& phi_curr, Grid<N>& phi_prev, const double dx, const double dt) override;

            ReflectingWall(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical);

    };

    template<int N>
    class AbsorbingWall  final : public Wall<N> {


        public:
            void  apply_condition (Grid<N>& phi_curr, Grid<N>& phi_prev, const double dx, const double dt) override;

            AbsorbingWall(int wall_coordinate, int start_coordinate, int end_coordinate,  bool vertical, int orientation);

        private:
           int orientation = 1;
};

    template <int N>
    class Scalar_Field final  {
        private:
            Grid<N> phi_curr;
            Grid<N> phi_prev;
            Grid<N> lapl;
            Grid<N> temp;


        public:
            double const dt = 0.7;
            double const dx = 1;

            double phi(int x, int y) const;

            Scalar_Field(Grid<N> init_phi, Grid<N> init_dot_phi);

            Scalar_Field();

            void get_lapl();

            void create_disturbance (int x, int y, int width, int lenght,  int amplitude);

            void evolve(std :: vector<ReflectingWall<N>> reflectingwalls, std :: vector<AbsorbingWall<N>> absorbingwalls);

            void apply_boundaries(std :: vector<ReflectingWall<N>> reflectingwalls, std :: vector<AbsorbingWall<N>> absorbingwalls);
    };

}

#include "field_class.tpp"

#endif
