#include "Physics.h"
#include <algorithm>
#include <cmath>
#include <iterator>

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

void Physics::update(std::vector<Ball> &balls, std::vector<Dust> &dust,
                     std::size_t ticks) const
{
    for (std::size_t i = 0; i < ticks; ++i)
    {
        // Было: тик начинался сразу с move(balls).
        // Стало: сначала перемещаем частицы и уменьшаем время их жизни.
        for (Dust &particle : dust)
        {
            particle.update(timePerTick);
        }

        // Было: частиц для удаления не существовало.
        // Стало: убираем из dust частицы, у которых закончилось время жизни.
        dust.erase(std::remove_if(dust.begin(), dust.end(), [](const Dust &particle)
        {
            return !particle.isAlive();
        }), dust.end());

        move(balls);
        collideWithBox(balls);

        // Было: collideBalls(balls).
        // Стало: передаём dust, чтобы столкновения могли создавать частицы.
        collideBalls(balls, dust);
    }
}

void Physics::collideBalls(std::vector<Ball> &balls, std::vector<Dust> &dust) const
{
    for (auto a = balls.begin(); a != balls.end(); ++a)
    {
        // Было: перебирались все пары шаров.
        // Стало: при false у первого шара пропускаем все пары с ним.
        if (!a->isCollidable())
        {
            continue;
        }
        for (auto b = std::next(a); b != balls.end(); ++b)
        {
            // Теперь пропускаем и пары, где false у второго шара.
            if (!b->isCollidable())
            {
                continue;
            }

            const double distance = distance2(a->getCenter(), b->getCenter());
            const double limit = a->getRadius() + b->getRadius();

            // Было: проверялось только пересечение, поэтому distance мог быть 0.
            // Стало: требуем distance > 0, чтобы при вычислении нормали
            // не делить вектор на нулевую длину.
            if (distance > 0.0 && distance < limit * limit)
            {
                processCollision(*a, *b, distance, dust);
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        // Было: отскок от стен проверялся у каждого шара.
        // Стало: при false пропускаем проверку, и шар проходит сквозь стенку.
        if (!ball.isCollidable())
        {
            continue;
        }

        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        // Проверяем, вышла ли координата за пределы допустимого диапазона.
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
            Point vector = ball.getVelocity().vector();
            vector.x = -vector.x;
            ball.setVelocity(vector);
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
            Point vector = ball.getVelocity().vector();
            vector.y = -vector.y;
            ball.setVelocity(vector);
        }
    }
}

void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }
}

void Physics::processCollision(Ball& a, Ball& b,
                               double distanceBetweenCenters2,
                               std::vector<Dust>& dust) const {
    // нормированный вектор столкновения
    const Point normal =
        (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    // получаем скорость в векторном виде
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    // коэффициент p учитывает скорость обоих мячей
    const double p =
        2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    // задаем новые скорости мячей после столкновения
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));

    // Было: метод заканчивался после изменения скоростей шаров.
    // Стало: после расчёта отскока добавляем 8 частиц в точке контакта.
    const Point contact = a.getCenter() + normal * a.getRadius();
    constexpr int particleCount = 8;
    constexpr double fullTurn = 6.283185307179586;
    constexpr double speed = 180.0;

    for (int i = 0; i < particleCount; ++i)
    {
        // Каждой новой частице задаём своё направление, с шагом 45 градусов.
        const double angle = fullTurn * i / particleCount;
        const Point velocity(std::cos(angle) * speed, std::sin(angle) * speed);
        dust.emplace_back(contact, velocity);
    }
}