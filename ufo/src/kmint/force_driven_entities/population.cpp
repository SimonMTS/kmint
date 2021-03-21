#include "kmint/force_driven_entities/population.hpp"

Population::Population(std::vector<kmint::ufo::human*>& h) : humans(h) {}

void Population::saveProperties() {
    for (int i = 0; i < humans.size(); i++) {
        humanproperties[i].Alignment = humans[i]->AlignmentWeight;
        humanproperties[i].Cohesion = humans[i]->CohesionWeight;
        humanproperties[i].Door = humans[i]->DoorWeight;
        humanproperties[i].Fitness = -1;
        humanproperties[i].GreenTank = humans[i]->GreenTankWeight;
        humanproperties[i].RedTank = humans[i]->RedTankWeight;
        humanproperties[i].Separation = humans[i]->SeparationWeight;
        humanproperties[i].Ufo = humans[i]->UfoWeight;
    }
}

void Population::Update() {
    for (unsigned long i = 0; i < humans.size(); i++) {
        if (humans[i]->removed()) {
            humans.erase(humans.begin() + i);
        }
    }
}

void Population::getParents() {
    std::cout << "Completing generation" << std::endl;
    float totalFitness = 0;
    for (int i = 0; i < 100; i++) {
        totalFitness += humanproperties[i].Fitness;
        std::cout << " Fitness of Human " << i << " is "
                  << humanproperties[i].Fitness << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < 100; i++) {
        auto p1 = getParentsFromFitness(totalFitness);
        auto p2 = getParentsFromFitness(totalFitness);
        HumanProperties parent1_properties = p1.first;
        HumanProperties parent2_properties = p2.first;
        int parent1_id = p1.second;
        int parent2_id = p2.second;

        std::cout << "Parents for Human " << i << " are " << parent1_id << " ("
                  << humanproperties[parent1_id].Fitness << ") and "
                  << parent2_id << " (" << humanproperties[parent2_id].Fitness
                  << ")" << std::endl;
        parents[i] = std::make_pair(parent1_properties, parent2_properties);
    }

    std::cout << std::endl;
}

void Population::Crossover() {
    for (int i = 0; i < 100; i++) {
        HumanProperties p1 = parents[i].first;
        HumanProperties p2 = parents[i].second;
        humans[i]->AlignmentWeight = p1.Alignment;
        humans[i]->CohesionWeight = p1.Cohesion;
        humans[i]->DoorWeight = p1.Door;
        humans[i]->GreenTankWeight = p1.GreenTank;
        humans[i]->RedTankWeight = p2.RedTank;
        humans[i]->SeparationWeight = p2.Separation;
        humans[i]->UfoWeight = p2.Ufo;
    }
}

void Population::Mutation() {
    std::cout << " Mutation" << std::endl;
    int MutationPercentage = 5;

    for (int i = 0; i < MutationPercentage; i++) {
        SetRandomMutation(i);
    }
    std::cout << std::endl;
}

void Population::SetRandomMutation(int i) {
    int GenToMutate = RandomNumber(0, 7);
    std::cout << "Human " << i;

    switch (GenToMutate) {
        case 0:
            std::cout << " Cohesion mutated from " << humans[i]->CohesionWeight
                      << " to ";
            humans[i]->CohesionWeight = RandomNumber(0, 1);
            std::cout << humans[i]->CohesionWeight << std::endl;
            break;
        case 1:
            std::cout << " Separation mutated from "
                      << humans[i]->SeparationWeight << " to ";
            humans[i]->SeparationWeight = RandomNumber(0, 1);
            std::cout << humans[i]->SeparationWeight << std::endl;

            break;
        case 2:
            std::cout << " Alignment mutated from "
                      << humans[i]->AlignmentWeight << " to ";
            humans[i]->AlignmentWeight = RandomNumber(0, 1);
            std::cout << humans[i]->AlignmentWeight << std::endl;

            break;
        case 3:
            std::cout << " RedTank mutated from " << humans[i]->RedTankWeight
                      << " to ";
            humans[i]->RedTankWeight = RandomNumber(-1, 1);
            std::cout << humans[i]->RedTankWeight << std::endl;

            break;
        case 4:
            std::cout << " GreenTank mutated from "
                      << humans[i]->GreenTankWeight << " to ";
            humans[i]->GreenTankWeight = RandomNumber(-1, 1);
            std::cout << humans[i]->GreenTankWeight << std::endl;

            break;
        case 5:
            std::cout << " Ufo mutated from " << humans[i]->UfoWeight << " to ";
            humans[i]->UfoWeight = RandomNumber(-1, 1);
            std::cout << humans[i]->UfoWeight << std::endl;

            break;
        case 6:
            std::cout << " Door mutated from " << humans[i]->UfoWeight
                      << " to ";
            humans[i]->DoorWeight = RandomNumber(-1, 1);
            std::cout << humans[i]->DoorWeight << std::endl;

            break;
    }
}

std::pair<HumanProperties, int> Population::getParentsFromFitness(float total) {
    float p1 = RandomNumber(0, total);

    for (int i = 0; i < 100; i++) {
        p1 -= humanproperties[i].Fitness;
        if (p1 <= 0) {
            return std::make_pair(humanproperties[i], i);
        }
    }

    return std::make_pair(HumanProperties{0, 0, 0, 0, 0, 0, 0, 0}, 0);
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
