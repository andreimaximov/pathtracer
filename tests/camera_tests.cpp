#include "camera.hpp"
#include "catch.hpp"
#include "glm/glm.hpp"

TEST_CASE("Ray is correct", "[pixel_ray]") {
  constexpr float EPS = 0.15;

  Camera camera;
  camera.P = glm::vec3(1, 2, 3);
  camera.U = glm::vec3(1, 0, 0);
  camera.V = glm::vec3(0, 1, 0);
  camera.W = glm::vec3(0, 0, -1);
  camera.fovy = glm::radians(90.0);
  camera.width = 5;
  camera.height = 5;

  SECTION("ray through center is correct") {
    Ray ray = camera.pixel_ray(2, 2);
    REQUIRE(ray.O == camera.P);
    REQUIRE(ray.D.x == Approx(0).epsilon(EPS));
    REQUIRE(ray.D.y == Approx(0).epsilon(EPS));
    REQUIRE(ray.D.z == Approx(-1).epsilon(EPS));
  }

  SECTION("ray through top left is correct") {
    Ray ray = camera.pixel_ray(0, 0);

    float d = glm::sqrt(2 * 2 + 2 * 2 + 2.5 * 2.5);
    REQUIRE(ray.O == camera.P);
    REQUIRE(ray.D.x == Approx(-2.0 / d).epsilon(EPS));
    REQUIRE(ray.D.y == Approx(+2.0 / d).epsilon(EPS));
    REQUIRE(ray.D.z == Approx(-2.5 / d).epsilon(EPS));
  }

  SECTION("ray through bottom right is correct") {
    Ray ray = camera.pixel_ray(4, 4);

    float d = glm::sqrt(2 * 2 + 2 * 2 + 2.5 * 2.5);
    REQUIRE(ray.O == camera.P);
    REQUIRE(ray.D.x == Approx(+2.0 / d).epsilon(EPS));
    REQUIRE(ray.D.y == Approx(-2.0 / d).epsilon(EPS));
    REQUIRE(ray.D.z == Approx(-2.5 / d).epsilon(EPS));
  }
}
