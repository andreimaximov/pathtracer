#include <glm/glm.hpp>

#ifndef RAY_HPP_
#define RAY_HPP_

struct Ray {
  glm::vec3 O;

  glm::vec3 D;

  Ray(const glm::vec3& O, const glm::vec3& D);

  glm::vec3 at(float t) const;
};

#endif  // RAY_HPP_
