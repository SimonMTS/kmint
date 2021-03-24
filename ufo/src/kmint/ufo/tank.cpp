#include "kmint/ufo/tank.hpp"

// #include <bits/stdc++.h>  // do not remove

#include <iostream>

#include "kmint/a_star/a_star.hpp"
#include "kmint/a_star/heuristics.hpp"
#include "kmint/graphics.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/node_algorithm.hpp"
namespace kmint::ufo {

namespace {
graphics::image tank_image(tank_type t) {
    constexpr scalar scale = 0.35;
    switch (t) {
        case tank_type::red:
            return graphics::image{"resources/tank_red.png", scale};
        case tank_type::green:
            return graphics::image{"resources/tank_green.png", scale};
    }
    return graphics::image{"resources/tank_green.png", scale};
}

}  // namespace

tank::tank(map::map_graph &g, map::map_node &initial_node, tank_type t)
    : graph{g},
      play::map_bound_actor{initial_node},
      type_{t},
      drawable_{*this, graphics::image{tank_image(t)}} {
    EntityType = "tank";
}

bool cmpp(std::pair<play::actor *, int> a, std::pair<play::actor *, int> b) {
    if (a.second > b.second) return true;
    return false;
}

void tank::act(delta_time dt) {
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 1) {
        if (this->type_ == tank_type::red) {
            graph.untag_all();
            // for (int i = 0; i < graph.num_nodes() - 1; i++) {
            //     if (graph[i].tag() == kmint::graph::node_tag::visited) {
            //         graph[i].tag(kmint::graph::node_tag::normal);
            //     }
            // }
        }

        if (damage >= 10000) {
            state = repair;
        }

        if (state == wander) {
            SenseUFO();
        }

        switch (state) {
            case wander:
                Wander();
                break;
            case flee:
                Flee();
                break;
            case gotoEMP:
                GoToEMP();
                break;
            case gotoShield:
                GoToShield();
                break;
            case repair:
                Repair();
                break;
        }

        Move();
        RoadkillOrSave();
        SetSprite();
        TagPath();
        t_since_move_ = from_seconds(0);
    }

    if (!attackable) t_since_attack_ += dt;
    if (to_seconds(t_since_attack_) > 1) {
        attackable = true;
        t_since_attack_ = from_seconds(0);
    }
}

void tank::Wander() {
    state = wander;
    if (weight < 1) {
        next_edge = &node()[random_int(0, node().num_edges())];
    }
};

void tank::SenseUFO() {
    this->ufos.clear();
    std::vector<play::actor *> ufos;
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType == "ufo") {
            play::actor &ufo = *i;
            ufos.push_back(&ufo);
        }
    }
    if (ufos.size() == 0) return;

    this->ufos = ufos;

    int number = RandomInt(0, 100);
    if (number >= 100 - FleeChance) {
        Flee();
    } else if (number >= 100 - FleeChance - EMPChance) {
        GoToEMP();
    } else if (number <= ShieldChance) {
        GoToShield();
    }
}

void tank::Flee() {
    state = flee;
    play::actor *ToFleeFrom = GetNearestUFO(ufos);

    if (fleecount < 10) {
        MoveAwayFrom(ToFleeFrom);
        fleecount++;
    } else {
        lastchoice = flee;
        fleecount = 0;
        Wander();
    }
};
void tank::GoToEMP() {
    state = gotoEMP;

    GoTo(pickup_type::EMP);

    if (this->path.size() == 0 && target != nullptr) {
        // target->available = false;
        // target->remove();
        target->NewLocation();
        lastchoice = gotoEMP;

        state = wander;
        LaserShieldCount++;
        target = nullptr;
        // std::cout << "Picked up EMP" << std::endl;

        return;
    }
};
void tank::GoToShield() {
    state = gotoShield;

    GoTo(pickup_type::SHIELD);

    if (this->path.size() == 0 && target != nullptr) {
        // target->available = false;
        // target->remove();
        target->NewLocation();
        state = wander;
        LaserShieldCount++;
        target = nullptr;
        lastchoice = gotoShield;

        // std::cout << "Picked up shield" << std::endl;
        return;
    }
};

void tank::Repair() {
    // std::cout << "Repair" << std::endl;
    state = repair;

    if (this->node().node_id() == Andre->node().node_id()) {
        state = wander;
        damage = 0;
        return;
    }

    GoToRepair();
}

void tank::GoToRepair() {
    // if (Andre->path.size() == 0) return;
    // const map_node &endnode = Andre->path[Andre->path.size() / 2 - 1];

    path = ufo::student::a_star::find_path(
        node(), Andre->node(), this->graph,
        ufo::student::heuristics::euclidean_distance);

    if (path.size() == 0) {
        // std::cout << "Already there";
        return;
    }

    for (int i = 0; i < this->node().num_edges(); i++) {
        if (node()[i].to().node_id() ==
            this->path[this->path.size() - 1].get().node_id()) {
            this->path.pop_back();
            next_edge = &node()[i];
            return;
        }
    }
}

void tank::GoTo(pickup_type type) {
    if (target == nullptr) {
        GetNearestPickup(type);
    }
    if (path.size() == 0) {
        // std::cout << "Already there";
        return;
    }
    for (int i = 0; i < this->node().num_edges(); i++) {
        if (node()[i].to().node_id() ==
            this->path[this->path.size() - 1].get().node_id()) {
            this->path.pop_back();
            next_edge = &node()[i];
            return;
        }
    }
}

void tank::GetNearestPickup(pickup_type type) {
    std::vector<std::reference_wrapper<map_node>> shortestpath;

    for (int i = 0; i < pickups.size(); i++) {
        if (pickups[i]->type != type) continue;
        if (pickups[i]->removed()) continue;

        std::vector<std::reference_wrapper<map_node>> currpath;

        currpath = ufo::student::a_star::find_path(
            node(), pickups[i]->node(), this->graph,
            ufo::student::heuristics::euclidean_distance);

        if (shortestpath.size() == 0 || currpath.size() < shortestpath.size()) {
            shortestpath = currpath;
            target = pickups[i];
        }
    }
    path = shortestpath;
}

play::actor *tank::GetNearestUFO(std::vector<play::actor *> ufos) {
    std::priority_queue<std::pair<play::actor *, int>,
                        std::vector<std::pair<play::actor *, int>>,
                        std::function<bool(std::pair<play::actor *, int>,
                                           std::pair<play::actor *, int>)>>
        queue(cmpp);

    for (int i = 0; i < ufos.size(); i++) {
        float distance = std::sqrt(
            std::pow(node().location().x() - ufos[i]->location().x(), 2) +
            std::pow(node().location().y() - ufos[i]->location().y(), 2));
        queue.push(std::make_pair(ufos[i], distance));
    }

    if (!queue.empty()) {
        return queue.top().first;
    }
}

void tank::Move() {
    if (next_edge != last_edge) {
        weight = next_edge->weight();
        last_edge = next_edge;
    }

    weight--;
    if (weight == 0) {
        node(this->next_edge->to());
    }
}

void tank::MoveTo(const int nodeid) {
    const kmint::map::map_node &end = graph[nodeid];
    kmint::map::map_node &start = this->node();
    ufo::student::node_list nodelist = ufo::student::a_star::find_path(
        start, end, this->graph, ufo::student::heuristics::euclidean_distance);
    std::cout << nodelist.size() << std::endl;

    if (nodelist.size() > 1) {
        for (int i = 0; i < this->node().num_edges(); i++) {
            if (node()[i].to().node_id() == nodelist.back().get().node_id()) {
                next_edge = &node()[i];
                nodelist.pop_back();
                return;
            }
        }
    } else {
        for (int i = 0; i < this->node().num_edges(); i++) {
            if (node()[i].to().node_id() == nodeid) {
                next_edge = &node()[i];
                return;
            }
        }
    }
}

void tank::MoveAwayFrom(play::actor *actor) {
    float distance = INT_MIN;
    map::map_edge *edge = nullptr;
    for (int i = 0; i < this->node().num_edges(); i++) {
        float currdistance = std::sqrt(
            std::pow(node().location().x() - actor->location().x(), 2) +
            std::pow(node().location().y() - actor->location().y(), 2));

        if (currdistance > distance) {
            distance = currdistance;
            edge = &node()[i];
        }
    }
    if (edge != nullptr) {
        next_edge = edge;
    } else {
        throw "MoveAwayFrom nullptr";
    }
}

void tank::SetSprite() {
    if (state == wander) {
        drawable_.set_tint({255, 255, 255});
    }

    else if (state == flee) {
        drawable_.set_tint({0, 0, 0});
    } else if (state == gotoEMP) {
        drawable_.set_tint({255, 140, 0});
    } else if (state == gotoShield) {
        drawable_.set_tint({0, 0, 255});
    }
}

bool tank::UFOAttack() {
    attackable = false;
    if (EMPCount > 0) {
        // std::cout << "Ufoattack EMP" << std::endl;
        EMPCount--;
        DamageHistory.push_back(0);
        UpdateChances();
        return true;
    }

    if (LaserShieldCount > 0) {
        LaserShieldCount--;
        damage += 20;
        DamageHistory.push_back(20);
        UpdateChances();

        return false;
    }

    damage += 50;
    DamageHistory.push_back(50);
    UpdateChances();

    return false;
};

void tank::UpdateChances() {
    if (lastchoice == wander) {
        std::cout << "Lastchoice is Wander" << std::endl;
        DamageHistory.erase(DamageHistory.end() - 1);
        return;
    }
    if (lastchoice == gotoEMP) {
        std::cout << "Lastchoice is EMP" << std::endl;
    }
    if (lastchoice == gotoShield) {
        std::cout << "Lastchoice is Shield" << std::endl;
    }

     if (lastchoice == flee) {
        std::cout << "Lastchoice is Flee" << std::endl;
    }
    int lastdamage = DamageHistory[DamageHistory.size() - 1];
    int avgdamage = 0;

    for (int i = 0; i < DamageHistory.size(); i++) {
        avgdamage += DamageHistory[i];
    }
    avgdamage /= DamageHistory.size();

    if (lastdamage < avgdamage) {
        if (lastchoice == State::flee && EMPChance > 0 && ShieldChance > 0) {
            FleeChance += 2;
            EMPChance--;
            ShieldChance--;
        }

        if (lastchoice == State::gotoEMP && FleeChance > 0 &&
            ShieldChance > 0) {
            FleeChance--;
            EMPChance += 2;
            ShieldChance--;
        }

        if (lastchoice == State::gotoShield && FleeChance > 0 &&
            EMPChance > 0) {
            FleeChance--;
            EMPChance--;
            ShieldChance += 2;
        }
    } else if (lastdamage >= avgdamage) {
        if (lastchoice == State::flee && (EMPChance + ShieldChance < 99)) {
            FleeChance -= 2;
            EMPChance++;
            ShieldChance++;
        }

        if (lastchoice == State::gotoEMP && (FleeChance + ShieldChance < 99)) {
            FleeChance++;
            EMPChance -= 2;
            ShieldChance++;
        }

        if (lastchoice == State::gotoShield && (FleeChance + EMPChance < 99)) {
            FleeChance++;
            EMPChance++;
            ShieldChance -= 2;
        }
    }

        std::cout  << " LASTCHOICE " << lastchoice
                  << " AVG " << avgdamage << " LAST " << lastdamage << " FLEE "
                  << FleeChance << " EMP " << EMPChance << " SHIELD " << ShieldChance
                  << " SIZE " << DamageHistory.size() << std::endl;
    
}

void tank::RoadkillOrSave() {
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != "human") continue;
        play::actor &human1 = *i;

        ufo::human *human = dynamic_cast<ufo::human *>(&human1);
        float distance =
            std::sqrt(std::pow(location().x() - human->location().x(), 2) +
                      std::pow(location().y() - human->location().y(), 2));

        if (distance > 20) continue;

        if (this->type_ == tank_type::red) {  // Roadkill
            human1.remove();
        } else if (this->type_ == tank_type::green) {
            human->isSafeTank = true;
        }
    }
}

bool tank::AvailablePickup(pickup_type type) {
    for (int i = 0; i < pickups.size(); i++) {
        if (pickups[i]->type == type && pickups[i]->available) return true;
    }
    return false;
}

void tank::TagPath() {
    for (auto node : path) {
        node.get().tag(kmint::graph::node_tag::path);
    }
}
}  // namespace kmint::ufo
