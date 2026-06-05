#pragma once
#include <SFML/Graphics.hpp>





struct Schiebregler {
    bool isDragging = false;
    sf::Vector2f position;
    sf::CircleShape handle;
    sf::RectangleShape track;
    float value = 0.0f; // 0.0 to 1.0
    float min = 0.0f;
    float max = 1.0f;
    sf::Color trackColor = sf::Color::White;
    sf::Color handleColor = sf::Color::White;
    float off = 0.0f; 

    Schiebregler(float x, float y, float width, float height) {
        position = sf::Vector2f(x, y);
        handle.setRadius(width/ 2);
        handle.setOrigin(width / 2, width / 2);
        handle.setFillColor(handleColor);
        handle.setPosition(position);
        track.setOrigin(width / 6, 0);
        track.setPosition(position);
        track.setSize(sf::Vector2f(width/3, height));
        track.setFillColor(trackColor);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(track);
        window.draw(handle);

    }
    bool inCircle(sf::Vector2i point) {
        return (point.x - handle.getPosition().x) * (point.x - handle.getPosition().x) +
               (point.y - handle.getPosition().y) * (point.y - handle.getPosition().y) <=
               handle.getRadius() * handle.getRadius();
    }
    float getValue() {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    void dragging(sf::Vector2i mouse) {
        handle.setPosition(handle.getPosition().x, mouse.y + off);
        if (mouse.y <= track.getPosition().y) handle.setPosition(handle.getPosition().x,position.y);
        if (mouse.y >= track.getPosition().y + track.getSize().y) handle.setPosition(handle.getPosition().x,position.y + track.getSize().y);
        value = (mouse.y - track.getPosition().y) / track.getSize().y;
    }


};