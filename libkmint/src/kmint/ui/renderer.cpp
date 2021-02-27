#include "kmint/ui/renderer.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "kmint/ui/sdl/create.hpp"
#include "kmint/ui/sdl/texture.hpp"
#include "kmint/ui/sdl/utility.hpp"
#include <utility>

namespace kmint::ui {

namespace {
sdl::texture load_texture(std::string const &fn, SDL_Renderer *ren) {
  return {sdl::surface{fn}, ren};
}

SDL_Rect to_sdl_rect(math::rectangle rect) {
  SDL_Rect r;
  r.x = static_cast<int>(rect.top_left().x());
  r.y = static_cast<int>(rect.top_left().y());
  r.w = static_cast<int>(rect.size().width());
  r.h = static_cast<int>(rect.size().height());
  return r;
}

} // namespace

sdl::texture &renderer::get_or_load(graphics::image const &img) {
  auto const &fn = img.filename();
  if (auto i = cache_.find(fn); i != cache_.end()) {
    return i->second;
  }
  return cache_.emplace(std::make_pair(fn, load_texture(fn, ren_.get())))
      .first->second;
}

void renderer::render_color(graphics::color const &color) noexcept {
  sdl::apply_rgba(SDL_SetRenderDrawColor, color, ren_.get());
}

graphics::color renderer::render_color() const noexcept {
  Uint8 r, g, b, a;
  SDL_GetRenderDrawColor(ren_.get(), &r, &g, &b, &a);
  return {r, g, b, a};
}

void renderer::render_image(math::vector2d center, graphics::image const &img) {
  sdl::render(ren_.get(), get_or_load(img), center, img.scale(), img.rotation());
}

void renderer::render_image(math::vector2d center, graphics::image const &img,
                            graphics::color tint) {

  sdl::render(ren_.get(), get_or_load(img), center, img.scale(), tint, img.rotation());
}

void renderer::render_rectangle(math::rectangle rect) {
  SDL_Rect r = to_sdl_rect(rect);
  SDL_RenderFillRect(ren_.get(), &r);
}

void renderer::render_rectangle_outline(math::rectangle rect) {
  SDL_Rect r = to_sdl_rect(rect);
  SDL_RenderDrawRect(ren_.get(), &r);
}

void renderer::render_line(math::vector2d from, math::vector2d to) {
  SDL_RenderDrawLine(ren_.get(), from.x(), from.y(), to.x(), to.y());
}

void renderer::begin_frame() noexcept {
  render_color(graphics::colors::black);
  SDL_RenderClear(ren_.get());
}
void renderer::end_frame() noexcept { SDL_RenderPresent(ren_.get()); }
} // namespace kmint::ui
