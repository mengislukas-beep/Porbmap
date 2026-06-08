#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Schiebregler.h"
#include "board.h"
#include <iostream>

int main() {
    sf::RenderWindow window(
        sf::VideoMode(1200, 1000),
        "Probmap"
    );
    sf::Font font;
    if (!font.loadFromFile("font/Inter-VariableFont_opsz,wght.ttf")) {
        std::cout << "Fehler beim Laden der Schriftart\n";
        return -1;
    }

    int lx = 1000;
    int ly = 1000;
    float cellSize = 4.f;




    int width = lx / cellSize;
    int height = ly / cellSize;

    Schiebregler slider(1100.f, 100.f, 20.f, 200.f);
    GridLines grid(width, height);
    float threshold = slider.getValue();
    threshold = 0.5f;
    std::vector<int> regions = grid.createRegions(threshold);
    regions = createMax4ConnectedRegions(regions, width, height);

    Board board(width, height, cellSize);
    board.setColors(regions);

    board.setColors(regions);
    


    
    
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
            if (event.key.code == sf::Keyboard::Up) {
                
                slider.handle.setPosition(slider.handle.getPosition().x, slider.position.y +(1-slider.getValue()) * slider.track.getSize().y);
                threshold = slider.getValue();
                regions = grid.createRegions(threshold);
                regions = createMax4ConnectedRegions(regions, width, height);
                board.setColors(regions);

            }
            if (event.key.code == sf::Keyboard::Down) {
                
                slider.handle.setPosition(slider.handle.getPosition().x, slider.position.y +(slider.getValue()) * slider.track.getSize().y);
                
                threshold = slider.getValue();
                regions = grid.createRegions(threshold);
                regions = createMax4ConnectedRegions(regions, width, height);
                board.setColors(regions);
            }
            }
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        if (slider.isDragging) {
            slider.dragging(mouse);
            threshold = slider.getValue();
            regions = grid.createRegions(threshold);
            regions = createMax4ConnectedRegions(regions, width, height);
            board.setColors(regions);

        }
        window.clear();
        slider.draw(window, font);
        board.draw(window);
        window.display();


    }
    return 0;
}