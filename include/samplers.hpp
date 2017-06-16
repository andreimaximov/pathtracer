#include <cassert>
#include <glm/glm.hpp>
#include <random>

#ifndef SAMPLERS_HPP_
#define SAMPLERS_HPP_

namespace samplers {
constexpr float PI = 3.14159f;

using glm::vec3;

/**
 * Generates a vector in a cosine weighted hemisphere with normal N using
 * specific values of s and t.
 *
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation
 * https://cseweb.ucsd.edu/classes/sp17/cse168-a/CSE168_07_Random.pdf
 */
inline vec3 cos_weighted_hemi(const vec3& N, float s, float t) {
  assert(s >= 0 && s <= 1);
  assert(t >= 0 && t <= 1);

  // Create coordinate frame for normal N.
  vec3 X;

  if (glm::abs(N.x) > std::abs(N.y)) {
    X = vec3(N.z, 0, -N.x) / glm::sqrt(N.x * N.x + N.z * N.z);
  } else {
    X = vec3(0, -N.z, N.y) / glm::sqrt(N.y * N.y + N.z * N.z);
  }

  vec3 Z = glm::cross(N, X);

  assert(glm::dot(X, Z) < 0.0001);
  assert(glm::dot(X, N) < 0.0001);
  assert(glm::dot(Z, N) < 0.0001);

  // Generate a sample from cos weighted hemisphere with normal (0, 1, 0).
  float u = 2.0f * PI * s;
  float v = std::sqrt(1 - t);

  vec3 V(v * std::cos(u), std::sqrt(t), v * std::sin(u));

  // Project onto hemisphere defined by N.
  return X * V.x + N * V.y + Z * V.z;
}

/**
 * Generates a random vector in a cosine weighted hemisphere with normal N.
 *
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation
 * https://cseweb.ucsd.edu/classes/sp17/cse168-a/CSE168_07_Random.pdf
 */
template <typename RNG>
inline vec3 cos_weighted_hemi(const vec3& N, RNG& rng) {
  static std::uniform_real_distribution<float> dist(0, 1);
  return cos_weighted_hemi(N, dist(rng), dist(rng));
}

/**
 * Cosine weighted hemisphere sample with adjustable variance. The variance
 * factor should be in the range [0, 1] and is used to control how clustered
 * towards the center of the hemisphere the generated vectors are.
 */
template <typename RNG>
inline vec3 var_cos_weighted_hemi(const vec3& N, float var, RNG& rng) {
  static std::uniform_real_distribution<float> dist(0, 1);

  assert(var >= 0 && var <= 1);

  float t = 1 - dist(rng) * var;
  return cos_weighted_hemi(N, dist(rng), t);
}

/**
 * Uniformly samples a point from triangle ABC using rejection sampling.
 *
 * http://www.joesfer.com/?p=84
 */
template <typename RNG>
inline vec3 triangle(const vec3& A, const vec3& B, const vec3& C, RNG& rng) {
  static std::uniform_real_distribution<float> dist(0, 1);

  float u, v;

  do {
    u = dist(rng);
    v = dist(rng);
  } while (u + v > 1);

  float w = 1 - u - v;

  vec3 P = A * u + B * v + C * w;

#ifndef NDEBUG
  vec3 AB = (A + B) * 0.5f;
  vec3 AC = (A + C) * 0.5f;
  vec3 BC = (B + C) * 0.5f;

  assert(glm::dot(AB - C, P - C) >= 0);
  assert(glm::dot(AC - B, P - B) >= 0);
  assert(glm::dot(BC - A, P - A) >= 0);
#endif

  return P;
}
}  // namespace samplers

#endif  // SAMPLERS_HPP_
