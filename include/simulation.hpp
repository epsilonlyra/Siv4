#ifndef H_SIMULATION
#define H_SIMULATION

#include "field_class.hpp"

namespace sim  {

struct font_not_loaded final  : base_exception {
        std::string what() override;
};

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


    private:

        std :: map<std :: string, bool> game_state {{"paused", false}};

        fc :: Scalar_Field<size> scalar;

        sf :: Image image;

        sf :: Texture texture;

        sf :: Sprite sprite;
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
