#include<simulation.hpp>


using namespace sim;


std::string font_not_loaded  :: what()  {
            return std::string("Font not found  on the expected reliative path from the executable\n");
}

void load_font_from_file(sf :: Font& font, std :: string& adress_from_executable) {
    if(!font.loadFromFile(adress_from_executable)) {
        throw(font_not_loaded());
}



//---------------------------------------------------------------------
// FPSdrawer methods


FPSdrawer :: FPSdrawer (int size, int x, int y, sf :: Font& font) {
            text.setFont(font);
            text.setCharacterSize(size);
            text.setFillColor(sf::Color::Green);
            text.setPosition(x, y);
}


 void  FPSdrawer  set_FPS(int FPS) {
            text.setString(std :: to_string(FPS));
}


void  FPSdrawer draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(text);
}

//---------------------------------------------------------------------
// TimeManager methods


int  TimeManager :: count_fps() {

        currentTime = clock.getElapsedTime();

        int  fps =  floor(1.0f / (currentTime.asSeconds() - previousTime.asSeconds()));
        if (fps <= BAD_FPS) {
            slow_counter++;
        }
        previousTime = currentTime;
        return fps;
}


int TimeManager ::  slow_frames_count() {
        return slow_counter;
    }

    int  TimeManager :: seconds_passed() {
        return  floor(currentTime.asSeconds());
    }







