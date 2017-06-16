#include <memory>
#include "light.hpp"
#include "triangle.hpp"

#ifndef AREA_LIGHT_HPP_
#define AREA_LIGHT_HPP_

class AreaLight : public Light {
 private:
  std::shared_ptr<Triangle> T;

 public:
  explicit AreaLight(std::shared_ptr<Triangle> T);

  /**
   * Returns a uniformly sampled point on the underlying triangle with the color
   * intensity divided by the PDF as described in these slides...
   *
   * https://inst.eecs.berkeley.edu/~cs283/sp13/lectures/283-lecture10.pdf
   */
  Sample sample(const glm::vec3& P, Light::RNG& rng) const override;
};

#endif  // AREA_LIGHT_HPP_
