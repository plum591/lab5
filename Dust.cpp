#include "Dust.hpp"

// Новый файл. В заготовке столкновение меняло только скорости шаров.
// Теперь у эффекта есть объекты с начальной точкой и скоростью.
Dust::Dust(const Point &center, const Point &velocity)
    : m_center(center), m_velocity(velocity)
{
}

// За тик частица смещается на скорость * время и теряет часть срока жизни.
void Dust::update(double time)
{
    m_center = m_center + m_velocity * time;
    m_time_left -= time;
}

// Physics использует этот метод, чтобы решить, пора ли удалить частицу.
bool Dust::isAlive() const
{
    return m_time_left > 0.0;
}

// World вызывает этот метод при рисовании кадра.
void Dust::draw(Painter &painter) const
{
    painter.draw(m_center, 10.0, Color(1.0, 0.75, 0.2));
}