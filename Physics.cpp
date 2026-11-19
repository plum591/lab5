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
        for (Dust &particle : dust)
        {
            particle.update(timePerTick);
        }
        dust.erase(std::remove_if(dust.begin(), dust.end(), [](const Dust &particle)
        {
            return !particle.isAlive();
        }), dust.end());

        move(balls);
        collideWithBox(balls);
        collideBalls(balls, dust);
    }
}

void Physics::collideBalls(std::vector<Ball> &balls, std::vector<Dust> &dust) const
{
    for (auto a = balls.begin(); a != balls.end(); ++a)
    {
        if (!a->isCollidable())
        {
            continue;
        }
        for (auto b = std::next(a); b != balls.end(); ++b)
        {
            if (!b->isCollidable())
            {
                continue;
            }

            const double distance = distance2(a->getCenter(), b->getCenter());
            const double limit = a->getRadius() + b->getRadius();
            if (distance > 0.0 && distance < limit * limit)
            {
                processCollision(*a, *b, distance, dust);
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {  
        if (!ball.isCollidable())
        {
            continue;
        }

        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        // определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
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

    const Point contact = a.getCenter() + normal * a.getRadius();
    constexpr int particleCount = 8;
    constexpr double fullTurn = 6.283185307179586;
    constexpr double speed = 180.0;

    for (int i = 0; i < particleCount; ++i)
    {
        const double angle = fullTurn * i / particleCount;
        const Point velocity(std::cos(angle) * speed, std::sin(angle) * speed);
        dust.emplace_back(contact, velocity);
    }
}
