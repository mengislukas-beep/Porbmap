#include "board.h"
#include <random>
#include <SFML/Graphics.hpp>

Board::Board(int width, int height, float cellSize)
    : m_width(width),
      m_height(height),
      m_cellSize(cellSize),
      m_vertices(sf::Quads, width * height * 4)
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int i = (y * m_width + x) * 4;

            float px = x * m_cellSize;
            float py = y * m_cellSize;

            m_vertices[i + 0].position = {px, py};
            m_vertices[i + 1].position = {px + m_cellSize, py};
            m_vertices[i + 2].position = {px + m_cellSize, py + m_cellSize};
            m_vertices[i + 3].position = {px, py + m_cellSize};

            setColor(x, y, sf::Color::White);
        }
    }
}

void Board::setColor(int x, int y, sf::Color color)
{
    int i = (y * m_width + x) * 4;

    m_vertices[i + 0].color = color;
    m_vertices[i + 1].color = color;
    m_vertices[i + 2].color = color;
    m_vertices[i + 3].color = color;
}

void Board::draw(sf::RenderWindow& window)
{
    window.draw(m_vertices);
}


GridLines::GridLines(int width, int height)
{
    values.resize(width * height);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0; i < width * height; i++)
    {
        values[i] = dist(gen);
    }
}

bool GridLines::connected_right(sf::Vector2i pos, float threshold)
{
    return values[pos.y * m_width + pos.x + 1] > threshold;   
}

bool GridLines::connected_down(sf::Vector2i pos, float threshold)
{
    return values[(pos.y + 1) * m_width + pos.x] > threshold;   
}