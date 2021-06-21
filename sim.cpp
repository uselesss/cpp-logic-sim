#include <iostream>
#include <SFML/Graphics.hpp>
#include "logic.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct connectorCircle {
    Connector connector;
    sf::CircleShape circle;
    int x, y;
};

struct Gate {
    Component component;
    sf::Sprite sprite;
    int x, y;
};

std::vector<Gate> gates;
std::vector<connectorCircle> connectors;
std::vector<sf::Vertex[2]> connectionLines;

int curTool = 0;
connectorCircle selected;

int main()
{
    //Nothing is selected by default
    selected.connector.name = "None";

    //Connector circle
    sf::CircleShape c(10);

    //Load textures
    sf::Texture texture;
    if (!texture.loadFromFile("img/and.png")) {
        std::cout << "Could not load a texture" << std::endl; 
        return 0;
    }

    //Create sprites
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.125, 0.125);

    //Window creation 
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Logic gates are cool...");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                curTool = event.key.code;
                std::cout << event.key.code << std::endl;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (curTool == 27) {
                        std::string name = "connectorId" + std::to_string(connectors.size());
                        Connector connector(name);
                        connectorCircle con0 = {connector, c, event.mouseButton.x, event.mouseButton.y};   
                        connectors.push_back(con0);
                    }

                    if (curTool == 28) {
                        std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
                        // sprite.setPosition(event.mouseButton.x, event.mouseButton.y);
                        
                        std::string name0 = "connectorId" + std::to_string(connectors.size());
                        Connector connector(name0);
                        connectorCircle con0 = {connector, c, event.mouseButton.x, event.mouseButton.y};
                        connectors.push_back(con0);
                        
                        std::string name1 = "connectorId" + std::to_string(connectors.size());
                        Connector connector1(name1);
                        connectorCircle con1 = {connector1, c, event.mouseButton.x, event.mouseButton.y+25};
                        connectors.push_back(con1);
                        
                        std::string name2 = "connectorId" + std::to_string(connectors.size());
                        Connector connector2(name2);
                        connectorCircle con2 = {connector2, c, event.mouseButton.x+60, event.mouseButton.y+13};
                        connectors.push_back(con2);
                        

                        std::string name = "andGateId" + std::to_string(gates.size());
                        And andGate(name);
                        Connector i0, i1, o0;
                        i0.parent = &andGate;
                        i1.parent = &andGate;

                        Gate gate = {andGate, sprite, event.mouseButton.x, event.mouseButton.y};

                        gates.push_back(gate);     
                    }  

                    //not
                    if (curTool == 29) {
                        std::string name0 = "connectorId" + std::to_string(connectors.size());
                        Connector connector(name0);
                        connectorCircle con0 = {connector, c, event.mouseButton.x, event.mouseButton.y};
                        connectors.push_back(con0);

                        std::string name1 = "connectorId" + std::to_string(connectors.size());
                        Connector connector1(name1);
                        connectorCircle con1 = {connector1, c, event.mouseButton.x, event.mouseButton.y+25};
                        connectors.push_back(con1);

                        std::string name = "notGateId" + std::to_string(gates.size());
                        Not notGate(name);
                        Connector i0, o0;
                        i0.parent = &notGate;

                    }


                    //wire
                    if (curTool == 30) {
                        
                        for (int i=0; i<connectors.size(); ++i) {
                            if (abs(connectors[i].x - event.mouseButton.x + 10) <= 10 && abs(connectors[i].y - event.mouseButton.y + 10) <= 10) {
                                
                                if (selected.connector.name == "None") {
                                    selected = connectors[i]; 
                                    connectors[i].circle.setFillColor(sf::Color::Yellow);
                                } else {
                                    selected.circle.setFillColor(sf::Color::White);
                                    
                                    connectionLines.push_back(sf::Vertex(sf::Vector2f(selected.x, selected.y), sf::Vector2f(connectors[i].x, connectors[i].y)));

                                    connectorCircle none;
                                    none.connector.name = "None";
                                    selected = none;
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(120, 120, 120));  

        //Draw all the stuff
        for (auto vtx: connectionLines) {
            window.draw(vtx, 2, sf::Lines);
        }

        for (Gate gate: gates) {
            gate.sprite.setPosition(gate.x, gate.y);
            window.draw(gate.sprite);
        }

        for (connectorCircle connector: connectors) {
            connector.circle.setPosition(connector.x, connector.y);
            // connector->circle->setFillColor(sf::Color(255, 0, 0));
            if (connector.connector.value == 1) connector.circle.setFillColor(sf::Color::Green);
            else window.draw(connector.circle);
        }

        window.display();
    }

    return 0;
}

