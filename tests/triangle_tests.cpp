#include <catch.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "ray.hpp"
#include "triangle.hpp"

TEST_CASE("Triangle intersection is correct", "[intersection]") {
  auto V = std::make_shared<std::vector<glm::vec3>>();
  V->push_back(glm::vec3(0, 0, 0));
  V->push_back(glm::vec3(0, 2, 0));
  V->push_back(glm::vec3(2, 0, 0));

  auto N = std::make_shared<std::vector<glm::vec3>>();
  N->push_back(glm::vec3(0, 0, -1));

  auto M = std::make_shared<std::vector<Material>>();
  M->push_back(Material());

  auto T = std::make_shared<std::vector<glm::vec2>>();

  Vertex a{0, 0, -1};
  Vertex b{1, 0, -1};
  Vertex c{2, 0, -1};

  Triangle triangle(a, b, c, 0, V, N, M, T);

  SECTION("parallel ray does not intersect") {
    Ray ray(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
    REQUIRE(!triangle.intersects(ray));
  }

  SECTION("out of bounds ray does not intersect") {
    // Intersects plane at (-1, -1, 0) but not in triangle
    Ray rayOne(glm::vec3(-2, -2, -1), glm::vec3(1, 1, 1));
    auto interOne = triangle.intersects(rayOne);
    REQUIRE(!interOne);

    // Intersects plane at (2, 2, 0) but not in triangle
    Ray rayTwo(glm::vec3(1, 1, -1), glm::vec3(1, 1, 1));
    auto interTwo = triangle.intersects(rayTwo);
    REQUIRE(!interTwo);
  }

  SECTION("in bounds ray intersects from bottom") {
    // Intersects plane at (1, 1, 0) inside triangle
    Ray ray(glm::vec3(0, 0, -1), glm::vec3(1, 1, 1));
    auto inter = triangle.intersects(ray);
    REQUIRE(inter);

    glm::vec3 P = ray.at(inter.t);

    REQUIRE(P.x == Approx(1));
    REQUIRE(P.y == Approx(1));
    REQUIRE(P.z == Approx(0));

    REQUIRE(inter.N.x == Approx(0));
    REQUIRE(inter.N.y == Approx(0));
    REQUIRE(inter.N.z == Approx(-1));
  }

  SECTION("in bounds ray intersects from top") {
    // Intersects plane at (1, 1, 0) inside triangle
    Ray ray(glm::vec3(0, 0, 1), glm::vec3(1, 1, -1));
    auto inter = triangle.intersects(ray);
    REQUIRE(inter);

    glm::vec3 P = ray.at(inter.t);

    REQUIRE(P.x == Approx(1));
    REQUIRE(P.y == Approx(1));
    REQUIRE(P.z == Approx(0));

    REQUIRE(inter.N.x == Approx(0));
    REQUIRE(inter.N.y == Approx(0));
    REQUIRE(inter.N.z == Approx(-1));
  }
}
