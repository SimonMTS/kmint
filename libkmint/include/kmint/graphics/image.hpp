#ifndef KMINT_GRAPHICS_IMAGE_HPP
#define KMINT_GRAPHICS_IMAGE_HPP

#include "kmint/math/angle.hpp"
#include "kmint/primitives.hpp"
#include <string>
#include <utility>

namespace kmint {
namespace graphics {

class image {
public:
  explicit image(std::string image_filename, scalar scale = scalar(1.0),
                 math::angle rotation = math::angle::from_degrees(0))
      : filename_{std::move(image_filename)}, scale_{scale}, rotation_{
                                                                 rotation} {}
  std::string const &filename() const noexcept { return filename_; }
  scalar scale() const noexcept { return scale_; }
  void scale(scalar new_scale) noexcept { scale_ = new_scale; }
  math::angle rotation() const noexcept { return rotation_; }
  void rotation(math::angle r) { rotation_ = r; }

private:
  std::string filename_;
  scalar scale_;
  math::angle rotation_;
};

} // namespace graphics
} // namespace kmint

#endif
