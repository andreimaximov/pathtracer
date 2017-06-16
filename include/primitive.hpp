#include <memory>
#include "bounding-box.hpp"
#include "glm/glm.hpp"
#include "material.hpp"
#include "ray.hpp"

#ifndef PRIMITIVE_HPP_
#define PRIMITIVE_HPP_

class Primitive {
 public:
  struct Intersection {
    /**
     * The distance to the intersection point. Negative if intersection DNE.
     */
    float t = -1;

    /**
     * The intersection normal.
     */
    glm::vec3 N;

    /**
     * The intersection texture coordinate.
     */
    glm::vec2 uv;

    /**
     * Checks if the intersection exists.
     */
    operator bool() const;
  };

  using NO_INTER = Intersection;

  using SharedPtr = std::shared_ptr<Primitive>;

  virtual ~Primitive() = default;

  /**
   * Returns the intersection result of this primitive and the ray.
   */
  virtual Intersection intersects(const Ray& ray) const = 0;

  /**
   * Returns a reference to the AABB of this primitive.
   */
  virtual const BoundingBox& bounds() const = 0;

  /**
   * Returns a reference to the material of this primitive.
   */
  virtual const Material& material() const = 0;

  /**
   * Returns a boolean indicating if this primitive has texture coordinates.
   */
  virtual bool has_texture_coords() const = 0;
};

#endif  // PRIMITIVE_HPP_
