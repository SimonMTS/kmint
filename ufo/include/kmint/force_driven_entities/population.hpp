#pragma once
#include <vector>
#include <array>
#include <tuple>
#include "kmint/ufo/human.hpp"

struct HumanProperties {
    float Fitness;
    float Cohesion;
    float Separation;
    float Alignment;
    float RedTank;
    float GreenTank;
    float Ufo;
    float Door;
};


class Population {
   public:
    Population(std::vector<kmint::ufo::human *> &h);
    std::vector<kmint::ufo::human *> &humans;
    std::array<HumanProperties, 100> humanproperties;
    std::array<std::pair<HumanProperties, HumanProperties>, 100> parents;
    void saveProperties();
    void getParents();
    void Crossover();
    void Mutation();
    void Update();

    void increaseCohesion();
    void increaseSeparation();
    void increaseAlignment();
    void increaseTank();
    void increaseUfo();

    void decreaseCohesion();
    void decreaseSeparation();
    void decreaseAlignment();
    void decreaseTank();
    void decreaseUfo();

    void SetValues();

   private:
    float CohesionWeight = 0.5;    //    0 tot 1
    float SeparationWeight = 0.5;  //    0 tot 1
    float AlignmentWeight = 0.5;   //	   0 tot 1
    float RedTankWeight = 0;       //   -1 tot 1;
    float GreenTankWeight = 0;     //   -1 tot 1;
    float UfoWeight = 0;           //   -1 tot 1;
    float DoorWeight = 0;          //   -1 tot 1;

    std::pair<HumanProperties, int> getParentsFromFitness(float total);
    void SetRandomMutation(int i);
     float RandomNumber(float Min, float Max) {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }
};