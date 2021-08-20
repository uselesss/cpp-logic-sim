#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include "logic.h"
#include "string.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

sf::Font font;

class Line : public sf::Drawable {
    public:
    Line(const sf::Vector2f& point1, const sf::Vector2f& point2):
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

class Gate : public sf::Drawable {
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


    //TODO rework size & textsize
    void setPosition(float x, float y) {
        this->sprite.setPosition(x, y);

        float size = name.length()/5.0;
        float textSize = name.length() * 12.0;
        this->label->setPosition(x + (100*size - textSize)/2.0, y+15);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(this->sprite);
        target.draw(*this->label);
    }

    Gate* select() {
        this->sprite.setColor(sf::Color(57, 160, 237));
        return this;
    }

    Gate *deselect() {
        this->sprite.setColor(sf::Color::White);
        return nullptr;
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
                if (event.type == sf::Event::MouseButtonReleased) {
                    this->onMouseRelease(event.mouseButton);
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

    // -------------------
    sf::RenderWindow *window;
    Gate *selected = nullptr;
    bool mouseDown = false;

    // -------------------
    std::vector<Gate*> gates;
    int tool = 0;

    // called on setup 
    void setup() {
        font.loadFromFile("fonts/FiraCode-Bold.ttf");

        this->gates.push_back(new Gate("intel core i9", "img/gate.png", 100, 80));
        this->gates.push_back(new Gate("and", "img/gate.png", 180, 80));
        this->gates.push_back(new Gate("or", "img/gate.png", 200, 80));
        this->gates.push_back(new Gate("nand", "img/gate.png", 590, 80));
    }

    // called on mouse press
    void onMousePress(sf::Event::MouseButtonEvent buttonEvent) {
        sf::Vector2i mousePos = {buttonEvent.x, buttonEvent.y};
        sf::Mouse::Button button = buttonEvent.button;
        this->mouseDown = true;
        //Translate mouse position
        auto translated_pos = this->window->mapPixelToCoords(mousePos); 

        if (this->tool == 0) {
            for (int i=0; i<this->gates.size(); ++i) {
                if (gates[i]->sprite.getGlobalBounds().contains(translated_pos)) {
                    if (!this->selected) {
                        this->selected = gates[i]->select();
                    } else {
                        this->selected = gates[i]->deselect();
                    }
                }
            }
        } else {
            this->gates.push_back(new Gate("test", "img/gate.png", translated_pos.x, translated_pos.y));
        }
    }

    // called on mouse release
    void onMouseRelease(sf::Event::MouseButtonEvent buttonEvent) {
        this->mouseDown = false;
        if(this->selected) {
            if (this->selected->sprite.getPosition().x > 200) {
                this->gates.erase(std::remove(this->gates.begin(), this->gates.end(), this->selected), this->gates.end());
                this->selected = nullptr;
                
            } else this->selected = this->selected->deselect();
        }
    }

    // called every frame
    void onUpdate() {
        for (Gate *gate: gates) this->window->draw(*gate);

        if (this->mouseDown && this->selected) {
            auto mousePos = sf::Mouse::getPosition(*this->window);

            this->selected->x = mousePos.x;
            this->selected->y = mousePos.y;
            this->selected->setPosition(mousePos.x, mousePos.y);
        } 

    }

    // called on key press
    void onKeyPress(int key_code) {
        std::cout << key_code << std::endl;
        if (key_code > 26) this->tool = key_code - 27;
    }
};


void posix_death_signal(int signum)
{
	std::cout << "Check your pointers, bitch!" << std::endl;
    signal(signum, SIG_DFL);
	exit(3); 
}

int main()
{
    signal(SIGSEGV, posix_death_signal);

    Window mainWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic gates are cool...");
    return 0;
}
