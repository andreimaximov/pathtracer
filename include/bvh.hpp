#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "bounding-box.hpp"
#include "primitive.hpp"
#include "ray.hpp"

#ifndef BVH_HPP_
#define BVH_HPP_

/**
 * Bounding volume hierarchy of scene primitives.
 */
class BVH {
 public:
  struct Intersection : public Primitive::Intersection {
    /**
     * Pointer to the intersecting primitive.
     */
    Primitive* primitive = nullptr;
  };

  BVH();

  /**
   * Calculates the intersection of this ray with the scene.
   */
  Intersection intersect(const Ray& ray) const;

  /**
   * Returns bounding box of the entire scene.
   */
  BoundingBox get_bounds() const;

  /**
   * Creates a BVH, transferring ownership of the primitives to the tree.
   */
  static BVH build(std::vector<Primitive::SharedPtr>&& primitives);

 private:
  struct Node {
    using NodePtr = std::unique_ptr<Node>;

    /**
     * Bounding box needs to be recreated before expanding!
     */
    BoundingBox bounds;

    NodePtr left;

    NodePtr right;

    /**
     * The number of primitives under this nodes hierarchy.
     */
    size_t size = 0;

    /**
     * Contains all of the primitives that are stored DIRECTLY at this node.
     */
    std::vector<Primitive*> primitives;

    explicit Node(const BoundingBox& bounds);
  };

  enum Axis { X, Y, Z };

  /**
   * Max height of the BVH.
   */
  static constexpr int MAX_HEIGHT = 20;

  /**
   * Minimum number of primitives to split.
   */
  static constexpr int MIN_SIZE = 10;

  static std::shared_ptr<spdlog::logger> LOG;

  Node root;

  std::vector<Primitive::SharedPtr> primitives;

  /**
   * Creates a BVH with the given root node.
   */
  BVH(Node&& root, std::vector<Primitive::SharedPtr>&& primitives);

  /**
   * Calculates the intersection of this ray with the primitives in the node.
   */
  Intersection intersect(const Ray& ray, Node const* node) const;

  /**
   * Creates a sub-tree with a limited height containing the primitives.
   */
  static Node::NodePtr build(std::vector<Primitive*>& primitives, int height);

  /**
   * Calculates the widest axis on which to split the node.
   */
  static Axis get_split_axis(Node const* node);

  /**
   * Helper that returns the coordinate of v corresponding to the axis.
   */
  static float get(const glm::vec3& v, Axis axis);
};

#endif  // BVH_HPP_
