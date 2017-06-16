#include "pathtracer.hpp"
#include <algorithm>
#include <cassert>
#include <glm/gtc/constants.hpp>
#include <limits>
#include "samplers.hpp"

PathTracer::PathTracer(Config config) : config(std::move(config)) {
  std::random_device rd;
  this->gen = std::mt19937(rd());
}

Color PathTracer::trace(const Scene& scene, const Ray& ray) const {
  return trace(scene, ray, 0, true);
}

Color PathTracer::trace(const Scene& scene, const Ray& ray, int depth,
                        bool emission) const {
  static std::uniform_real_distribution<float> fdist(0.0f, 1.01f);

  auto inter = scene.bvh.intersect(ray);
  if (!inter) {
    return config.rendering.background;
  }

  const Material& mat = inter.primitive->material();

  // Check if we are using a diffuse texture.
  bool use_texture = (inter.primitive->has_texture_coords() && mat.Kd_texture);

  Color Kd = use_texture ? mat.Kd_texture->get_pixel_uv(inter.uv.x, inter.uv.y)
                         : mat.Kd;

  if (config.debug.normals) {
    auto N = (inter.N + 1.f) * 0.5f;
    return Color(N.x, N.y, N.z);
  } else if (config.debug.diffuse && !Kd.isTransparent()) {
    return Kd;
  } else if (!mat.Ke.isBlack()) {
    return emission ? mat.Ke : Color::BLACK;
  }

  Shading type = Shading::NONE;

  // Determine the type of shading.
  if (!Kd.isBlack() || use_texture) {
    type = Shading::DIFF;
  } else if (!mat.Ks.isBlack() && mat.Kt.isBlack()) {
    type = Shading::REFL_ONLY;
  } else if (!mat.Ks.isBlack()) {
    type = Shading::REFL_REFR;
  } else {
    return Color::BLACK;
  }

  float p = 1.0f;

  // Russian Roulette termination - use the average color intensity as
  // termination probability. Based on:
  // https://inst.eecs.berkeley.edu/~cs294-13/fa09/lectures/scribe-lecture5.pdf
  if (depth > config.rendering.bounces) {
    // The probability of termination is inversely proporional to the material
    // intensity.
    switch (type) {
      case Shading::DIFF:
        p = Kd.max();
        break;
      case Shading::REFL_ONLY:
        p = mat.Ks.max();
        break;
      case Shading::REFL_REFR:
        p = std::max(mat.Ks.max(), mat.Kt.max());
        break;
      default:
        assert(false);
    }

    // Prevent infinite recursion for intense materials.
    p = depth > config.rendering.bounces * 2 ? 0.0f : p;

    if (fdist(gen) < p) {
      p = 1.0f / p;
    } else {
      return Color::BLACK;
    }
  }

  glm::vec3 P = ray.at(inter.t);
  glm::vec3 N = glm::dot(inter.N, ray.D) < 0 ? inter.N : -inter.N;
  glm::vec3 O = P + config.rendering.epsilon * N;

  // If we encounter a transparent texture, continue shooting the ray through
  // with a 50% chance of depth increase so that we do not recurse infinitely.
  if (Kd.isTransparent() && use_texture) {
    Ray through(P + ray.D * config.rendering.epsilon, ray.D);
    return trace(scene, through, depth + fdist(gen) * 2, emission);
  }

  // Directly sample a light for diffuse surfaces.
  Color Li =
      (type == Shading::DIFF) ? direct_light_sample(scene, O, N) : Color::BLACK;

  if (type == Shading::DIFF) {
    glm::vec3 D = samplers::cos_weighted_hemi(inter.N, gen);
    return Kd * (Li + trace(scene, Ray(O, D), depth + 1, false) * p);
  } else if (type == Shading::REFL_ONLY) {
    glm::vec3 R = reflect(inter.N, ray.D, mat.Pr);
    return mat.Ks * p * trace(scene, Ray(O, R), depth + 1, true);
  } else if (type == Shading::REFL_REFR) {
    glm::vec3 R = reflect(inter.N, ray.D, mat.Pr);
    float kr = fresnel(inter.N, ray.D, mat.Ni);

    // Probabilistically chose between reflection and refraction based on
    // fresnel factor.
    if (fdist(gen) < kr) {
      return mat.Ks * p * trace(scene, Ray(O, R), depth + 1, true);
    } else {
      glm::vec3 O = P - config.rendering.epsilon * N;
      glm::vec3 T = refract(inter.N, ray.D, mat.Ni);
      return mat.Kt * p * trace(scene, Ray(O, T), depth + 1, true);
    }
  }

  return Color::BLACK;
}

glm::vec3 PathTracer::reflect(const glm::vec3& N, const glm::vec3& I,
                              float roughness) const {
  // Generate a perfect reflection direction. This will be used as the
  // normal for the hemisphere sampling.
  glm::vec3 D = I - 2.0f * N * glm::dot(I, N);
  return samplers::var_cos_weighted_hemi(D, roughness, gen);
}

/**
 * Implementation based on tutorial from Scratchapixel. Note, the math on
 * Scratchapixel is correct but the same implementation is wrong. I used
 * Wikipedia to verify my implementation.
 *
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
 * https://en.wikipedia.org/wiki/Fresnel_equations
 */
float PathTracer::fresnel(const glm::vec3& N, const glm::vec3& I,
                          float ior) const {
  bool isExit = glm::dot(I, N) > 0;

  float cosi = glm::abs(glm::dot(I, N));
  float etai = isExit ? ior : 1;
  float etat = isExit ? 1 : ior;

  float k = 1 - etai * etai * (1.0 - cosi * cosi);

  if (k < 0) {
    return 1;  // Total internal reflection!
  }

  assert(cosi >= -1 && cosi <= 1);

  float sint = (etai / etat) * glm::sqrt((1 - cosi * cosi));
  float cost = glm::sqrt(1 - sint * sint);

  float fprp = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
  float fpar = (etat * cost - etai * cosi) / (etat * cost + etai * cosi);

  float kr = 0.5 * (fprp * fprp + fpar * fpar);

  return kr;
}

glm::vec3 PathTracer::refract(const glm::vec3& N, const glm::vec3& I,
                              float ior) const {
  bool isExit = glm::dot(N, I) > 0;

  glm::vec3 N_ = isExit ? -N : N;
  float etai = isExit ? ior : 1 / ior;

  return glm::refract(I, N_, etai);
}

Color PathTracer::direct_light_sample(const Scene& scene, const glm::vec3& P,
                                      const glm::vec3& N) const {
  static std::uniform_int_distribution<int> idist(
      0, std::numeric_limits<int>::max());

  if (scene.lights.empty()) {
    return Color::BLACK;
  }

  int i = idist(gen) % scene.lights.size();
  auto sample = scene.lights[i]->sample(P, gen);
  auto D = glm::normalize(sample.P - P);
  float cos = glm::dot(N, D);

  // Throw this sample out if it is behind the normal.
  if (cos < 0) {
    return Color::BLACK;
  }

  auto shadow = scene.bvh.intersect(Ray(P, D));
  auto dist = glm::distance(sample.P, P);

  if (!shadow || shadow.t + config.rendering.epsilon > dist) {
    return sample.color * glm::dot(N, D) * scene.lights.size();
  } else {
    return Color::BLACK;
  }
}
