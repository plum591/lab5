#include "Ball.hpp"

constexpr double pi = 3.141592653589793;

// Было: создать Ball из параметров одной записи файла было нельзя.
// Стало: конструктор сохраняет параметры и один раз вычисляет массу.
Ball::Ball(const Point &center, const Velocity &velocity, const Color &color,
           double radius, bool collidable)
    : m_center(center), m_velocity(velocity), m_color(color),
      m_radius(radius), m_mass(4.0 * pi * radius * radius * radius / 3.0),
      m_collidable(collidable)
{
}

// Было: setVelocity() был пустым, а getVelocity() возвращал {}.
// Стало: оба метода работают с реальной скоростью в m_velocity.
void Ball::setVelocity(const Velocity &velocity)
{
    m_velocity = velocity;
}

Velocity Ball::getVelocity() const
{
    return m_velocity;
}

// Было: в draw() стоял TODO и шар не рисовался.
// Стало: Painter получает центр, радиус и цвет конкретного шара.
void Ball::draw(Painter &painter) const
{
    painter.draw(m_center, m_radius, m_color);
}

// Было: setCenter() был пустым, а getCenter() возвращал {}.
// Стало: движок записывает новый центр в m_center и может прочитать его.
void Ball::setCenter(const Point &center)
{
    m_center = center;
}

Point Ball::getCenter() const
{
    return m_center;
}

// Было: getRadius() и getMass() возвращали {} — для double это 0.
// Стало: методы возвращают радиус из файла и вычисленную массу.
double Ball::getRadius() const
{
    return m_radius;
}

double Ball::getMass() const
{
    return m_mass;
}

// Было: у Ball не было метода чтения флага.
// Стало: возвращаем значение, сохранённое конструктором.
bool Ball::isCollidable() const
{
    return m_collidable;
}