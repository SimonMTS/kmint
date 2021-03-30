#include "kmint/force_driven_entities/forces.hpp"

#include "kmint/a_star/heuristics.hpp"

namespace student {

// todo this is probably not a good way to do this
kmint::math::vector2d forces::stay_on_map(const kmint::ufo::human &human) {
    const auto &closest =
        kmint::ufo::find_closest_node_to(human.graph_, human.location());
    auto df = human.location() - closest.location();

    if (std::abs(df.x()) > 10 || std::abs(df.y()) > 10) {
        df.y(-std::min((float)1, df.y()));
        df.x(-std::min((float)1, df.x()));
        return df;
    } else {
        return {0, 0};
    }
}

std::default_random_engine forces::generator{};
std::uniform_real_distribution<float> forces::distribution{-1, 1};
kmint::math::vector2d forces::wander(const kmint::ufo::human &human) {
    return {distribution(generator), distribution(generator)};
}

kmint::math::vector2d forces::cohesion(const kmint::ufo::human &human) {
    kmint::math::vector2d sum = kmint::math::vector2d{0, 0};
    int count = 0;
    for (unsigned long i = 0; i < human.population->humans.size(); i++) {
        // the memory in other can be corrupted
        kmint::ufo::human &other = *human.population->humans[i];
        // this prevents most crashes, but is not a real fix
        if (!human.removed() && !other.removed() && human.id != other.id &&
            &other.graph_ == &human.graph_) {
            float d = distance(human, other.location());
            if ((d > 0) && (d < human.DesiredCohesionDistance)) {
                sum += other.location();
                count++;
            }
        }
    }
    if (count > 0) {
        sum = sum / count;
        return seek(human, sum);
    }
    return kmint::math::vector2d{0, 0};
}

kmint::math::vector2d forces::separation(const kmint::ufo::human &human) {
    kmint::math::vector2d steer{0, 0};
    int count = 0;

    for (unsigned long i = 0; i < human.population->humans.size(); i++) {
        // the memory in other can be corrupted
        kmint::ufo::human &other = *human.population->humans[i];
        // this prevents most crashes, but is not a real fix
        if (!human.removed() && !other.removed() && human.id != other.id &&
            &other.graph_ == &human.graph_) {
            float d = distance(human, other.location());

            if ((d > 0) && (d < human.DesiredSeparationDistance)) {
                kmint::math::vector2d diff = kmint::math::vector2d{0, 0};
                diff = human.location() - other.location();
                diff = normalize(diff);
                diff = diff / d;
                steer += diff;
                count++;
            }
        }
    }
    if (count > 0) {
        steer = steer / float(count);
    }

    auto steermag = sqrt(steer.x() * steer.x() + steer.y() * steer.y());

    if (steermag > 0) {
        steer = normalize(steer);
        steer *= human.maxSpeed;
        steer -= human.velocity;
        steer = limit(steer, human.maxForce);
    }
    return steer;
}

kmint::math::vector2d forces::alignment(const kmint::ufo::human &human) {
    kmint::math::vector2d sum{0, 0};

    int count = 0;
    for (unsigned long i = 0; i < human.population->humans.size(); i++) {
        // the memory in other can be corrupted
        kmint::ufo::human &other = *human.population->humans[i];
        // this prevents most crashes, but is not a real fix
        if (!human.removed() && !other.removed() && human.id != other.id &&
            &other.graph_ == &human.graph_) {
            float d = distance(human, other.location());

            if ((d > 0) && (d < human.DesiredAlignmentDistance)) {
                sum += other.velocity;
                count++;
            }
        }
    }

    if (count > 0) {
        sum = sum / (float)count;
        sum = normalize(sum);
        sum = sum * human.maxSpeed;

        kmint::math::vector2d steer;
        steer = sum - human.velocity;
        steer = limit(steer, human.maxForce);
        return steer;
    } else {
        return kmint::math::vector2d{0, 0};
    }
}

kmint::math::vector2d forces::attacted_to(const kmint::ufo::human &human,
                                          const kmint::play::actor &actor,
                                          float range) {
    kmint::math::vector2d steer{0, 0};
    int count = 0;

    float d = distance(human, actor);

    if ((d > 0) && (d < range)) {
        kmint::math::vector2d diff = kmint::math::vector2d{0, 0};
        diff = human.location() - actor.location();
        diff *= 900;
        steer += diff;
        count++;
    }

    if (count > 0) {
        steer = steer / float(count);
    }

    auto steermag = sqrt(steer.x() * steer.x() + steer.y() * steer.y());

    if (steermag > 0) {
        steer = normalize(steer);
        steer *= human.maxSpeed;
        steer -= human.velocity;
        steer = limit(steer, human.maxForce);
        steer = steer * 3;
    }

    return steer * -1;
}

kmint::math::vector2d forces::limit(const kmint::math::vector2d &v,
                                    float maxforce) {
    kmint::math::vector2d temp = v;
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    if (m > maxforce) {
        temp = {v.x() / m, v.y() / m};
    }
    return temp;
}

float forces::distance(const kmint::ufo::human &human,
                       const kmint::play::actor &actor) {
    float dx = human.location().x() - actor.location().x();
    float dy = human.location().y() - actor.location().y();
    float dist = sqrt(dx * dx + dy * dy);
    return dist;
}

float forces::distance(const kmint::play::free_roaming_actor &human,
                       const kmint::math::vector2d &v) {
    float dx = human.location().x() - v.x();
    float dy = human.location().y() - v.y();
    float dist = sqrt(dx * dx + dy * dy);
    return dist;
}
kmint::math::vector2d forces::normalize(const kmint::math::vector2d &v) {
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    kmint::math::vector2d temp;
    if (m > 0) {
        temp = kmint::math::vector2d{v.x() / m, v.y() / m};
    } else {
        temp = kmint::math::vector2d{v.x(), v.y()};
    }
    return temp;
}

kmint::math::vector2d forces::seek(const kmint::ufo::human &human,
                                   const kmint::math::vector2d &v) {
    kmint::math::vector2d desired = v - human.location();

    desired = normalize(desired);

    desired = desired * human.maxSpeed;

    kmint::math::vector2d steer = desired - human.velocity;

    steer = limit(steer, human.maxForce);
    return steer;
}

}  // namespace student