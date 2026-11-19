#pragma once

#include "Painter.h"

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