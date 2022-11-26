#include "field_class.hpp"
#include <SFML/Graphics.hpp>

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



const int FPS  = 30;

int main() {


    fc :: Scalar_Field scalar;

    scalar.create_disturbance((fc :: N) / 2 , (fc:: N) / 2, 50, 1);

    sf::RenderWindow window(sf::VideoMode(fc :: N, fc :: N), "SIV4");

    window.setFramerateLimit(FPS);


    sf :: Image image;
    image.create(fc :: N, fc :: N, sf :: Color ::  Red);

    update_Image(scalar, image);



    sf :: Texture texture;
    texture.create(fc :: N, fc :: N);
    texture.update(image);

    sf :: Sprite sprite;
    sprite.setTexture(texture);

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
            }


            if (event.type = sf :: Event :: MouseButtonPressed) {

                scalar.create_disturbance(event.mouseButton.x, event.mouseButton.y,  50, 1);
            }
        }


        window.clear(sf::Color::Black);

        window.draw(sprite);

        update_Image(scalar, image);

        texture.update(image);

        window.display();

        scalar.evolve(1);

        currentTime = clock.getElapsedTime();
        float fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        if (fps <= 27) {
            slow_counter++;
        }
        previousTime = currentTime;


    }
    std :: cout <<  "slow_frames " << slow_counter << '\n';
    std :: cout << "time_elapsed " <<  floor(currentTime.asSeconds()) << '\n';


    return 0;
}
