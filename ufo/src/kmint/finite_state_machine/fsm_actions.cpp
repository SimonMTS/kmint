#include "kmint/finite_state_machine/fsm_actions.hpp"

#include <iostream>

#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo::student {

void fsm_actions::Execute_Wander(saucer& s) {
    float xspeed = RandomInt(-1, 1);
    float yspeed = RandomInt(-1, 1);
    s.acceleration += {xspeed, yspeed};
}

void fsm_actions::Execute_HuntHuman(saucer& s) {
    // move toward target human
    {
        s.GetNearest("human");
        ufo::human* human = dynamic_cast<ufo::human*>(s.target);

        math::vector2d desired = human->location() - s.location();
        desired = ::student::forces::limit(desired, 1);

        float humanx = abs(human->velocity.x());
        float humany = abs(human->velocity.y());

        float xspeed = desired.x() * humanx;
        float yspeed = desired.y() * humany;

        desired = math::vector2d{xspeed, yspeed};
        s.acceleration += desired;
    }

    // saucer::AttackHuman() // beam up humans in range
   /* for (auto i = s.begin_perceived(); i != s.end_perceived(); ++i) {
        if (i->EntityType != "human") continue;
        play::actor& human = *i;

        ufo::human* human1 = dynamic_cast<ufo::human*>(&human);

        float distance =
            std::sqrt(std::pow(s.location().x() - human.location().x(), 2) +
                      std::pow(s.location().y() - human.location().y(), 2));

        if (distance < 20 && !human1->isSafeTank && !human1->isSafeHouse) {
            human1->remove();
        }
    }*/
}

void fsm_actions::Execute_HuntTank(saucer& s) {
    // move toward target tank
    {
        s.GetNearest("tank");

        math::vector2d desired = s.target->location() - s.location();
        math::vector2d steer = desired;
        steer = ::student::forces::limit(steer, 1);
        s.acceleration += steer;
    }

    // saucer::AttackTank() // attack tanks in range
    for (auto i = s.begin_perceived(); i != s.end_perceived(); ++i) {
        if (i->EntityType != "tank") continue;
        play::actor& tank = *i;

        ufo::tank* tank1 = dynamic_cast<ufo::tank*>(&tank);
        float distance =
            std::sqrt(std::pow(s.location().x() - tank.location().x(), 2) +
                      std::pow(s.location().y() - tank.location().y(), 2));
        //  if (distance < eatRange && !pig.isSafe) {

        if (distance < 20 && tank1->attackable) {
            // std::cout << "AttackTank" << std::endl;
            // tank.remove();
            if (tank1->UFOAttack()) {
                s.state = nomove;
                return;
            };
        }
    }
}

void fsm_actions::Execute_NoMove(saucer& s) {
    s.acceleration = {0, 0};
    s.velocity = {0, 0};

    s.NoMoveCount++;

    // Saucer acts ever 0.1s so 200 for 20s
    if (s.NoMoveCount >= 200) {
        s.NoMoveCount = 0;
        s.state = wander;
    }
}

#pragma region helpers

float fsm_actions::RandomInt(float Min, float Max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(Min * 1000, Max * 1000);
    return ((float)distr(gen)) / 1000;
}

#pragma endregion helpers

}  // namespace kmint::ufo::student