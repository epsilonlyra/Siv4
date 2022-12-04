#include "simulation.hpp"

#include <cmath>



void sim :: load_font_from_file(sf :: Font& font, std :: string& adress_from_executable) {
    if(!font.loadFromFile(adress_from_executable)) {
        throw(font_not_loaded());
    }
}



//---------------------------------------------------------------------
// FPSdrawer methods


sim :: FPSdrawer :: FPSdrawer (int size, int x, int y, sf :: Font& font) {
            text.setFont(font);
            text.setCharacterSize(size);
            text.setFillColor(sf::Color::Green);
            text.setPosition(x, y);
}


 void  sim :: FPSdrawer ::  set_FPS(int FPS) {
            text.setString(std :: to_string(FPS));
}


void  sim :: FPSdrawer ::  draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(text);
}

//---------------------------------------------------------------------
// TimeManager methods


int sim ::  TimeManager :: count_fps() {

        currentTime = clock.getElapsedTime();

        int  fps =  floor(1.0f / (currentTime.asSeconds() - previousTime.asSeconds()));
        if (fps <= BAD_FPS) {
            slow_counter++;
        }
        previousTime = currentTime;
        return fps;
}


int sim ::  TimeManager ::  slow_frames_count() {
        return slow_counter;
    }

    int sim ::  TimeManager :: seconds_passed() {
        return  floor(currentTime.asSeconds());
    }






