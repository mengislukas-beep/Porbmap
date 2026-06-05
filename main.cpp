#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Schiebregler.h"
#include "board.h"
int main() {
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "Probmap"
    );
    Schiebregler slider(700.f, 100.f, 20.f, 200.f);
    Board board(20, 15, 32.f);

    board.setColor(0, 0, sf::Color::Red);
    board.setColor(1, 0, sf::Color::Green);
    board.setColor(2, 0, sf::Color::Blue);
    board.setColor(5, 5, sf::Color::Yellow);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent( event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            sf::Vector2i mouse = sf::Mouse::getPosition(window);
            if (event.type == sf::Event::MouseButtonPressed 
                && event.mouseButton.button == sf::Mouse::Left && slider.inCircle(mouse)) {
                slider.isDragging = true;

                slider.off  = slider.handle.getPosition().y- mouse.y;


            }
            if (event.type == sf::Event::MouseButtonReleased 
                && event.mouseButton.button == sf::Mouse::Left) {
                slider.isDragging = false;
            }
        }
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        if (slider.isDragging) {
            slider.dragging(mouse);

        }
        window.clear();
        slider.draw(window);
        board.draw(window);
        window.display();


    }
    return 0;
}