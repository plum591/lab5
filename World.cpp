#include "World.h"

#include <cmath>
#include <fstream>
#include <stdexcept>

static constexpr double timePerTick = 0.001;

std::istream &operator>>(std::istream &stream, Point &point)
{
    return stream >> point.x >> point.y;
}

std::istream &operator>>(std::istream &stream, Color &color)
{
    double red, green, blue;
    if (stream >> red >> green >> blue)
    {
        color = Color(red, green, blue);
    }
    return stream;
}

World::World(const std::string &worldFilePath)
    : physics(timePerTick)
{
    std::ifstream stream(worldFilePath);
    if (!stream)
    {
        throw std::runtime_error("Cannot open model: " + worldFilePath);
    }

    if (!(stream >> topLeft >> bottomRight) ||
        topLeft.x >= bottomRight.x || topLeft.y >= bottomRight.y)
    {
        throw std::runtime_error("Invalid world bounds");
    }
    physics.setWorldBox(topLeft, bottomRight);

    stream >> std::boolalpha;
    while (true)
    {
        stream >> std::ws;
        if (stream.eof())
        {
            break;
        }

        Point center, velocity;
        Color color;
        double radius;
        bool collidable;
        if (!(stream >> center >> velocity >> color >> radius >> collidable) ||
            radius <= 0.0)
        {
            throw std::runtime_error("Invalid ball in model");
        }

        balls.emplace_back(center, Velocity(velocity), color, radius, collidable);
    }
}

void World::show(Painter &painter) const
{
    painter.draw(topLeft, bottomRight, Color(1.0, 1.0, 1.0));

    for (const Ball &ball : balls)
    {
        ball.draw(painter);
    }
    for (const Dust &particle : dust)
    {
        particle.draw(painter);
    }
}

void World::update(double time)
{
    time += restTime;
    const auto ticks = static_cast<std::size_t>(std::floor(time / timePerTick));
    restTime = time - ticks * timePerTick;
    physics.update(balls, dust, ticks);
}