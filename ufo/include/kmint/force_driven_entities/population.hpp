#pragma once
#include <vector>
#include "kmint/ufo/human.hpp"

class Population {
   public:
    Population();
    std::vector<kmint::ufo::human *> &humans = std::vector<kmint::ufo::human *>();
    void setPopulation(std::vector<kmint::ufo::human *> humans);
    void Update();
};