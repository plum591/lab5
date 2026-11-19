#pragma once

#include "Painter.h"
#include "Velocity.h"

class Ball
{
public:
    // Было: у Ball не было конструктора с параметрами модели.
    // Стало: один вызов конструктора задаёт центр, скорость, цвет, радиус и флаг.
    Ball(const Point &center, const Velocity &velocity, const Color &color,
         double radius, bool collidable);

    void setVelocity(const Velocity &velocity);
    Velocity getVelocity() const;
    void draw(Painter &painter) const;
    void setCenter(const Point &center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;

    // Было: движок не мог спросить у шара, включены ли столкновения.
    // Стало: он получает значение флага через isCollidable().
    bool isCollidable() const;

private:
    // Было: класс объявлял методы, но полей для данных шара не было.
    // Стало: каждый Ball хранит свои данные. Радиус и масса постоянны.
    Point m_center;
    Velocity m_velocity;
    Color m_color;
    const double m_radius;
    const double m_mass;
    bool m_collidable;
};