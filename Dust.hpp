#pragma once

#include "Painter.h"

// Было: в заготовке не было класса частицы.
// Стало: Dust хранит центр, скорость и оставшееся время жизни частицы.
class Dust
{
public:
    Dust(const Point &center, const Point &velocity);
    void update(double time);
    bool isAlive() const;
    void draw(Painter &painter) const;

private:
    Point m_center;
    Point m_velocity;
    double m_time_left = 0.25;
};