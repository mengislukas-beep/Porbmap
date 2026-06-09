#include "board.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <set>
#include <vector>


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

void Board::setColors(const std::vector<int>& regions) {
    for (int i = 0; i < static_cast<int>(regions.size()); i++) {
        int x = i % m_width;
        int y = i / m_width;
        if (regions[i] ==  0) 
        {
            setColor(x, y, sf::Color::Red);
        }
        if (regions[i] ==  1) 
        {
            setColor(x, y, sf::Color::Green);
        }
        if (regions[i] ==  2) 
        {
            setColor(x, y, sf::Color::Blue);
        }
        if (regions[i] ==  3) 
        {
            setColor(x, y, sf::Color::Yellow);
        }
    }
}


GridLines::GridLines(int width, int height)
    : m_width(width), m_height(height)
{
    values.resize(width * height);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0; i < width * height; i++)
        values[i] = dist(gen);
}

bool GridLines::connected_right(sf::Vector2i pos, float threshold)
{
    return values[pos.y * m_width + pos.x + 1] > threshold;   
}

bool GridLines::connected_down(sf::Vector2i pos, float threshold)
{
    return values[(pos.y + 1) * m_width + pos.x] > threshold;   
}
std::vector<int> GridLines::createRegions(float threshold)
{
    std::vector<int> regions(m_width * m_height, -1);

    int currentRegion = 0;

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int startIndex = y * m_width + x;

            if (regions[startIndex] != -1)
                continue;

            std::vector<sf::Vector2i> stack;
            stack.push_back({x, y});
            regions[startIndex] = currentRegion;

            while (!stack.empty())
            {
                sf::Vector2i pos = stack.back();
                stack.pop_back();

                int px = pos.x;
                int py = pos.y;

                // rechts
                if (px + 1 < m_width &&
                    connected_right(pos, threshold))
                {
                    int ni = py * m_width + (px + 1);

                    if (regions[ni] == -1)
                    {
                        regions[ni] = currentRegion;
                        stack.push_back({px + 1, py});
                    }
                }

                // links
                if (px - 1 >= 0 &&
                    connected_right({px - 1, py}, threshold))
                {
                    int ni = py * m_width + (px - 1);

                    if (regions[ni] == -1)
                    {
                        regions[ni] = currentRegion;
                        stack.push_back({px - 1, py});
                    }
                }

                // unten
                if (py + 1 < m_height &&
                    connected_down(pos, threshold))
                {
                    int ni = (py + 1) * m_width + px;

                    if (regions[ni] == -1)
                    {
                        regions[ni] = currentRegion;
                        stack.push_back({px, py + 1});
                    }
                }

                // oben
                if (py - 1 >= 0 &&
                    connected_down({px, py - 1}, threshold))
                {
                    int ni = (py - 1) * m_width + px;

                    if (regions[ni] == -1)
                    {
                        regions[ni] = currentRegion;
                        stack.push_back({px, py - 1});
                    }
                }
            }

            currentRegion++;
        }
    }

    return regions;
}



std::vector<int> createMax4ConnectedRegions(
    const std::vector<int>& regions,
    int width,
    int height
)
{
    int cellCount = width * height;

    int regionCount = 0;
    for (int id : regions)
    {
        if (id + 1 > regionCount)
            regionCount = id + 1;
    }

    std::vector<std::set<int>> neighbours(regionCount);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int current = regions[y * width + x];

            if (x + 1 < width)
            {
                int right = regions[y * width + x + 1];

                if (current != right)
                {
                    neighbours[current].insert(right);
                    neighbours[right].insert(current);
                }
            }

            if (y + 1 < height)
            {
                int down = regions[(y + 1) * width + x];

                if (current != down)
                {
                    neighbours[current].insert(down);
                    neighbours[down].insert(current);
                }
            }
        }
    }

    std::vector<int> regionColors(regionCount, -1);

    for (int region = 0; region < regionCount; region++)
    {
        std::array<bool, 4> used = {false, false, false, false};

        for (int neighbour : neighbours[region])
        {
            int color = regionColors[neighbour];

            if (color != -1)
                used[color] = true;
        }

        for (int color = 0; color < 4; color++)
        {
            if (!used[color])
            {
                regionColors[region] = color;
                break;
            }
        }
    }

    std::vector<int> result(cellCount);

    for (int i = 0; i < cellCount; i++)
    {
        int region = regions[i];
        result[i] = regionColors[region];
    }

    return result;
}