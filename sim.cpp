#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "logic.h"
#include "string.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

sf::Font font;

class sfLine : public sf::Drawable {
    public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2):
        color(sf::Color::Yellow), thickness(5.f)
    {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i=0; i<4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices,4,sf::Quads);
    }


    private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};

class Gate {
    public:
    Gate() {}
    Gate(std::string name, std::string imPath, float x, float y) {
        this->name = name;
        this->label->setString(name);
        this->label->setCharacterSize(20);
        this->label->setFillColor(sf::Color(76, 96, 133));
        this->label->setFont(font);

        float size = name.length()/5.0;
        float textSize = name.length() * 12.0;

        this->label->setPosition(x + (100*size - textSize)/2.0, y+15);
        this->x = x;
        this->y = y;
        texture->loadFromFile(imPath);
        this->sprite.setTexture(*texture);
        this->sprite.setPosition(x, y);
        this->sprite.setScale(size, 1);
    }

    float x,y;
    sf::Text *label = new sf::Text;
    std::string name;
    sf::Texture *texture = new sf::Texture;
    sf::Sprite sprite;

    void draw() {

    }
};

class Window {
    private: 
    void run() {
        sf::Event event;
        while (this->window->isOpen()) {
            while (this->window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    this->window->close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    this->onKeyPress(event.key.code);
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    this->onMousePress(event.mouseButton);
                }
            }
            this->window->clear(sf::Color(50, 51, 44));  
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
    Gate zxc1;
    Gate zxc2;
    Gate zxc3;

    sf::Vertex line[4] =
    {
        sf::Vertex(sf::Vector2f(10.f, 10.f), sf::Color(57, 160, 237)),
        sf::Vertex(sf::Vector2f(150.f, 150.f), sf::Color(57, 160, 237)),
        sf::Vertex(sf::Vector2f(150.f, 150.f), sf::Color(57, 160, 237)),
        sf::Vertex(sf::Vector2f(200.f, 300.f), sf::Color(57, 160, 237)),
    };

    // called on setup 
    void setup() {
        font.loadFromFile("fonts/FiraCode-Bold.ttf");

        zxc = Gate("OR", "img/gate.png", 100, 80);
        zxc1 = Gate("AND", "img/gate.png", 100, 160);
        zxc2 = Gate("NOT", "img/gate.png", 100, 240);
        zxc3 = Gate("NAND", "img/gate.png", 100, 320);
    }

    // called on mouse press
    void onMousePress(sf::Event::MouseButtonEvent buttonEvent) {

    }

    // called every frame
    void onUpdate() {
        this->window->draw(this->zxc.sprite);
        this->window->draw(*this->zxc.label);

        this->window->draw(this->zxc1.sprite);
        this->window->draw(*this->zxc1.label);
        
        this->window->draw(this->zxc2.sprite);
        this->window->draw(*this->zxc2.label);

        this->window->draw(this->zxc3.sprite);
        this->window->draw(*this->zxc3.label);

        this->window->draw(line, 4, sf::Lines);
    }

    // called on key press
    void onKeyPress(int key_code) {
        std::cout << key_code << std::endl;
    }
};

int main()
{
    Window mainWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic gates are cool...");
    return 0;
}
