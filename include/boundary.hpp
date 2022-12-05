#ifndef H_BOUNDARY
#define H_BOUNDARY


extern template<int N>
class  Grid  final {};

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


#include "boundary.tpp"
#endif
