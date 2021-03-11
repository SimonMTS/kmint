#include "kmint/force_driven_entities/population.hpp"

Population::Population() {

}

void Population::setPopulation(std::vector<kmint::ufo::human*> humans) {
    this->humans = humans;
}

void Population::Update() {
    for (int i = 0; i < humans.size(); i++) {
        if (humans[i]->removed()) {
            humans.erase(humans.begin() + i);
        }
    }
}