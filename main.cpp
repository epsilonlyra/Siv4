#include "field_class.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include<string>
void update_Image(fc :: Scalar_Field& scalar, sf :: Image& image) {

    for (int i = 0; i < fc ::  N; i++) {
        for (int j = 0; j < fc ::  N; j++) {

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


class Game_manager {

    public  :


        Game_manager() {
            texture.create(fc :: N, fc :: N);
            sprite.setTexture(texture);
            image.create(fc :: N, fc :: N, sf :: Color ::  Red);
        }

        const  static int FPS = 30;

        const static int BAD_FPS = 27;

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


        fc :: Scalar_Field scalar;

        sf :: Image image;

        sf :: Texture texture;

        sf :: Sprite sprite;


};


int main() {

    Game_manager manager;

    manager.disturb_my_scalar_field((fc :: N) / 2 , (fc:: N) / 2,  10, 100, 5);

    sf::RenderWindow window(sf::VideoMode(fc :: N, fc :: N), "SIV4");
    window.setFramerateLimit(manager.FPS);


    sf::Vertex line1 [] =
    {
    sf::Vertex(sf::Vector2f(78, 0)),
    sf::Vertex(sf::Vector2f(78, fc :: N  / 2 - 5 ))
    };

    sf::Vertex line2 [] =
    {
    sf::Vertex(sf::Vector2f(78, fc :: N / 2 + 5 )),
    sf::Vertex(sf::Vector2f(78, fc :: N ))
    };

    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;

    int slow_counter = 0;

    while (window.isOpen())
    {
        currentTime = clock.getElapsedTime();

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

        currentTime = clock.getElapsedTime();
        float fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        if (fps <= manager.BAD_FPS) {
            slow_counter++;
        }
        previousTime = currentTime;


}

    std :: cout <<  "slow_frames " << slow_counter << '\n';
    std :: cout << "time_elapsed " <<  floor(currentTime.asSeconds()) << '\n';


    return 0;
}
