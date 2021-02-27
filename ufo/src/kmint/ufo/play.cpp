#include "kmint/ufo/play.hpp"
#include "kmint/main.hpp" // voor de main loop
#include "kmint/play.hpp"
#include "kmint/ufo/andre.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/resources.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/ui.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

namespace kmint::ufo {

int play() {
  // een app object is nodig om
  ui::app app{};

  //  maak een venster aan
  ui::window window{app.create_window({1024, 768}, "kmint", 1.0)};

  // maak een podium aan
  play::stage s{ {1024, 768} };

  auto m = map();
  auto& graph = m.graph();

  s.build_actor<play::background>(math::size(1024, 768),
                                  graphics::image{m.background_image()});
  s.build_actor<play::map_actor>(math::vector2d{0.f, 0.f}, m.graph());

  for (std::size_t h{0}; h < 100; ++h) {
    s.build_actor<ufo::human>();
  }


  s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::red);
  s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::green);
  s.build_actor<ufo::andre>(graph, ufo::random_node_of_kind(m, 'R'));
  s.build_actor<ufo::saucer>(saucer_type::blue);
  s.build_actor<ufo::saucer>(saucer_type::green);
  s.build_actor<ufo::saucer>(saucer_type::beige);
  s.build_actor<ufo::saucer>(saucer_type::yellow);

  // Maak een event_source aan (hieruit kun je alle events halen, zoals
  // toetsaanslagen)
  ui::events::event_source event_source{};

  // main_loop stuurt alle actors aan.
  main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
    // gebruik dt om te kijken hoeveel tijd versterken is
    // sinds de vorige keer dat deze lambda werd aangeroepen
    // loop controls is een object met eigenschappen die je kunt gebruiken om de
    // main-loop aan te sturen.

    for (ui::events::event &e : event_source) {
      // event heeft een methode handle_quit die controleert
      // of de gebruiker de applicatie wilt sluiten, en zo ja
      // de meegegeven functie (of lambda) aanroept om met het
      // bijbehorende quit_event
      //
      e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
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

} // namespace kmint::ufo
