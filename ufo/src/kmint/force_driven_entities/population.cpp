#include "kmint/force_driven_entities/population.hpp"

Population::Population(std::vector<kmint::ufo::human*>& h) : humans(h) {}

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

void Population::increaseCohesion() {
    if (CohesionWeight <= 0.9f)
        CohesionWeight += 0.1f;
    else
        CohesionWeight = 1;
    SetValues();
};
void Population::increaseSeparation() {
    if (SeparationWeight <= 0.9f)
        SeparationWeight += 0.1f;
    else
        SeparationWeight = 1;
    SetValues();
};
void Population::increaseAlignment() {
    if (AlignmentWeight <= 0.9f)
        AlignmentWeight += 0.1f;
    else
        AlignmentWeight = 1;
    SetValues();
};

void Population::increaseTank() {
    if (GreenTankWeight <= 0.9f)
        GreenTankWeight += 0.1f;
    else
        GreenTankWeight = 1;

    if (RedTankWeight <= 0.9f)
        RedTankWeight += 0.1f;
    else
        RedTankWeight = 1;
    SetValues();
};

void Population::increaseUfo() {
    if (UfoWeight <= 0.9f)
        UfoWeight += 0.1f;
    else
        UfoWeight = 1;
    SetValues();
};

void Population::decreaseCohesion() {
    if (CohesionWeight >= 0.1f)
        CohesionWeight -= 0.1f;
    else
        CohesionWeight = 0;
    SetValues();
};
void Population::decreaseSeparation() {
    if (SeparationWeight >= 0.1f)
        SeparationWeight -= 0.1f;
    else
        SeparationWeight = 0;
    SetValues();
};
void Population::decreaseAlignment() {
    if (AlignmentWeight >= 0.1f)
        AlignmentWeight -= 0.1f;
    else
        AlignmentWeight = 0;
    SetValues();
};

void Population::decreaseTank() {
    if (GreenTankWeight >= -0.9f)
        GreenTankWeight -= 0.1f;
    else
        GreenTankWeight = -1.f;

    if (RedTankWeight >= -0.9f)
        RedTankWeight -= 0.1f;
    else
        RedTankWeight = -1.f;
    SetValues();
};

void Population::decreaseUfo() {
    if (UfoWeight >= -0.9f)
        UfoWeight -= 0.1f;
    else
        UfoWeight = -1.f;
    SetValues();
};

void Population::SetValues() {
    for (int i = 0; i < humans.size(); i++) {
        humans[i]->AlignmentWeight = this->AlignmentWeight;
        humans[i]->CohesionWeight = this->CohesionWeight;
        humans[i]->SeparationWeight = this->SeparationWeight;
        humans[i]->RedTankWeight = this->RedTankWeight;
        humans[i]->GreenTankWeight = this->GreenTankWeight;

        humans[i]->UfoWeight = this->UfoWeight;
    }

    std::cout << "SetValues A\t" << AlignmentWeight << "\tC\t" << CohesionWeight
              << "\tS\t" << SeparationWeight << "\tT\t" << RedTankWeight
              << "\tU\t" << UfoWeight << std::endl;
}
