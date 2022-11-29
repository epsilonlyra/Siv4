
#include "field_class.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include<string>

template<int N>
void update_Image(fc :: Scalar_Field<N>& scalar, sf :: Image& image) {

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


class SimulationManager {

    public  :

        int const  static size = 300;

        SimulationManager() {
            texture.create(size, size);
            sprite.setTexture(texture);
            image.create(size, size, sf :: Color ::  Red);
        }

        const  static int FPS = 30;



        void pause() {
            game_state["paused"] = !game_state["paused"];
        }


        void draw_my_scalar_field(sf :: RenderWindow& window) {
            update_Image(scalar, image);
            texture.update(image);
            window.draw(sprite);
        }

        void evolve_my_scalar_field(double dt) {

            if (!game_state["paused"]) {
                scalar.evolve(dt);
            }
        }


        void disturb_my_scalar_field(int x, int y, int width, int length, int amplitude) {
            scalar.create_disturbance(x, y, width, length, amplitude);
        }


    private:

        std :: map<std :: string, bool> game_state {{"paused", false}};


        fc :: Scalar_Field<size> scalar;

        sf :: Image image;

        sf :: Texture texture;

        sf :: Sprite sprite;
};

class TimeManager  {

    public:
         const static int BAD_FPS = 27;

    int count_fps() {

        currentTime = clock.getElapsedTime();

        int  fps =  floor(1.0f / (currentTime.asSeconds() - previousTime.asSeconds()));
        if (fps <= BAD_FPS) {
            slow_counter++;
        }
        previousTime = currentTime;
        return fps;
    }


    int slow_frames_count() {
        return slow_counter;
    }

    int seconds_passed() {
        return  floor(currentTime.asSeconds());
    }

    private:
        sf::Clock clock;

        sf::Time currentTime;

        sf::Time previousTime = clock.getElapsedTime();;

        int slow_counter = 0;

};


int main() {

    SimulationManager manager;

    TimeManager time_manager;

    manager.disturb_my_scalar_field((manager.size) / 2 , (manager.size) / 2,  10, 100, 1);

    sf::RenderWindow window(sf::VideoMode(manager.size, manager.size), "SIV4");
    window.setFramerateLimit(manager.FPS);


    sf::Vertex line1 [] =
    {
    sf::Vertex(sf::Vector2f(78, 0)),
    sf::Vertex(sf::Vector2f(78, manager.size  / 2 - 5 ))
    };

    sf::Vertex line2 [] =
    {
    sf::Vertex(sf::Vector2f(78, manager.size / 2 + 5 )),
    sf::Vertex(sf::Vector2f(78, manager.size ))
    };


    while (window.isOpen())
    {
        time_manager.count_fps();

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

               manager.disturb_my_scalar_field(event.mouseButton.x, event.mouseButton.y,  50, 50,  1);
            }
        }


        window.clear(sf::Color::Black);



        manager.evolve_my_scalar_field(1);
        manager.draw_my_scalar_field(window);

        window.draw(line1, 2, sf :: Lines);
        window.draw(line2, 2, sf :: Lines);

        window.display();
}

    std :: cout <<  "slow_frames " << time_manager.slow_frames_count() << '\n';
    std :: cout << "time_elapsed " << time_manager.seconds_passed() << '\n';

    return 0;
}

