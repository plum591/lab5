#pragma once
#include "Ball.hpp"
#include "Physics.h"
#include <vector>
// Добавили прямое подключение заголовка для std::string.
#include <string>

class Painter;

class World {
  public:
    World(const std::string& worldFilePath);
    void show(Painter& painter) const;
    void update(double time);

  private:
    Point topLeft;
    Point bottomRight;
    Physics physics;

    // Было: World хранил только вектор шаров balls.
    // Стало: временные частицы хранятся отдельно, в dust.
    std::vector<Ball> balls;
    std::vector<Dust> dust;

    // Остаток времени, которого пока не хватило на целый тик.
    double restTime = 0.;
};