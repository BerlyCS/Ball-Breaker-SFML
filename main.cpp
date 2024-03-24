#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

using namespace sf;
using namespace std;

const float width = VideoMode::getDesktopMode().width;
const float height = VideoMode::getDesktopMode().height;
int max_speed = 5;
int max_size = 50;
int objects = 100;
int stroke = 1;

//srand(time(0));

class Ball : public CircleShape {
private:
    float velY;
    float velX;
    int size;
    int X;
    int Y;
    Color color;
    bool killed;

public:
    Ball():
        velX(rand()%max_speed+1),
        velY(rand()%max_speed+1),
        size(rand() % max_size + 20),
        killed(false)
    {
        X = rand() % (int(width) - size * 2);
        Y = rand() % (int(height) - size * 2);

        velX = velX * pow(-1,rand()%2+1);
        velY = velY * pow(-1,rand()%2+1);

        setRadius(size);
        setPosition(Vector2f(X,Y));
        setOutlineColor(Color::Black);
        setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
    }

    void setVelY(float vel) {velY = vel;}
    void setVelX(float vel) {velX = vel;}
    float getVelY() {return velY;}
    float getVelX() {return velX;}
    void setPosX(float X) {this->X=X;}
    void setPosY(float Y) {this->Y=Y;}
    int getPosX(){return X;}
    int getPosY(){return Y;}
    void kill() {killed=true;}
    bool is_dead() {return killed;}
    void revive() {killed=false;}
    
    //Movement
    virtual void move() {
        X = X + velX;
        if (X < 0 || X > width - size * 2) {
            velX *= -1;
        }
        Y = Y + velY;
        if (Y < 0 || Y > height - size * 2) {
            velY *= -1;
        }
        setPosition(Vector2f(X,Y));
    }
    
    //Collision with cursor
    bool distance(int Mx, int My) {
        float dist = sqrt((Mx - X - size) * (Mx - X -size) + (My - Y-size) * (My - Y-size));
        if (dist-1 < size) {
            return true;
        }
        return false;
    }

    //Unused
    void change_color() {
        setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
    }

};

void dr_line(RenderWindow& window, RectangleShape& lineh, RectangleShape& linev, CircleShape& circle ) {
    //Unused value
    float easing = 0.5f;

    float Mx = Mouse::getPosition(window).x;
    float My = Mouse::getPosition(window).y;

    //Horizontal
    lineh.setPosition(0,My);
    lineh.setOutlineThickness(2.f);
    lineh.setOutlineColor(Color::Red); 
    lineh.setFillColor(Color::Red); 

    //Vertical
    linev.setPosition(Mx,0);
    linev.setOutlineThickness(2.f);
    linev.setOutlineColor(Color::Red); 
    linev.setFillColor(Color::Red); 
    
    //Circle
    circle.setPosition(Mx-50,My-50);
    circle.setOutlineThickness(2.f); 
    circle.setOutlineColor(Color::Red); 
    circle.setFillColor(Color::Transparent);  

    window.draw(lineh);
    window.draw(linev);
    window.draw(circle);
}

void menu(RenderWindow& window, Font& font, bool& game_start) {

    int n=100;
    Ball menu[n];

    RectangleShape bg(Vector2(width, height));
    bg.setFillColor(Color(0,0,0,192));

    int borde = 10;
    int offset_text=37 - borde;
    
    //Start Menu Text --------------
    int start_width_pos=30, start_height_pos=10;
    Vector2f start_pos(start_width_pos,start_height_pos);

    Text start("Start Game", font, 100);
    start.setFillColor(Color::White);
    start.setPosition(start_pos);

    RectangleShape s_start(Vector2f(start.getGlobalBounds().width+borde*2, start.getGlobalBounds().height+borde*2));
    s_start.setFillColor(Color(128,128,128,64));
    //s_start.setOutlineColor(Color(64,64,64));
    s_start.setPosition(start_width_pos-borde,start_height_pos+offset_text);

    //Options Menu Text ---------------
    int option_width_pos=30, option_height_pos=108;
    Vector2f option_pos(option_width_pos,option_height_pos);

    Text Options("Options",font,100);
    Options.setFillColor(Color::White);
    Options.setPosition(option_pos);

    //Options Rectangle Transparent text
    RectangleShape s_Options(Vector2f(Options.getGlobalBounds().width+borde*2, Options.getGlobalBounds().height+borde*2));
    s_Options.setFillColor(Color(128,128,128,64));
    //s_Options.setOutlineColor(Color(64,64,64));
    s_Options.setPosition(option_width_pos-borde, option_height_pos+offset_text);

    //Exit Menu Text ------------
    int exit_width_pos=30, exit_heigth_pos=230;
    Vector2f exit_pos(exit_width_pos,exit_heigth_pos);
    
    Text Exit("Exit",font, 100);
    Exit.setFillColor(Color::White);
    Exit.setPosition(exit_pos);

    RectangleShape s_Exit(Vector2f(Exit.getGlobalBounds().width+borde*2, Exit.getGlobalBounds().height+borde*2));
    s_Exit.setFillColor(Color(128,128,128,64));
    s_Exit.setPosition(exit_pos.x-borde, exit_pos.y+offset_text);

    //Bucle principal de menu
    while (window.isOpen()) {

        Event event;
        //Eventos
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::S)) {
                game_start=true;
                return;
            }

            //TODO if exit_text pressed then close;
        }

        float Mx = Mouse::getPosition(window).x;
        float My = Mouse::getPosition(window).y;

        for (int i=0;i<n;i++) {
            menu[i].move();
            window.draw(menu[i]);
        }

        window.draw(bg);
        window.draw(s_start);
        window.draw(start);
        window.draw(s_Options);
        window.draw(Options);
        window.draw(s_Exit);
        window.draw(Exit);

        window.display();
        window.clear();
    }

}

void pause_screen(Font& font) {

}

void game_over_screen(RenderWindow& window, Font& font) {
    SoundBuffer buffer;
    buffer.loadFromFile("GameOver.ogg");
    Text gameOver("Game Over!",font,128);
    gameOver.setFillColor(Color::White);
    gameOver.setPosition((width - gameOver.getLocalBounds().width)/2, 
                        ((height - gameOver.getLocalBounds().height)/2-45));
                        // -45 because the text appears very low;

    window.draw(gameOver);
}

//Unused
void reset_game(Ball balls[]) {
    for (int i=0;i<objects;i++) {
        balls->revive();
    }
}

void draw_score(RenderWindow& window, Font& font, int score, Text& src) {
    src.setString("Score: "+ to_string(score));
    window.draw(src);
}

int main() {
    RenderWindow window(VideoMode::getFullscreenModes()[0], "Bouncing balls", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    RectangleShape lineh(Vector2f(width, stroke));
    RectangleShape linev(Vector2f(stroke, height));
    CircleShape circle(50.f);

    ifstream params("parametros.txt");
    if (!(params >> max_speed >> max_size >> objects)) {
        return 1;
    }

    Ball Balls[objects];

    Font font;
    if (!font.loadFromFile("pixelated.ttf")) {

        return EXIT_FAILURE;
    }

    int score=0;
    Text Score("", font, 50);
    Score.setPosition(Vector2f(10,10));

    bool aHold=false, aHoldPrev=false, game_over=false, game_start=false;

    SoundBuffer buffer;
    if (!buffer.loadFromFile("Fire.ogg")) {
        return 1;
    }
    
    Sound fire(buffer);

    SoundBuffer b2;
    if (!b2.loadFromFile("Hit.ogg")) {
        return EXIT_FAILURE;
    }

    Sound hit(b2);


    while (window.isOpen()) {
        Event event;
        //Eventos
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            
            //Detecta un disparo
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::B 
                                                || event.type == Event::MouseButtonPressed) {
                fire.play();
                aHold = true;
            }

            if (event.type == Event::KeyReleased && event.key.code == Keyboard::B 
                                                 || event.type == sf::Event::MouseButtonReleased) {
                aHold = false;
            }
        }

        if (!game_start) {
            menu(window, font, game_start);
            continue;
        }
        
        //Logica
        float Mx = Mouse::getPosition(window).x;
        float My = Mouse::getPosition(window).y;

        for (int i = 0; i < objects; i++) {
            if (!Balls[i].is_dead()) {
                if (Balls[i].distance(Mx, My) && aHold && !aHoldPrev) {
                    score++;
                    hit.play();
                    Balls[i].kill();
                }
            }
           Balls[i].move();
        }

        aHoldPrev = aHold;

        window.clear();

        game_over=true;

        for (int i = 0; i < objects; i++) {
            if (Balls[i].is_dead()) {
                continue;
            }
            else {
                game_over=false;
                window.draw(Balls[i]);
            }
        }

        dr_line(window, lineh, linev, circle);
        draw_score(window, font, score, Score);
        
        window.display();

        if (game_over) {
            Sound game_over_fx;
            game_over_fx.setBuffer(buffer);
            game_over_fx.play();
            while (game_over_fx.getStatus() == Sound::Playing) {
                dr_line(window, lineh, linev, circle);
                game_over_screen(window, font);
                window.display();
                window.clear();
            }
            window.close();
        }

    }
    return EXIT_SUCCESS;
}
