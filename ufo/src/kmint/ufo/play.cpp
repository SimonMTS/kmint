#include "kmint/ufo/play.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "kmint/force_driven_entities/population.hpp"
#include "kmint/main.hpp"  // voor de main loop
#include "kmint/play.hpp"
#include "kmint/ufo/andre.hpp"
#include "kmint/ufo/doors.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/ufo/pickup.hpp"
#include "kmint/ufo/resources.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/ui.hpp"
namespace kmint::ufo {

int play() {
    // een app object is nodig om
    ui::app app{};

    //  maak een venster aan
    ui::window window{app.create_window({1024, 768}, "kmint", 1.0)};

    // maak een podium aan
    play::stage s{{1024, 768}};

    auto m = map();
    auto &graph = m.graph();

    s.build_actor<play::background>(math::size(1024, 768),
                                    graphics::image{m.background_image()});
    s.build_actor<play::map_actor>(math::vector2d{0.f, 0.f}, m.graph());

    int id = 0;
    for (std::size_t h{0}; h < 100; ++h) {
        s.build_actor<ufo::human>(graph, id);
        id++;
    }

    std::vector<kmint::ufo::human *> humans;
    for (int i = 0; i < s.actors_.size(); i++) {
        ufo::human *human = dynamic_cast<ufo::human *>(s.actors_[i].get());

        if (human != nullptr) {
            humans.push_back(human);
        }
    }

    s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'),
                             tank_type::red);

    s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'),
                             tank_type::green);
    s.build_actor<ufo::andre>(graph, ufo::random_node_of_kind(m, 'R'));

    ufo::tank *tank1 = dynamic_cast<ufo::tank *>(s.actors_[102].get());
    ufo::tank *tank2 = dynamic_cast<ufo::tank *>(s.actors_[103].get());
    ufo::andre *andre = dynamic_cast<ufo::andre *>(s.actors_[104].get());

    tank1->Andre = andre;
    tank2->Andre = andre;

    std::vector<ufo::Pickup *> pickups;

    for (int i = 0; i < 3; i++) {
        s.build_actor<ufo::Pickup>(graph, ufo::random_node_of_kind(m, 'R'),
                                   ufo::pickup_type::SHIELD);
        s.build_actor<ufo::Pickup>(graph, ufo::random_node_of_kind(m, 'R'),
                                   ufo::pickup_type::EMP);

        ufo::Pickup *pkup1 =
            dynamic_cast<ufo::Pickup *>(s.actors_[105 + i * 2].get());
        ufo::Pickup *pkup2 =
            dynamic_cast<ufo::Pickup *>(s.actors_[106 + i * 2].get());

        pickups.push_back(pkup1);
        pickups.push_back(pkup2);
    }

    tank1->pickups = pickups;
    tank2->pickups = pickups;

    std::vector<play::actor *> ufos;

    s.build_actor<ufo::saucer>(saucer_type::blue,
                               ufo::random_node_of_kind(m, 'R').location());
   // s.build_actor<ufo::saucer>(saucer_type::green,
    //                           ufo::random_node_of_kind(m, 'R').location());
   // s.build_actor<ufo::saucer>(saucer_type::beige,
   //                            ufo::random_node_of_kind(m, 'R').location());
   // s.build_actor<ufo::saucer>(saucer_type::yellow,
   //                            ufo::random_node_of_kind(m, 'R').location());

    ufos.push_back(s.actors_[111].get());
   // ufos.push_back(s.actors_[112].get());
   // ufos.push_back(s.actors_[113].get());
   // ufos.push_back(s.actors_[114].get());

    std::vector<play::actor *> doors;

    s.build_actor<ufo::Doors>(graph, math::vector2d{401, 628});
    s.build_actor<ufo::Doors>(graph, math::vector2d{460, 613});
    s.build_actor<ufo::Doors>(graph, math::vector2d{640, 496});
    s.build_actor<ufo::Doors>(graph, math::vector2d{624, 320});
    s.build_actor<ufo::Doors>(graph, math::vector2d{688, 129});
    s.build_actor<ufo::Doors>(graph, math::vector2d{598, 139});

    doors.push_back(s.actors_[112].get());
    doors.push_back(s.actors_[113].get());
    doors.push_back(s.actors_[114].get());
    doors.push_back(s.actors_[115].get());
    doors.push_back(s.actors_[116].get());
    doors.push_back(s.actors_[117].get());

    Population population{humans};
    population.saveProperties();

    for (auto &actor : s) {
        ufo::human *human = dynamic_cast<ufo::human *>(&actor);
        if (human != nullptr) {
            human->redtank = tank1;
            human->greentank = tank2;
            human->ufos = ufos;
            human->population = &population;
            human->doors = doors;
        }
    }

    // Maak een event_source aan (hieruit kun je alle events halen, zoals
    // toetsaanslagen)
    ui::events::event_source event_source{};

    // main_loop stuurt alle actors aan.
    main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
        // gebruik dt om te kijken hoeveel tijd versterken is
        // sinds de vorige keer dat deze lambda werd aangeroepen
        // loop controls is een object met eigenschappen die je kunt gebruiken
        // om de main-loop aan te sturen.

        population.Update();

        if (andre->totalsteps >= 200) {
            population.getParents();
            s.actors_.erase(s.actors_.begin() + 2, s.actors_.end());

            id = 0;
            for (std::size_t h{0}; h < 100; ++h) {
                s.build_actor<ufo::human>(graph, id);
                id++;
            }

            humans.clear();
            for (int i = 0; i < s.actors_.size(); i++) {
                ufo::human *human =
                    dynamic_cast<ufo::human *>(s.actors_[i].get());

                if (human != nullptr) {
                    humans.push_back(human);
                }
            }

            s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'),
                                     tank_type::red);

            s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'),
                                     tank_type::green);
            s.build_actor<ufo::andre>(graph, ufo::random_node_of_kind(m, 'R'));

            tank1 = dynamic_cast<ufo::tank *>(s.actors_[102].get());
            tank2 = dynamic_cast<ufo::tank *>(s.actors_[103].get());
            andre = dynamic_cast<ufo::andre *>(s.actors_[104].get());

            tank1->Andre = andre;
            tank2->Andre = andre;

            pickups.clear();

            for (int i = 0; i < 3; i++) {
                s.build_actor<ufo::Pickup>(graph,
                                           ufo::random_node_of_kind(m, 'R'),
                                           ufo::pickup_type::SHIELD);
                s.build_actor<ufo::Pickup>(graph,
                                           ufo::random_node_of_kind(m, 'R'),
                                           ufo::pickup_type::EMP);

                ufo::Pickup *pkup1 =
                    dynamic_cast<ufo::Pickup *>(s.actors_[105 + i * 2].get());
                ufo::Pickup *pkup2 =
                    dynamic_cast<ufo::Pickup *>(s.actors_[106 + i * 2].get());

                pickups.push_back(pkup1);
                pickups.push_back(pkup2);
            }

            tank1->pickups = pickups;
            tank2->pickups = pickups;

            ufos.clear();

            s.build_actor<ufo::saucer>(
                saucer_type::blue, ufo::random_node_of_kind(m, 'R').location());
            s.build_actor<ufo::saucer>(
                saucer_type::green,
                ufo::random_node_of_kind(m, 'R').location());
            s.build_actor<ufo::saucer>(
                saucer_type::beige,
                ufo::random_node_of_kind(m, 'R').location());
            s.build_actor<ufo::saucer>(
                saucer_type::yellow,
                ufo::random_node_of_kind(m, 'R').location());

            ufos.push_back(s.actors_[111].get());
            ufos.push_back(s.actors_[112].get());
            ufos.push_back(s.actors_[113].get());
            ufos.push_back(s.actors_[114].get());

            doors.clear();

            s.build_actor<ufo::Doors>(graph, math::vector2d{401, 628});
            s.build_actor<ufo::Doors>(graph, math::vector2d{460, 613});
            s.build_actor<ufo::Doors>(graph, math::vector2d{640, 496});
            s.build_actor<ufo::Doors>(graph, math::vector2d{624, 320});
            s.build_actor<ufo::Doors>(graph, math::vector2d{688, 129});
            s.build_actor<ufo::Doors>(graph, math::vector2d{598, 139});

            doors.push_back(s.actors_[115].get());
            doors.push_back(s.actors_[116].get());
            doors.push_back(s.actors_[117].get());
            doors.push_back(s.actors_[118].get());
            doors.push_back(s.actors_[119].get());
            doors.push_back(s.actors_[120].get());

            population.humans = humans;
            population.Crossover();
            population.Mutation();
            population.saveProperties();

            for (auto &actor : s) {
                ufo::human *human = dynamic_cast<ufo::human *>(&actor);
                if (human != nullptr) {
                    human->redtank = tank1;
                    human->greentank = tank2;
                    human->ufos = ufos;
                    human->population = &population;
                    human->doors = doors;
                }
            }
        }

        for (ui::events::event &e : event_source) {
            // event heeft een methode handle_quit die controleert
            // of de gebruiker de applicatie wilt sluiten, en zo ja
            // de meegegeven functie (of lambda) aanroept om met het
            // bijbehorende quit_event
            //
            e.handle_quit(
                [&ctl](ui::events::quit_event qe) { ctl.quit = true; });
            e.handle_key_up([&](auto ke) {
                switch (ke.key) {
                    case ui::events::key::one:
                        population.increaseAlignment();
                        break;
                    case ui::events::key::two:
                        population.decreaseAlignment();
                        break;
                    case ui::events::key::three:
                        population.increaseCohesion();
                        break;
                    case ui::events::key::four:
                        population.decreaseCohesion();
                        break;
                    case ui::events::key::five:
                        population.increaseSeparation();
                        break;
                    case ui::events::key::six:
                        population.decreaseSeparation();
                        break;
                    case ui::events::key::seven:
                        population.increaseTank();
                        break;
                    case ui::events::key::eight:
                        population.decreaseTank();
                        break;
                    case ui::events::key::nine:
                        population.increaseUfo();
                        break;
                    case ui::events::key::zero:
                        population.decreaseUfo();
                        break;
                    case ui::events::key::p:
                        ctl.pause = !ctl.pause;
                        break;
                    case ui::events::key::r:
                        ctl.render = !ctl.render;
                        break;
                    case ui::events::key::opening_bracket:
                        ctl.time_scale /= 2.0;
                        break;
                    case ui::events::key::closing_bracket:
                        ctl.time_scale *= 2.0;
                        break;
                    default:
                        break;
                }
            });
        }
    });
    return 0;
}

}  // namespace kmint::ufo
