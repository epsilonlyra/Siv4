#ifndef H_SIMULATION
#define H_SIMULATION

#include <vector>
#include "field_class.hpp"
#include "errors.hpp"

namespace sim  {


void load_font_from_file(sf :: Font& font, std :: string& adress_from_executable);

template<int N>
void update_Image(fc :: Scalar_Field<N>& scalar, sf :: Image& image);

class FPSdrawer final  : public sf :: Drawable  {
    private :
        sf :: Text text;

    public:
        FPSdrawer(int size, int x, int y, sf :: Font& font);

        void  set_FPS(int FPS);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

template<int size>
class SimulationManager final  {

    public  :

        int get_size();

        SimulationManager();

        const  int FPS = 30;

        void pause();

        void draw_my_scalar_field(sf :: RenderWindow& window);

        void evolve_my_scalar_field();

        void disturb_my_scalar_field(int x, int y, int width, int length, int amplitude);

        void add_reflecting_wall(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical);

        void add_super_absorber(int wall_coordinate, int start_coordinate, int end_coordinate, bool vertical);

        void add_harmonic_oscillator(int x, int y, double period, double amplitude, double phase);

        void add_impulse_oscillator(int x, int y, int period, double amplitude);

    private:

        std :: map<std :: string, bool> game_state {{"paused", false}};

        fc :: Scalar_Field<size> scalar;

        sf :: Image image;

        sf :: Texture texture;

        sf :: Sprite sprite;

        std :: vector<ReflectingWall<size>> reflectingwalls;

        std :: vector<AbsorbingWall<size>> absorbingwalls;

        std :: vector<Impulse_Source<size>> impulsesources;

        std :: vector<Harmonic_Source<size>> harmonicsources;

        fc ::  Impulse_Source<size> test_source = Impulse_Source<size>(200, 200, 10);

};

class TimeManager  final  {

    public:
         const int BAD_FPS = 27;

    int count_fps();

    int slow_frames_count();
    int seconds_passed();

    private:
        sf::Clock clock;

        sf::Time currentTime;

        sf::Time previousTime = clock.getElapsedTime();;

        int slow_counter = 0;

};
}

#include "simulation.tpp"

#endif
