
#include "kmint/ufo/doors.hpp"
#include "kmint/graphics.hpp"
#include "kmint/map/map.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/human.hpp"

namespace kmint::ufo {
namespace {
    graphics::image door_image() { return graphics::image{"resources/door.png"}; }

}

Doors::Doors(map::map_graph &g, math::vector2d location)
    : play::free_roaming_actor{location},
    drawable_{*this, graphics::image{door_image()}},
    graph_{g}{
};

void Doors::act(delta_time dt){
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 0.1) {
        for (auto i = begin_perceived(); i != end_perceived(); ++i) {
            if (i->EntityType != "human") continue;
            play::actor &human = *i;

            ufo::human *human1 = dynamic_cast<ufo::human *>(&human);

            human1->isSafeHouse = true;
            human1->setLocation(location());
            
        }

    }

};



}  // namespace kmint::ufo