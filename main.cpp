#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
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

using namespace sf;
using namespace std;

float width = VideoMode::getDesktopMode().width;
float height = VideoMode::getDesktopMode().height;
int max_speed = 5;
int max_size = 50;
int objects = 100;
int stroke = 1;

//void srand(time(0));

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
    float easing = 0.5f;

    float Mx = Mouse::getPosition(window).x;
    float My = Mouse::getPosition(window).y;


    //Horizontal Line
    //VertexArray lineh(LineStrip, 4);
    //lineh[0].position = Vector2f(0, My+stroke);
    //lineh[1].position = Vector2f(width, My+stroke);
    //lineh[2].position = Vector2f(0, My-stroke);
    //lineh[3].position = Vector2f(width, My-stroke);
    
    //Vertical line
    //VertexArray linev(LineStrip, 2);
    //linev[0].position = Vector2f(Mx, 0);
    //linev[1].position = Vector2f(Mx, height);

    //Horizontal
    lineh.setPosition(0,My);
    lineh.setOutlineThickness(2.f); // Set outline thickness
    lineh.setOutlineColor(Color::Red); // Set outline color
    lineh.setFillColor(Color::Red); // Set fill color to transparent
    
    //Vertical
    linev.setPosition(Mx,0);
    linev.setOutlineThickness(2.f); // Set outline thickness
    linev.setOutlineColor(Color::Red); // Set outline color
    linev.setFillColor(Color::Red); // Set fill color to transparent
    
    //Circle
    circle.setPosition(Mx-50,My-50);
    circle.setOutlineThickness(2.f); // Set outline thickness
    circle.setOutlineColor(Color::Red); // Set outline color
    circle.setFillColor(Color::Transparent); // Set fill color to transparent
 
    window.draw(lineh);
    window.draw(linev);
    window.draw(circle);
}

void pause_screen(Font& font) {
    
}

void game_over_screen(RenderWindow& window, Font& font) {
    Text gameOver;
    gameOver.setFont(font); 
    gameOver.setString("Game Over!"); 
    gameOver.setCharacterSize(128);
    gameOver.setFillColor(Color::White);
    gameOver.setPosition((width - gameOver.getLocalBounds().width)/2, 
                         (height - gameOver.getLocalBounds().height)/2);
    window.draw(gameOver);
}

//Unused
void reset_game(Ball balls[]) {
    for (int i=0;i<objects;i++) {
        balls->revive();
    }
}

void draw_score(RenderWindow& window, Font& font) {

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

    bool aHold=false, aHoldPrev=false, game_over=false;

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
        
        //Logica
        float Mx = Mouse::getPosition(window).x;
        float My = Mouse::getPosition(window).y;

        for (int i = 0; i < objects; i++) {
            if (!Balls[i].is_dead()) {
                if (Balls[i].distance(Mx, My) && aHold && !aHoldPrev) {
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

        window.display();

        if (game_over) {
            buffer.loadFromFile("GameOver.ogg");
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
