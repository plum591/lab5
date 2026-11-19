#include "World.h"

#include <cmath>
#include <fstream>
#include <stdexcept>

static constexpr double timePerTick = 0.001;

// Было: координаты каждой точки читались по отдельности в конструкторе.
// Стало: выражение stream >> point само читает x и y.
std::istream &operator>>(std::istream &stream, Point &point)
{
    return stream >> point.x >> point.y;
}

// Было: red, green и blue читались отдельно в конструкторе World.
// Стало: stream >> color читает три компоненты и создаёт из них Color.
std::istream &operator>>(std::istream &stream, Color &color)
{
    double red, green, blue;
    if (stream >> red >> green >> blue)
    {
        color = Color(red, green, blue);
    }
    return stream;
}

// Было: Physics создавался с шагом по умолчанию.
// Стало: явно передаём шаг, по которому World считает тики.
World::World(const std::string &worldFilePath)
    : physics(timePerTick)
{
    std::ifstream stream(worldFilePath);

    // Было: открытие файла не проверялось.
    // Стало: при ошибке передаём сообщение обработчику в main().
    if (!stream)
    {
        throw std::runtime_error("Cannot open model: " + worldFilePath);
    }

    // Было: четыре числа читались отдельно, границы не проверялись.
    // Стало: читаем две точки и проверяем размеры прямоугольника.
    if (!(stream >> topLeft >> bottomRight) ||
        topLeft.x >= bottomRight.x || topLeft.y >= bottomRight.y)
    {
        throw std::runtime_error("Invalid world bounds");
    }
    physics.setWorldBox(topLeft, bottomRight);

    stream >> std::boolalpha;

    // Было: while (stream.peek(), stream.good()) проверял состояние потока
    // до чтения записи, а результат чтения самой записи не проверялся.
    // Стало: сначала пропускаем пробелы и проверяем конец файла,
    // затем проверяем, что удалось прочитать всю запись.
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

        // Было: после чтения параметров стоял TODO, а balls оставался пустым.
        // Стало: из каждой записи создаём Ball прямо в векторе.
        // Velocity получает готовые компоненты (vx, vy) через Point.
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

    // Было: show() рисовал область и шары.
    // Стало: после шаров рисует ещё и частицы из dust.
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

    // Было: physics.update(balls, ticks).
    // Стало: передаём ещё и dust, чтобы движок обновлял частицы.
    physics.update(balls, dust, ticks);
}