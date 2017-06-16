#include <glm/glm.hpp>
#include <memory>
#include "color.hpp"
#include "image.hpp"

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

/**
 * http://paulbourke.net/dataformats/mtl/
 * http://exocortex.com/blog/extending_wavefront_mtl_to_support_pbr
 */
struct Material {
  Color Ke;

  Color Ka;

  Color Kd;

  Color Ks;

  Color Kt;

  float Ni;

  float Pr;

  /**
   * Optional diffuse texture. See Config::Loader::textures to enable/disable.
   */
  std::shared_ptr<Image> Kd_texture;
};

#endif  // MATERIAL_HPP_
