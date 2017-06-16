#include <random>
#include "color.hpp"
#include "config.hpp"
#include "ray.hpp"
#include "scene.hpp"

#ifndef PATHTRACER_HPP_
#define PATHTRACER_HPP_

class PathTracer {
 private:
  enum Shading { NONE, DIFF, REFL_ONLY, REFL_REFR };

  Config config;

  mutable std::mt19937 gen;

  Color trace(const Scene& scene, const Ray& ray, int depth,
              bool emission) const;

  /**
   * Generates a reflection ray.
   */
  glm::vec3 reflect(const glm::vec3& N, const glm::vec3& I,
                    float roughness) const;

  /**
   * Calculates the reflection factor using the fresnel formula.
   */
  float fresnel(const glm::vec3& N, const glm::vec3& I, float ior) const;

  /**
   * Returns the refraction direction of an incident ray. This only makes sense
   * if the ray does not experience total internal reflection which can be
   * checked with a call to fresnel(...).
   */
  glm::vec3 refract(const glm::vec3& N, const glm::vec3& I, float ior) const;

  /**
   * Returns the direct lighting contribution of directly sampling a light
   * source from position P with normal N. The contribution is scaled by the
   * PDF = 1 / |Lights| of a uniform sample.
   */
  Color direct_light_sample(const Scene& scene, const glm::vec3& P,
                            const glm::vec3& N) const;

 public:
  /**
   * Creates a configured path tracer.
   */
  explicit PathTracer(Config config);

  /**
   * Calculates the color of shooting this ray into the scene.
   */
  Color trace(const Scene& scene, const Ray& ray) const;
};

#endif  // PATHTRACER_HPP_
