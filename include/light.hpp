#include <glm/glm.hpp>
#include <random>
#include "color.hpp"

#ifndef LIGHT_HPP_
#define LIGHT_HPP_

class Light {
 public:
  struct Sample {
    glm::vec3 P;

    Color color;
  };

  using RNG = std::mt19937;

  virtual ~Light() = default;

  /**
   * Samples the contribution of the light from point P.
   */
  virtual Sample sample(const glm::vec3& P, RNG& rng) const = 0;
};

#endif  // LIGHT_HPP_
