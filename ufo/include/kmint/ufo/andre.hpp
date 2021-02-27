#ifndef KMINT_UFO_ANDRE_HPP
#define KMINT_UFO_ANDRE_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"


namespace kmint::ufo {

class andre : public play::map_bound_actor {
public:
	andre(map::map_graph& g, map::map_node& initial_node);
	// wordt elke game tick aangeroepen
	void act(delta_time dt) override;
	ui::drawable const& drawable() const override { return drawable_; }
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
	// geeft de lengte van een zijde van de collision box van deze actor terug.
	// Belangrijk voor collision detection
	scalar collision_range() const override { return 16.0; }
	// geeft aan dat andré andere actors kan zien
	bool perceptive() const override { return true; }

private:
	// hoeveel tijd is verstreken sinds de laatste beweging
	delta_time t_since_move_{};
	// weet hoe de koe getekend moet worden
	play::image_drawable drawable_;
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_ANDRE_HPP */
