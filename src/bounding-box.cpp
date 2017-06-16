#include "bounding-box.hpp"
#include <algorithm>

BoundingBox::BoundingBox() : min(glm::vec3(0, 0, 0)), max(glm::vec3(0, 0, 0)) {}

BoundingBox::BoundingBox(const glm::vec3& center) : min(center), max(center) {}

void BoundingBox::expand(const BoundingBox& box) {
  min.x = std::min(min.x, box.min.x);
  min.y = std::min(min.y, box.min.y);
  min.z = std::min(min.z, box.min.z);
  max.x = std::max(max.x, box.max.x);
  max.y = std::max(max.y, box.max.y);
  max.z = std::max(max.z, box.max.z);
}

void BoundingBox::expand(const glm::vec3& point) {
  min.x = std::min(min.x, point.x);
  min.y = std::min(min.y, point.y);
  min.z = std::min(min.z, point.z);
  max.x = std::max(max.x, point.x);
  max.y = std::max(max.y, point.y);
  max.z = std::max(max.z, point.z);
}

glm::vec3 BoundingBox::center() const {
  float mx = min.x + (max.x - min.x) * 0.5;
  float my = min.y + (max.y - min.y) * 0.5;
  float mz = min.z + (max.z - min.z) * 0.5;
  return glm::vec3(mx, my, mz);
}

/**
 * Based on:
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
 */
bool BoundingBox::intersects(const Ray& ray) const {
  // Calculate inverse direction once to avoid expensive division.
  float dxinv = 1.0f / ray.D.x;
  float dyinv = 1.0f / ray.D.y;
  float dzinv = 1.0f / ray.D.z;

  // Calculate intersection parametrics of x and y planes
  float txmin = (min.x - ray.O.x) * dxinv;
  float txmax = (max.x - ray.O.x) * dxinv;

  float tymin = (min.y - ray.O.y) * dyinv;
  float tymax = (max.y - ray.O.y) * dyinv;

  // Handle negative directions when ray origin inside bounding box.
  if (txmin > txmax) {
    std::swap(txmin, txmax);
  }

  if (tymin > tymax) {
    std::swap(tymin, tymax);
  }

  if (txmin > tymax || tymin > txmax) {
    return false;
  }

  float tmin = std::min(txmin, tymin);
  float tmax = std::min(txmax, tymax);

  // Continue with similar check for z-axis.
  float tzmin = (min.z - ray.O.z) * dzinv;
  float tzmax = (max.z - ray.O.z) * dzinv;

  // Handle negative directions when ray origin inside bounding box (again).
  if (tzmin > tzmax) {
    std::swap(tzmin, tzmax);
  }

  if (tzmin > tmax || tmin > txmax) {
    return false;
  }

  return true;
}
