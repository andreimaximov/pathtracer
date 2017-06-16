#include "config.hpp"
#include <array>

void from_json(const nlohmann::json& json, Config& config) {
  config.camera.width = json["camera"]["width"].get<int>();
  config.camera.height = json["camera"]["height"].get<int>();
  config.camera.center = json["camera"]["center"].get<glm::vec3>();
  config.camera.position = json["camera"]["position"].get<glm::vec3>();
  config.camera.up = json["camera"]["up"].get<glm::vec3>();
  config.camera.fovy = json["camera"]["fovy"].get<float>();

  config.job.threads = json["job"]["threads"].get<int>();
  config.job.partitions = json["job"]["partitions"].get<int>();

  config.rendering.bounces = json["rendering"]["bounces"].get<int>();
  config.rendering.samples = json["rendering"]["samples"].get<int>();
  config.rendering.epsilon = json["rendering"]["epsilon"].get<float>();
  config.rendering.background = json["rendering"]["background"].get<Color>();

  config.loader.textures = json["loader"]["textures"].get<bool>();
  config.loader.normals = json["loader"]["normals"].get<bool>();

  config.debug.normals = json["debug"]["normals"].get<bool>();
  config.debug.diffuse = json["debug"]["diffuse"].get<bool>();
}

namespace nlohmann {
void adl_serializer<glm::vec3>::to_json(json& j, const glm::vec3& v) {
  j = json(std::array<float, 3>({v.x, v.y, v.z}));
}

void adl_serializer<glm::vec3>::from_json(const json& j, glm::vec3& v) {
  v.x = j[0];
  v.y = j[1];
  v.z = j[2];
}

void adl_serializer<Color>::to_json(json& j, const Color& c) {
  j = json(std::array<float, 3>({c.r, c.g, c.b}));
}

void adl_serializer<Color>::from_json(const json& j, Color& c) {
  c.r = j[0];
  c.g = j[1];
  c.b = j[2];
}
}  // namespace nlohmann
