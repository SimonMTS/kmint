#include "kmint/finite_state_machine/fsm_actions.hpp"

#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo::student {

void fsm_actions::Execute_Wander(saucer& s) {
    s.acceleration += s.WanderDirection * 0.01;

    // saucer::SetWanderDirection()
    float xspeed = 20.f * RandomInt(-0.1, 0.1);
    float yspeed = 20.f * RandomInt(-0.1, 0.1);
    s.WanderDirection = {xspeed, yspeed};
}

void fsm_actions::Execute_HuntHuman(saucer& s) {
    // move toward target human
    {
        s.GetNearest("human");
        ufo::human* human = dynamic_cast<ufo::human*>(s.target);

        math::vector2d desired = s.target->location() - s.location();
        math::vector2d steer = desired + human->velocity;
        steer = ::student::forces::limit(steer, 1);
        s.acceleration += steer;
    }

    // saucer::AttackHuman() // beam up humans in range
    for (auto i = s.begin_perceived(); i != s.end_perceived(); ++i) {
        if (i->EntityType != "human") continue;
        play::actor& human = *i;

        ufo::human* human1 = dynamic_cast<ufo::human*>(&human);

        float distance =
            std::sqrt(std::pow(s.location().x() - human.location().x(), 2) +
                      std::pow(s.location().y() - human.location().y(), 2));

        if (distance < 20 && !human1->isSafeTank) {
            human1->remove();
        }
    }
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

void fsm_actions::Execute_NoMove(saucer& s) { s.acceleration = {0, 0}; }

#pragma region helpers

int fsm_actions::RandomInt(float Min, float Max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-1, 1);
    return distr(gen);
}

#pragma endregion helpers

}  // namespace kmint::ufo::student