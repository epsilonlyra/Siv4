#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "simulation.hpp"


using namespace sim;


template<int size>
void analyze_string(SimulationManager<size>& manager, std::string init_str) {
    std::istringstream stream(init_str);
    std::string type;
    stream >> type;
    type = init_str.substr(0, init_str.find(" "));
    if ((type == "reflection_wall") || (type == "absorbtion_wall")) {
        int first_coord, second_begin, second_end;
        std::string position;
        stream >> first_coord >> second_begin >> second_end >> position;

        bool vertical = true;
        if (position == "vertical") vertical = true;
        if (position == "horizontal") vertical = false;

        if (type == "reflection_wall") manager.add_reflecting_wall(first_coord, second_begin, second_end, vertical);
        else manager.add_super_absorber(first_coord, second_begin, second_end, vertical);
    }
    if (type == "harmonic_source") {
        int x, y;
        double period, amplitude, phase;
        stream >> x >> y >> period >> amplitude >> phase;
        manager.add_harmonic_oscillator(x, y, period, amplitude, phase);
    }
    if (type == "impulse_source") {
        int x, y, period;
        double amplitude;
        stream >> x >> y >> period >> amplitude;
        manager.add_impulse_oscillator(x, y, period, amplitude);
    }
}

template<int size>
void analyze_input(SimulationManager<size>& manager) {
    std::ifstream file("../configurations/config.txt");
    while (true) {
        std::string temp;
        std::getline(file, temp);
        std::cout << "line: " << temp << std::endl;
        if (temp == "") break;
        analyze_string(manager, temp);
    }
    file.close();
}

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

    analyze_input(manager);
    // Space for configuration
    /*{
        manager.add_super_absorber(130, 0, 90, true);
        manager.add_super_absorber(130, 100, 145, true);
        manager.add_super_absorber(130, 155, 200, true);
        manager.add_super_absorber(130, 210, 300, true);
        manager.add_harmonic_oscillator(50, 150, 20, 4, 0);
        // manager.add_reflecting_wall(x, y_begin, y_end, true); -- vertical reflecting wall
        // manager.add_reflecting_wall(y, x_begin, x_end, true); -- horizontal reflecting wall
        // manager.add_super_absorber(x, y_begin, y_end, true); -- vertical absorbing wall
        // manager.add_super_absorber(y, x_begin, x_end, false); -- horizontal absorbing wall
        // manager.add_impulse_oscillator(x, y, period, ampl);
        // manager.add_harmonic_oscillator(x, y, period, ampl, phase);
    }*/

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

                if (event.key.code == sf::Keyboard::S) {
                    manager.switch_sources();
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

