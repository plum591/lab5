#include "Ball.hpp"

constexpr double pi = 3.141592653589793;

Ball::Ball(const Point &center, const Velocity &velocity, const Color &color,
           double radius, bool collidable)
    : m_center(center), m_velocity(velocity), m_color(color),
      m_radius(radius), m_mass(4.0 * pi * radius * radius * radius / 3.0),
      m_collidable(collidable)
{
}

void Ball::setVelocity(const Velocity &velocity)
{
    m_velocity = velocity;
}

Velocity Ball::getVelocity() const
{
    return m_velocity;
}

void Ball::draw(Painter &painter) const
{
    painter.draw(m_center, m_radius, m_color);
}

void Ball::setCenter(const Point &center)
{
    m_center = center;
}

Point Ball::getCenter() const
{
    return m_center;
}

double Ball::getRadius() const
{
    return m_radius;
}

double Ball::getMass() const
{
    return m_mass;
}

bool Ball::isCollidable() const
{
    return m_collidable;
}