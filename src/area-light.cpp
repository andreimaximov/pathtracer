#include "area-light.hpp"
#include <algorithm>
#include "samplers.hpp"

AreaLight::AreaLight(std::shared_ptr<Triangle> T) : T(T) {}

Light::Sample AreaLight::sample(const glm::vec3& P, Light::RNG& rng) const {
  glm::vec3 A = T->vert(T->a.v);
  glm::vec3 B = T->vert(T->b.v);
  glm::vec3 C = T->vert(T->c.v);

  glm::vec3 X = samplers::triangle(A, B, C, rng);

  glm::vec3 N = glm::normalize(glm::cross(A - B, A - C));
  glm::vec3 D = glm::normalize(P - X);

  if (glm::dot(N, D) < 0) {
    N = -N;
  }

  float r = glm::distance(P, X);
  float cos = glm::dot(D, N);

#ifndef NDEBUG
  assert(cos >= 0);
#endif

  glm::vec3 AB = A - B;
  glm::vec3 AC = A - C;
  float area = glm::dot(AB, AC) * glm::length(AB) * glm::length(AC);

  Color Ke = T->material().Ke;
  Color I = Ke * ((cos * area) / (r * r));

  // Hack - Ensure the color doesn't blow up because this causes very rare but
  // random artifacts if we are unfortunate enough to sample a point VERY close
  // to an area light.
  if (I.max() > Ke.max()) {
    I = I * (Ke.max() / I.max());
  }

  return Sample{X, I};
}
