#include "ray.hpp"

Ray::Ray(const glm::vec3& O, const glm::vec3& D) : O(O), D(glm::normalize(D)) {}

glm::vec3 Ray::at(float t) const {
  return O + D * t;
}
