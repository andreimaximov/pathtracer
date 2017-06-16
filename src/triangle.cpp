#include "triangle.hpp"
#include <glm/glm.hpp>
#include <limits>

Triangle::Triangle(Vertex a, Vertex b, Vertex c, size_t m,
                   SharedAttrib<glm::vec3> V, SharedAttrib<glm::vec3> N,
                   SharedAttrib<Material> M, SharedAttrib<glm::vec2> T)
    : a(a), b(b), c(c), m(m), V(V), N(N), M(M), T(T) {
  box = BoundingBox(vert(a.v));
  box.expand(vert(b.v));
  box.expand(vert(c.v));
}

Primitive::Intersection Triangle::intersects(const Ray& ray) const {
  glm::vec3 ab = vert(b.v) - vert(a.v);
  glm::vec3 ac = vert(c.v) - vert(a.v);

  // Check if ray is parallel to plane of triangle
  glm::vec3 N = glm::normalize(glm::cross(ab, ac));
  if (glm::abs(glm::dot(N, ray.D)) < 0.0001) {
    return NO_INTER();
  }

  // Calculate plane intersection point
  float t = glm::dot((vert(a.v) - ray.O), N) / glm::dot(ray.D, N);
  if (t < 0) {
    return NO_INTER();
  }
  glm::vec3 P = ray.at(t);

  // Calculate area of triangle
  float area = glm::length(glm::cross(ab, ac)) * 0.5;

  // Check ab side
  glm::vec3 x = glm::cross(ab, P - vert(a.v));
  float u = glm::length(x) / (2.0 * area);
  if (glm::dot(N, x) < 0) {
    return NO_INTER();
  }

  // Check ac side
  glm::vec3 z = glm::cross(ac, P - vert(c.v));
  float w = glm::length(z) / (2.0 * area);
  if (glm::dot(N, z) > 0) {
    return NO_INTER();
  }

  // Check bc side
  glm::vec3 y = glm::cross(vert(c.v) - vert(b.v), P - vert(b.v));
  float v = glm::length(y) / (2.0 * area);
  if (glm::dot(N, y) < 0) {
    return NO_INTER();
  }

  if (a.n != -1 && b.n != -1 && c.n != -1) {
    // Interpolate only if all vertex normals specified
    N = u * norm(c.n) + w * norm(b.n) + v * norm(a.n);
  } else if (glm::dot(N, ray.D) > 0) {
    // Otherwise flip direction if ray hitting opposite side
    N *= -1;
  }

  // Calculate texture coordinates
  glm::vec2 uv;
  if (has_texture_coords()) {
    uv = u * tex(c.t) + w * tex(b.t) + v * tex(a.t);
  }

  return Intersection{t, glm::normalize(N), uv};
}

const BoundingBox& Triangle::bounds() const {
  return box;
}

const Material& Triangle::material() const {
  return (*M.get())[m];
}

bool Triangle::has_texture_coords() const {
  return a.t != -1 && b.t != -1 && c.t != -1;
}

glm::vec3 Triangle::vert(size_t i) const {
  return (*V.get())[i];
}

glm::vec3 Triangle::norm(size_t i) const {
  return (*N.get())[i];
}

glm::vec2 Triangle::tex(size_t i) const {
  return (*T.get())[i];
}
