#include <glm/glm.hpp>
#include "ray.hpp"

#ifndef BOUNDING_BOX_HPP_
#define BOUNDING_BOX_HPP_

struct BoundingBox {
  glm::vec3 min;

  glm::vec3 max;

  /**
   * Creates a bounding box encapsulating the origin.
   */
  BoundingBox();

  /**
   * Creates a bounding box encapsulating a single point.
   */
  explicit BoundingBox(const glm::vec3& center);

  /**
   * Expands the bounding box to contain the point.
   */
  void expand(const glm::vec3& point);

  /**
   * Expands the bounding box to contain the box.
   */
  void expand(const BoundingBox& box);

  /**
   * Returns the center of the bounding box.
   */
  glm::vec3 center() const;

  /**
   * Checks if the ray intersects with the bounding box.
   */
  bool intersects(const Ray& ray) const;
};

#endif  // BOUNDING_BOX_HPP_
