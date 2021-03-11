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

kmint::math::vector2d forces::normalize(const kmint::math::vector2d &vector) {
    auto len = sqrt((vector.x() * vector.x()) + (vector.y() * vector.y()));

    // if (len <= 1) {
    //     return vector;
    // } else {
    return vector / len;
    // }
}

// todo this is just a tmp implementation, should be redone properly
kmint::math::vector2d forces::cohesion(const kmint::ufo::human &human) {
    kmint::math::vector2d sum = kmint::math::vector2d{0, 0};
    int count = 0;

    if (human.removed()) return {0, 0};

    for (auto &other_human : *human.other_humans) {
        if (other_human.get().removed()) continue;
        if (other_human.get().location() == human.location()) continue;

        int dist;
        {
            int pxdist = 16;
            int x = other_human.get().location().x() - human.location().x();
            int y = other_human.get().location().y() - human.location().y();
            int res = sqrt(pow(x, 2) + pow(y, 2));  // / pxdist;
            dist = res;
        }

        if (dist < 50) {
            sum += other_human.get().location() - human.location();
            count++;
        }
    }

    if (count > 0) {
        sum /= count;
        return sum;
    } else {
        return {0, 0};
    }
}

kmint::math::vector2d forces::separation(const kmint::ufo::human &human) {
    kmint::math::vector2d steer {0, 0};
    int count = 0;

    if (human.removed()) return steer;

    for (auto &other_human : *human.other_humans) {
        if (&other_human == nullptr) {
            std::cout << "other human is nullptr" << std::endl;
        }
        if (other_human.get().removed()) continue;
        if (other_human.get().location() == human.location()) continue;

        float d = distance(human, other_human);

        if ((d > 0) && (d < human.separationdist)) {
            kmint::math::vector2d diff = kmint::math::vector2d{0, 0};
            diff = human.location() - other_human.get().location();
            diff = Normalize(diff);
            diff = diff / d;
            steer += diff;
            count++;
        }
    }

    if (count > 0) {
        steer = steer / float(count);
    }

    auto steermag = sqrt(steer.x() * steer.x() + steer.y() * steer.y());

    if (steermag > 0) {
        steer = Normalize(steer);
        steer *= human.maxSpeed;
        steer -= human.velocity;
        steer = limit(steer, human.maxForce);
    }
    return steer;
}

// todo
kmint::math::vector2d forces::alignment(const kmint::ufo::human &human) {
    return {0, 0};
}

kmint::math::vector2d forces::limit(const kmint::math::vector2d &v, float maxforce) {
    kmint::math::vector2d temp = v;
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    if (m > maxforce) {
        temp = {v.x() / m, v.y() / m};
    }
    return temp;
};

float forces::distance(const kmint::ufo::human& human,
    const kmint::play::actor& actor) {

    float dx = human.location().x() - actor.location().x();
    float dy = human.location().y() - actor.location().y();
    float dist = sqrt(dx * dx + dy * dy);
    return dist;
}

kmint::math::vector2d forces::Normalize(kmint::math::vector2d v) {
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    kmint::math::vector2d temp;
    if (m > 0) {
        temp = kmint::math::vector2d{v.x() / m, v.y() / m};
    } else {
        temp = kmint::math::vector2d{v.x(), v.y()};
    }
    return temp;
}

}  // namespace student