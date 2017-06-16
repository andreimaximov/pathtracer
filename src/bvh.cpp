#include "bvh.hpp"
#include <algorithm>
#include <cassert>

std::shared_ptr<spdlog::logger> BVH::LOG = spdlog::stdout_color_mt("BVH");

/**
 * ============================================================
 *                             BVH
 * ============================================================
 */

BVH::BVH() : root(BoundingBox()) {}

BVH::BVH(Node&& root, std::vector<Primitive::SharedPtr>&& primitives)
    : root(std::move(root)), primitives(std::move(primitives)) {}

BVH::Intersection BVH::intersect(const Ray& ray) const {
  return intersect(ray, &root);
}

BVH::Intersection BVH::intersect(const Ray& ray, Node const* node) const {
  if (node == nullptr || !node->bounds.intersects(ray)) {
    return Intersection();
  } else if (!node->primitives.empty()) {
    // Test primitives!
    Intersection closest;

    for (const auto primitive : node->primitives) {
      auto inter = primitive->intersects(ray);
      if (inter && (!closest || inter.t < closest.t)) {
        closest.t = inter.t;
        closest.N = inter.N;
        closest.uv = inter.uv;
        closest.primitive = primitive;
      }
    }

    return closest;
  }

  // Choose the closest intersection.
  Intersection closest;
  auto left = intersect(ray, node->left.get());
  auto right = intersect(ray, node->right.get());

  if (left && right) {
    closest = (left.t < right.t) ? left : right;
  } else if (!left) {
    closest = right;
  } else if (!right) {
    closest = left;
  }

  return closest;
}

BoundingBox BVH::get_bounds() const {
  return root.bounds;
}

BVH BVH::build(std::vector<Primitive::SharedPtr>&& primitives) {
  // Create the node before std::move(primitive)!
  std::vector<Primitive*> naked(primitives.size());
  for (size_t i = 0; i < primitives.size(); i++) {
    naked[i] = primitives[i].get();
  }
  auto node = build(naked, MAX_HEIGHT);
  return BVH(std::move(*node), std::move(primitives));
}

//
// Based on:
// https://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
//
BVH::Node::NodePtr BVH::build(std::vector<Primitive*>& primitives, int height) {
  // Handle case of no primitives.
  if (primitives.empty()) {
    return BVH::Node::NodePtr(nullptr);
  }

  // Setup node bounding box.
  auto node = BVH::Node::NodePtr(new BVH::Node(primitives[0]->bounds()));
  for (const auto primitive : primitives) {
    node->bounds.expand(primitive->bounds());
  }

  if (height == 0 || primitives.size() < MIN_SIZE) {
    // Dump all primitives into leaf node.
    for (const auto primitive : primitives) {
      node->primitives.push_back(primitive);
    }
    node->size = node->primitives.size();
    LOG->debug("Created node with {:d} primitives.", node->size);
    return node;
  }

  // Split primitives along widest axis.
  Axis axis = get_split_axis(node.get());
  float mid = get(node->bounds.center(), axis);

  // Partition.
  auto predicate = [mid, axis](const Primitive* prim) {
    return get(prim->bounds().center(), axis) < mid;
  };

  auto split = std::partition(primitives.begin(), primitives.end(), predicate);

  // If the partition did not work just sort by mid-point of widest
  // axis and split set in halt.
  if (split == primitives.begin() || split == primitives.end()) {
    split = primitives.begin() + primitives.size() / 2;

    auto comparator = [axis](Primitive* lhs, Primitive* rhs) {
      return get(lhs->bounds().center(), axis) <
             get(rhs->bounds().center(), axis);
    };

    std::sort(primitives.begin(), primitives.end(), comparator);
  }

  std::vector<Primitive*> left(primitives.begin(), split);
  std::vector<Primitive*> right(split, primitives.end());

  // Ensure we aren't adding depth pointlessly...
  assert(!left.empty() && !right.empty());

  if (!left.empty()) {
    node->left = build(left, height - 1);
    node->size += node->left->size;
  }

  if (!right.empty()) {
    node->right = build(right, height - 1);
    node->size += node->right->size;
  }

  LOG->debug("Created node with {:d} primitives.", node->size);

  return node;
}

BVH::Axis BVH::get_split_axis(Node const* node) {
  Axis axis = Axis::X;
  float widest = node->bounds.max.x - node->bounds.min.x;

  float wy = node->bounds.max.y - node->bounds.min.y;
  if (wy > widest) {
    axis = Axis::Y;
    widest = wy;
  }

  float wz = node->bounds.max.z - node->bounds.min.z;
  if (wz > widest) {
    axis = Axis::Z;
  }

  return axis;
}

float BVH::get(const glm::vec3& v, Axis axis) {
  switch (axis) {
    case Axis::X:
      return v.x;
    case Axis::Y:
      return v.y;
    case Axis::Z:
      return v.z;
  }
}

/**
 * ============================================================
 *                          BVH::Node
 * ============================================================
 */

BVH::Node::Node(const BoundingBox& bounds) : bounds(bounds) {}
