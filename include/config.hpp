#include <glm/glm.hpp>
#include <json.hpp>
#include "color.hpp"

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

struct Config {
  struct Camera {
    /**
     * Width in pixels of render.
     */
    int width;

    /**
     * Height in pixels of render.
     */
    int height;

    /**
     * The scene focus point of the camera
     */
    glm::vec3 center;

    /**
     * The position of the camera in scene coordinates.
     */
    glm::vec3 position;

    /**
     * The camera orientation up vector.
     */
    glm::vec3 up;

    /**
     * The verticle field of view in degrees of the camera.
     */
    float fovy;
  };

  struct Job {
    /**
     * The number of threads to use for rendering.
     */
    int threads;

    /**
     * The number of partitions to split the image into for rendering.
     */
    int partitions;
  };

  struct Rendering {
    /**
     * The ray depth before Russian Roulette path termination begins.
     */
    int bounces;

    /**
     * The number of samples for each pixel.
     */
    int samples;

    /**
     * The floating point correction factor for intersections. This is used to
     * ensure recursive rays completely pass through surface or bounce off
     * without immediately hitting the same surface due to floating point error.
     */
    float epsilon;

    /**
     * The uniform scene background color that acts as environment lighting.
     */
    Color background;
  };

  struct Loader {
    /**
     * Specifies if diffuse textures should be loaded.
     */
    bool textures;

    /**
     * Specifies if provided geometry normals should be loaded. If false flat
     * shading is calculated. This is useful in case the specified normals are
     * inconsistent.
     */
    bool normals;
  };

  struct Debug {
    /**
     * Render the geometry normals only.
     */
    bool normals;

    /**
     * Render the diffuse color only.
     */
    bool diffuse;
  };

  Camera camera;

  Job job;

  Rendering rendering;

  Loader loader;

  Debug debug;
};

/**
 * Converts JSON to a Config.
 */
void from_json(const nlohmann::json& json, Config& config);

namespace nlohmann {
/**
 * glm::vec3 JSON serializer
 */
template <>
struct adl_serializer<glm::vec3> {
  static void to_json(json& j, const glm::vec3& v);

  static void from_json(const json& j, glm::vec3& v);
};

/**
 * Color serializer
 */
template <>
struct adl_serializer<Color> {
  static void to_json(json& j, const Color& c);

  static void from_json(const json& j, Color& c);
};
}  // namespace nlohmann

#endif  // CONFIG_HPP_
