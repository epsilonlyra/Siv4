#ifndef T_BOUNDARY
#define T_BOUNDARY


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
            phi_curr.change_val(x - orientation, y, 0);
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

#endif // T_BOUNDARY
