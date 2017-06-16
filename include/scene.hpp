#include <memory>
#include <vector>
#include "bvh.hpp"
#include "camera.hpp"
#include "light.hpp"

#ifndef SCENE_HPP_
#define SCENE_HPP_

struct Scene {
  Camera camera;

  BVH bvh;

  std::vector<std::unique_ptr<Light>> lights;
};

#endif  // SCENE_HPP_
