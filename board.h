#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>


class Board
{
public:
    Board(int width, int height, float cellSize);

    void setColor(int x, int y, sf::Color color);
    void draw(sf::RenderWindow& window);

private:
    int m_width;
    int m_height;
    float m_cellSize;

    sf::VertexArray m_vertices;
};

class GridLines
{
public:
    GridLines(int width, int height);
    bool connected_right(sf::Vector2i a,float threshold);
    bool connected_down(sf::Vector2i a,float threshold);
private:
    std::vector<float> values;
    int m_width;
    int m_height;
};