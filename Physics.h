#pragma once
#include "Ball.hpp"
#include "Dust.hpp"
#include <vector>
#include <cstddef>

class Physics {
  public:
    Physics(double timePerTick = 0.001);
    void setWorldBox(const Point& topLeft, const Point& bottomRight);

    // Было: update(balls, ticks).
    // Стало: update(balls, dust, ticks) получает и вектор частиц.
    void update(std::vector<Ball>& balls, std::vector<Dust>& dust, std::size_t ticks) const;

  private:
    // Было: collideBalls(balls) и processCollision(a, b, distance).
    // Стало: оба метода принимают dust по ссылке, чтобы обработчик
    // столкновения мог добавить частицы в вектор, принадлежащий World.
    void collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dust) const;
    void collideWithBox(std::vector<Ball>& balls) const;
    void move(std::vector<Ball>& balls) const;
    void processCollision(Ball& a, Ball& b,
                          double distanceBetweenCenters2,
                          std::vector<Dust>& dust) const;

  private:
    Point topLeft;
    Point bottomRight;
    double timePerTick;
};