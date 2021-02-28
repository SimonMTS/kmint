#include "kmint/force_driven_entities/forces.hpp"

#include "kmint/a_star/heuristics.hpp"

namespace student {

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

kmint::math::vector2d forces::cohesion(const kmint::ufo::human &human) {
    // std::cout << human.other_humans->size() << "\n";
    kmint::math::vector2d sum = kmint::math::vector2d{0, 0};
    int count = 0;

    for (auto &other_human : *human.other_humans) {
        if (other_human.get().location() == human.location()) continue;

        int dist;
        {
            int pxdist = 16;
            int x = other_human.get().location().x() - human.location().x();
            int y = other_human.get().location().y() - human.location().y();
            int res = sqrt(pow(x, 2) + pow(y, 2));  // / pxdist;
            dist = res;
        }

        if (dist < 400) {
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

}  // namespace student