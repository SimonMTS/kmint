#include "kmint/ufo/play.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "kmint/main.hpp"  // voor de main loop
#include "kmint/play.hpp"
#include "kmint/ufo/andre.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/ufo/resources.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/pickup.hpp"
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

    for (std::size_t h{0}; h < 100; ++h) {
        s.build_actor<ufo::human>(graph);
    }

    int skip = 2;  // terrible solution
    std::vector<std::reference_wrapper<ufo::human>> humans;
    for (auto &actor : s) {
        if (skip-- > 0) continue;
        ufo::human &h = dynamic_cast<ufo::human &>(actor);
        humans.push_back(h);
        h.other_humans = &humans;
    }

    s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'),
                             tank_type::red);

    s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::green);
    s.build_actor<ufo::andre>(graph, ufo::random_node_of_kind(m, 'R'));

     ufo::tank *tank1 = dynamic_cast<ufo::tank*>(s.actors_[102].get());
     ufo::tank *tank2 = dynamic_cast<ufo::tank*>(s.actors_[103].get());
     ufo::andre *andre = dynamic_cast<ufo::andre*>(s.actors_[104].get());

     tank1->Andre = andre;
     tank2->Andre = andre;

     std::vector<ufo::Pickup *> pickups;

     for (int i = 0; i < 3; i++) {
         s.build_actor<ufo::Pickup>(graph, ufo::random_node_of_kind(m, 'R'), ufo::pickup_type::SHIELD);
         s.build_actor<ufo::Pickup>(graph, ufo::random_node_of_kind(m, 'R'), ufo::pickup_type::EMP);

         ufo::Pickup *pkup1 = dynamic_cast<ufo::Pickup *>(s.actors_[105 + i * 2].get());
         ufo::Pickup *pkup2 = dynamic_cast<ufo::Pickup *>(s.actors_[106 + i * 2].get());

         pickups.push_back(pkup1);
         pickups.push_back(pkup2);

     }

     tank1->pickups = pickups;
     tank2->pickups = pickups;

     std::vector<play::actor *> ufos;

     s.build_actor<ufo::saucer>(saucer_type::blue);
     s.build_actor<ufo::saucer>(saucer_type::green);
     s.build_actor<ufo::saucer>(saucer_type::beige);
     s.build_actor<ufo::saucer>(saucer_type::yellow);

     ufos.push_back(s.actors_[111].get());
     ufos.push_back(s.actors_[112].get());
     ufos.push_back(s.actors_[113].get());
     ufos.push_back(s.actors_[114].get());

     for (auto &actor : s) {
         ufo::human *human = dynamic_cast<ufo::human *>(&actor);
         if (human != nullptr) {
             human->redtank = tank1;
             human->greentank = tank2;
             human->ufos = ufos;
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
