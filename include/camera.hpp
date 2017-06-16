#include <glm/glm.hpp>
#include <random>
#include "ray.hpp"

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

struct Camera {
  glm::vec3 U;  // x-axis

  glm::vec3 V;  // y-axis

  glm::vec3 W;  // z-axis

  glm::vec3 P;  // Position

  float fovy;  // Field of view in radians

  int width;  // Image width

  int height;  // Image height

  mutable std::mt19937 gen;

  Camera();

  /**
   * Sets up the camera to point towards C from the position P oriented with
   * up vector U.
   */
  void set_position(glm::vec3 P, glm::vec3 C, glm::vec3 U);

  /**
   * Sets up the vamera resolution and field of view.
   */
  void set_view(float fovy, int width, int height);

  /**
   * Returns a ray through the (x, y) pixel.
   */
  Ray pixel_ray(int x, int y) const;
};

#endif  // CAMERA_HPP_
