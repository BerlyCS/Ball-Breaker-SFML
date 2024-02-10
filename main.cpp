#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace sf;
using namespace std;

float width = VideoMode::getDesktopMode().width;
float height = VideoMode::getDesktopMode().height;
int max_speed = 5;
int max_size = 50;
int objects = 100;
int stroke = 1;

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
        velX((rand()%2 == 0 ? -1 : 1)*rand()%max_speed+1),
        velY((rand()%2 == 0 ? -1 : 1)*rand()%max_speed+1),
        size(rand() % max_size + 20),
        killed(false)
    {
        X = rand() % (int(width) - size * 2);
        Y = rand() % (int(height) - size * 2);

        setRadius(size);
        setPosition(Vector2f(X,Y));
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

    bool distance(int Mx, int My) {
        float dist = sqrt((Mx - X - size) * (Mx - X -size) + (My - Y-size) * (My - Y-size));
        if (dist-1 < size) {
            return true;
        }
        return false;
    }

    void change_color() {
        setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
    }

};

void dr_line(RenderWindow& window, float Mx, float My, RectangleShape& lineh, RectangleShape& linev, CircleShape& circle ) {
    float easing = 0.5f;
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
    lineh.setFillColor(Color::White); // Set fill color to transparent
    
    //Vertical
    linev.setPosition(Mx,0);
    linev.setOutlineThickness(2.f); // Set outline thickness
    linev.setOutlineColor(Color::Red); // Set outline color
    linev.setFillColor(Color::White); // Set fill color to transparent
    
    //Circle
    circle.setPosition(Mx-50,My-50);
    circle.setOutlineThickness(2.f); // Set outline thickness
    circle.setOutlineColor(Color::Red); // Set outline color
    circle.setFillColor(Color::Transparent); // Set fill color to transparent

    //window.draw(linev);
    window.draw(lineh);
    window.draw(linev);
    window.draw(circle);


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

    bool aHold=false, aHoldPrev=false, game_finish=false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::B) {
                aHold = true;
            }

            if (event.type == Event::KeyReleased && event.key.code == Keyboard::B) {
                aHold = false;
            }
        }
  
        float Mx = Mouse::getPosition(window).x;
        float My = Mouse::getPosition(window).y;

        for (int i = 0; i < objects; i++) {
           if (Balls[i].distance(Mx, My) && aHold && !aHoldPrev) {
                Balls[i].kill();
            }
           Balls[i].move();
        }
        aHoldPrev = aHold;

        window.clear();

        game_finish=true;

        for (int i = 0; i < objects; i++) {
            if (Balls[i].is_dead()) {
                continue;
            }
            else {
                game_finish=false;
                window.draw(Balls[i]);
            }
        }

        if (game_finish) {
            window.close();
        }

        dr_line(window, Mx, My, lineh, linev, circle);

        window.display();
    }
    return 0;
}
