#include <SFML/Graphics.hpp>
#include <map>
#include<string>

#include "simulation.hpp"


using namespace sim;

int main() {

    sf :: Font font;

    std :: string file = "../assets/sus.ttf";
     try {
         sim :: load_font_from_file(font, file );
    }

    catch (font_not_loaded& error) {
        std :: cout << error.what();
    }


    sim :: SimulationManager<300> manager;

    sim :: TimeManager time_manager;

    sim :: FPSdrawer fps_drawer(manager.get_size() / 15, 0, 0, font);

    //manager.disturb_my_scalar_field((manager.get_size()) / 2 , (manager.get_size()) / 2,  1, 100, 1);

    sf::RenderWindow window(sf::VideoMode(manager.get_size(), manager.get_size()), "SIV4");
    window.setFramerateLimit(manager.FPS);

    while (window.isOpen())
    {
        fps_drawer.set_FPS(time_manager.count_fps());

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Return) {
                    window.close();
                }

                if (event.key.code == sf::Keyboard:: Space) {
                    manager.pause();
                }
            }


            if (event.type = sf :: Event :: MouseButtonPressed) {

               manager.disturb_my_scalar_field(event.mouseButton.x, event.mouseButton.y,  5, 5,  1);
            }
        }


        window.clear(sf::Color::Black);

        manager.evolve_my_scalar_field();
        manager.draw_my_scalar_field(window);

        window.draw(fps_drawer);

        window.display();
}

std :: cout <<  "slow_frames " << time_manager.slow_frames_count() << '\n';
std :: cout << "time_elapsed " << time_manager.seconds_passed() << '\n';


return 0;
}

