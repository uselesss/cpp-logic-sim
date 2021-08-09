#include <iostream>
#include <SFML/Graphics.hpp>
#include "logic.h"
#include "string.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

class Gate {
    public:
    Gate() {}
    Gate(std::string name, std::string imPath, float x, float y) {
        this->name = name;
        this->x = x;
        this->y = y;
        texture.loadFromFile(imPath);
        this->sprite.setTexture(texture);
        this->sprite.setPosition(x, y);
    }

    float x,y;
    std::string name;
    sf::Texture texture;
    sf::Sprite sprite;
};

class Window {
    private: 
    void run() {
        while (this->window->isOpen()) {
            sf::Event event;
            while (this->window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    this->window->close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    this->onKeyPress(event.key.code);
                }
            }
            this->window->clear(sf::Color(120, 120, 120));  
            this->onUpdate();
            this->window->display();
        }
    }

    public:
    Window(int screen_w, int screen_h, std::string title) {
        this->window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), title);
        this->window->setFramerateLimit(144);
        this->setup();
        this->run();
    }

    sf::RenderWindow *window;
    Gate zxc;

    void setup() {
        // called on setup 
        zxc = Gate("zxc", "logic.png", 100, 80);
    }

    void onMousePress(sf::Mouse::Button button, sf::Vector2i mouse_pos) {
        // called on mouse press
    }

    void onUpdate() {
        // called every frame
        this->window->draw(this->zxc.sprite);
    }

    void onKeyPress(int key_code) {
        // called on key press
        std::cout << key_code << std::endl;
    }
};

int main()
{
    Window mainWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic gates are cool...");
    return 0;
}
