#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "color.hpp"
#include "config.hpp"
#include "image-loader.hpp"
#include "scene.hpp"

#ifndef PARSER_HPP_
#define PARSER_HPP_

class Parser {
 private:
  Config config;

  ImageLoader loader;

  std::unordered_map<std::string, std::shared_ptr<Image>> textures;

  static std::shared_ptr<spdlog::logger> LOG;

  static Color to_color(const tinyobj::real_t* color);

  static glm::vec3 to_vec(const tinyobj::real_t* vec);

 public:
  explicit Parser(Config config);

  /**
   * Parses an OBJ file into a scene object. The OBJ material loaders is
   * finicky so ensure the material base directory contains a trailing slash.
   */
  Scene parse(std::string scene_file,
              std::string materials_path) throw(std::logic_error);

  /**
   * Loads a texture into the cache and returns a pointer to the texture.
   */
  std::shared_ptr<Image> load_texture(const std::string& filepath);

  /**
   * Clears the internal state such as the texture cache.
   */
  void reset();
};

#endif  // PARSER_HPP_
