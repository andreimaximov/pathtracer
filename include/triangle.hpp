#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "material.hpp"
#include "primitive.hpp"
#include "vertex.hpp"

#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

class Triangle : public Primitive {
 public:
  template <typename T>
  using SharedAttrib = std::shared_ptr<std::vector<T>>;

  Vertex a;

  Vertex b;

  Vertex c;

  size_t m;

  SharedAttrib<glm::vec3> V;

  SharedAttrib<glm::vec3> N;

  SharedAttrib<Material> M;

  SharedAttrib<glm::vec2> T;

  BoundingBox box;

  Triangle(Vertex a, Vertex b, Vertex c, size_t m, SharedAttrib<glm::vec3> V,
           SharedAttrib<glm::vec3> N, SharedAttrib<Material> M,
           SharedAttrib<glm::vec2> T);

  Intersection intersects(const Ray& ray) const override;

  const BoundingBox& bounds() const override;

  const Material& material() const override;

  bool has_texture_coords() const override;

  glm::vec3 vert(size_t i) const;

  glm::vec3 norm(size_t i) const;

  glm::vec2 tex(size_t i) const;
};

#endif  // TRIANGLE_HPP_
