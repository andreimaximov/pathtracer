#include "camera.hpp"

Camera::Camera() {
  std::random_device rd;
  this->gen = std::mt19937(rd());
}

void Camera::set_position(glm::vec3 P, glm::vec3 C, glm::vec3 U) {
  this->P = P;

  this->W = glm::normalize(C - this->P);
  this->U = glm::normalize(glm::cross(this->W, U));
  this->V = glm::normalize(glm::cross(this->U, this->W));
}

void Camera::set_view(float fovy, int width, int height) {
  this->fovy = fovy;
  this->width = width;
  this->height = height;
}

/**
 * Tent-filter anti-aliasing based on inverse of tent CDF.
 * http://blog.mir.dlang.io/random/2016/08/19/intro-to-random-sampling.html
 */
Ray Camera::pixel_ray(int x, int y) const {
  static std::uniform_real_distribution<float> dist(0, 1);

  // tan(fovy * 0.5) = (height / 2) / d -> d = height / (2 * tan(fovy * 0.5))
  float d = height / (2 * glm::tan(fovy * 0.5));

  // tan(fovx / 2) = (width / 2) / d -> fovx = 2 * atan(width / (2 * d))
  float fovx = 2 * glm::atan(width / (2 * d));

  float hw = width / 2.0;
  float hh = height / 2.0;

  // There are many rays that go through a pixel - choose one randomly
  // (weighted at center) for an aliasing effect.
  float u = 2 * dist(gen);
  float v = 2 * dist(gen);

  // The square root skews the samples towards the center. CDF of tent is
  // C(x) = x^2 so inverse is sqrt(x). Symmetry can be applied to opposite
  // sides.
  float xr = (u < 1 ? glm::sqrt(u) - 1 : 1 - glm::sqrt(2 - u)) * 0.5f;
  float yr = (v < 1 ? glm::sqrt(v) - 1 : 1 - glm::sqrt(2 - v)) * 0.5f;

  float a = (glm::tan(fovx * 0.5) * (x + 0.5 + xr - hw)) / hw;
  float b = (glm::tan(fovy * 0.5) * (hh - 0.5 - y - yr)) / hh;

  glm::vec3 D = glm::normalize(a * U + b * V + W);

  return Ray(P, D);
}
