#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include "logic.h"
#include "string.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

sf::Font font;
bool fpsDebug = true;

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

        float size = name.length()/3;
        float textSize = name.length() * 12.0;

        this->label->setPosition(x + (100*size - textSize)/2.0, y+15);
        this->x = x;
        this->y = y;
        texture->loadFromFile(imPath);
        this->sprite.setTexture(*texture);
        this->sprite.setColor(spriteColor);
        this->sprite.setPosition(x, y);
        this->sprite.setScale(size, 1);

        sf::CircleShape *i0 = new sf::CircleShape();
        sf::CircleShape *i1 = new sf::CircleShape();
        sf::CircleShape *o0 = new sf::CircleShape();

        i0->setOrigin(this->connectorRadius, this->connectorRadius);
        i1->setOrigin(this->connectorRadius, this->connectorRadius);
        o0->setOrigin(this->connectorRadius, this->connectorRadius);

        i0->setRadius(this->connectorRadius);
        i1->setRadius(this->connectorRadius);
        o0->setRadius(this->connectorRadius);

        i0->setFillColor(sf::Color(50, 51, 44));
        i1->setFillColor(sf::Color(50, 51, 44));
        o0->setFillColor(sf::Color(50, 51, 44));
 
        i0->setPosition(this->sprite.getPosition());
        i1->setPosition(this->sprite.getPosition());
        o0->setPosition(this->sprite.getPosition());

        this->connectors.push_back(i0);
        this->connectors.push_back(i1);
        this->connectors.push_back(o0);
    }

    float x,y;
    Component *logicGate;
    sf::Text *label = new sf::Text;
    sf::Color spriteColor = sf::Color(54, 241, 205);
    std::string name;
    sf::Vector2f scale;
    sf::Texture *texture = new sf::Texture;
    sf::Sprite sprite;
    std::vector<sf::CircleShape*> connectors;
    short connectorRadius = 10;

    //TODO rework size & textsize
    void setPosition(float x, float y) {
        this->sprite.setPosition(x, y);
        this->x = x;
        this->y = y;

        float size = name.length()/5.0;
        float textSize = name.length() * 12.0;
        this->label->setPosition(x + (100*size - textSize)/2.0, y + 15 * this->sprite.getScale().y);

        for (short i=0; i<this->connectors.size()-1; ++i) {
            this->connectors[i]->setPosition(this->label->getPosition().x-this->connectorRadius/2, this->label->getPosition().y+i*2*this->connectorRadius);
        }
        this->connectors[2]->setPosition(this->label->getPosition().x + 80, this->label->getPosition().y + this->connectorRadius);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(this->sprite);
        for (auto connector: this->connectors) target.draw(*connector);
        target.draw(*this->label);
    }

    Gate* select() {
        this->sprite.setColor(sf::Color(57, 160, 237));
        return this;
    }

    Gate* deselect() {
        this->sprite.setColor(this->spriteColor);
        return nullptr;
    }

    void setBackendGate(Component *gate) {
        this->logicGate = gate;
    }
};

class Window {
    private: 
    void run() {
        sf::Event event;
        sf::Clock clock;
        float dt = 0.f;
        sf::Text fps;
        fps.setFont(font);
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

            if (fpsDebug) {
                fps.setString(std::to_string(1/dt));
                this->window->draw(fps);
            }
            this->window->display();
            dt = clock.restart().asSeconds();
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
    int tool = 0;
    sf::Vector2i selectedDp;
    sf::Vector2i mousePos;

    // -------------------
    std::vector<Gate*> gates;
    sf::RectangleShape deleteBoundsRect;

    // called on setup 
    void setup() {
        font.loadFromFile("fonts/FiraCode-Bold.ttf");

        this->gates.push_back(new Gate("AND", "img/gate.png", 100, 80));
        this->gates[0]->setBackendGate(new And("and1"));

        this->deleteBoundsRect.setFillColor(sf::Color(224, 17, 95));
        this->deleteBoundsRect.setSize(sf::Vector2f(SCREEN_WIDTH, 100)); 
        this->deleteBoundsRect.setPosition(0, SCREEN_HEIGHT-100); 
    }

    // called on mouse press
    void onMousePress(sf::Event::MouseButtonEvent buttonEvent) {
        sf::Mouse::Button button = buttonEvent.button;
        this->mouseDown = true;

        if (this->tool == 0) {
            for (int i=0; i<this->gates.size(); ++i) {
                if (gates[i]->sprite.getGlobalBounds().contains(this->mousePos.x, this->mousePos.y)) {
                    if (!this->selected) {
                        this->selected = gates[i]->select();
                        this->selectedDp = this->mousePos - sf::Vector2i(gates[i]->sprite.getPosition());
                    } else {
                        this->selected = gates[i]->deselect();
                    }
                }
            }
        } else {
            this->gates.push_back(new Gate("test"+std::to_string(this->tool), "img/gate.png", this->mousePos.x, this->mousePos.y));
        }
    }

    // called on mouse release
    void onMouseRelease(sf::Event::MouseButtonEvent buttonEvent) {
        this->mouseDown = false;
        if(this->selected) {
            if (this->mousePos.y > SCREEN_HEIGHT-100) {
                this->gates.erase(std::remove(this->gates.begin(), this->gates.end(), this->selected), this->gates.end());
                this->selected = nullptr;
                
            } else this->selected = this->selected->deselect();
        }
    }

    // called every frame
    void onUpdate() {
        this->mousePos = sf::Mouse::getPosition(*this->window);

        if (this->selected) {
            this->window->draw(this->deleteBoundsRect);

            if (this->mouseDown) {                
                this->selected->setPosition(mousePos.x - this->selectedDp.x, mousePos.y - this->selectedDp.y);
            }
        } 
        for (Gate *gate: gates) this->window->draw(*gate);
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
