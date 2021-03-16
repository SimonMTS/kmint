#pragma once
#include <vector>

#include "kmint/ufo/human.hpp"

class Population {
   public:
    Population(std::vector<kmint::ufo::human *> &h);
    std::vector<kmint::ufo::human *> &humans;
    void setPopulation(std::vector<kmint::ufo::human *> humans);
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
};