#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <fstream>
#include <vector>

using namespace sf;
using namespace std;


string trash;

float width = 1366.0f;
float height = 768.0f;
int max_speed = 5;
int max_size = 50;
int objects = 100;

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
        size(rand() % max_size + 20)
    {
        color = Color(rand() % 256, rand() % 256, rand() % 256);

        killed=false;

        X = rand() % (int(width) - size * 2);
        Y = rand() % (int(height) - size * 2);
        Vector2f posicion(X, Y);

        setRadius(size);
        setPosition(posicion);
        setFillColor(color);
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

int main() {
    RenderWindow window(VideoMode::getFullscreenModes()[0], "Bouncing balls", Style::Fullscreen);
    window.setFramerateLimit(60);

    ifstream params("parametros.txt");
    if (!(params >> max_speed >> max_size >> objects)) {
        return 1;
    }

    Ball Balls[objects];

    Font font;
    if (!font.loadFromFile("pixelated.ttf")) {
        return EXIT_FAILURE;
    }

    bool aHold=false, aHoldPrev=false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::A) {
                aHold = true;
            }

            // Verificar si la tecla "A" se ha soltado
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::A) {
                aHold = false;
            }
        }

        Vector2i mousePosition = Mouse::getPosition(window);

        for (int i = 0; i < objects; i++) {
           Balls[i].move();
           if (Balls[i].distance(mousePosition.x, mousePosition.y) && aHold && !aHoldPrev) {
                Balls[i].kill();
            }
        }

         aHoldPrev = aHold;

        window.clear();

        for (int i = 0; i < objects; i++) {
            if (Balls[i].is_dead()) continue;
            else window.draw(Balls[i]);
        }

        window.display();
    }

    return 0;
}

