#include "Dust.hpp"

Dust::Dust(const Point &center, const Point &velocity)
    : m_center(center), m_velocity(velocity)
{
}

void Dust::update(double time)
{
    m_center = m_center + m_velocity * time;
    m_time_left -= time;
}

bool Dust::isAlive() const
{
    return m_time_left > 0.0;
}

void Dust::draw(Painter &painter) const
{
    painter.draw(m_center, 10.0, Color(1.0, 0.75, 0.2));
}