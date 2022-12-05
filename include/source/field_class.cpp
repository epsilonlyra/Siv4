#include "field_class.hpp"


class Source {
    public :
        void update();

        void move();


        void disturb();

    private :
        int counter = 0;
        int x;
        int y;
        int vx;
        int vy;


};
