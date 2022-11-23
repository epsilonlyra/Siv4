#include "field_class.hpp"
#include <SFML/Graphics.hpp>

void update_Image(fc :: Scalar_Field& scalar, sf :: Image& image) {

    for (int i = 0; i < fc ::  N; i++) {
        for (int j = 0; j < fc ::  N; j++) {

            if (scalar.phi(i, j) > 0) {
                sf :: Color color(250 * scalar.phi(i, j), 0, 0);
                image.setPixel(i, j, color);
            }
            else {
                image.setPixel(i, j, sf :: Color :: Black);
            }

        }
    }
}
const int FPS  = 30;

int main() {


    fc :: Grid grid;
    fc :: Grid grid2;

    for (int i = 0; i < 10; i++) {
             for (int j = 0; j < 10; j++){
                 grid.change_val(10 + i, 10 + j, 1);
             }
    }


    fc :: Scalar_Field scalar (grid,grid2);


    sf::RenderWindow window(sf::VideoMode(fc :: N, fc :: N), "SIV4");
    window.setFramerateLimit(30);


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


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear(sf::Color::Black);

        window.draw(sprite);
        update_Image(scalar, image);
        scalar.evolve(1);
        texture.update(image);


        window.display();

        currentTime = clock.getElapsedTime();
        float fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        std::cout << "fps =" << floor(fps) << std::endl;
        previousTime = currentTime;
    }


    return 0;
}
