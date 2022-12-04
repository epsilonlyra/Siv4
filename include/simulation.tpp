#ifndef T_SIMULATION
#define T_SIMULATION


using namespace sim;

template <int  N>
void sim :: update_Image(fc :: Scalar_Field<N>& scalar, sf :: Image& image) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            if (scalar.phi(i, j) > 0) {
                sf :: Color color( scalar.phi(i, j) * 255, 0, 0);
                image.setPixel(i, j, color);
            }
            else {
                sf :: Color color(0, 0, -scalar.phi(i, j) * 255);
                image.setPixel(i, j, color);
            }

        }
    }
}


//---------------------------------------------------------------------
//  SimulationManager methods

template <int size>
 int  SimulationManager<size> :: get_size() {
            return size;
}

template <int size>
SimulationManager<size> :: SimulationManager() {
            texture.create(size, size);
            sprite.setTexture(texture);
            image.create(size, size, sf :: Color ::  Red);

            reflectingwalls.emplace_back(100, 0, 300, true);
            reflectingwalls.emplace_back(200, 0, 300, false);
            reflectingwalls.emplace_back(250,0, 300, true);



}

template<int size>
void SimulationManager<size> ::  pause() {
            game_state["paused"] = !game_state["paused"];
}

template<int size>
void  SimulationManager<size> :: draw_my_scalar_field(sf :: RenderWindow& window) {
            update_Image(scalar, image);
            texture.update(image);
            window.draw(sprite);
            for (auto wall : reflectingwalls) {
            window.draw(wall);
            }
}


template<int size>
void  SimulationManager<size> :: evolve_my_scalar_field() {
    if (!game_state["paused"]) {
        scalar.evolve(reflectingwalls);
    }
}

template <int size>
void SimulationManager<size> :: disturb_my_scalar_field(int x, int y, int width, int length, int amplitude) {
    scalar.create_disturbance(x, y, width, length, amplitude);
}


#endif
