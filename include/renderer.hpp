#include <spdlog/spdlog.h>
#include <string>
#include "config.hpp"
#include "image-saver.hpp"
#include "scene.hpp"

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

class Renderer {
 public:
  /**
   * Creates a configured renderer.
   */
  Renderer(Config config, const ImageSaver& saver);

  /**
   * Renders the scene.
   */
  void render(const Scene& scene, const std::string& output) const;

 private:
  static std::shared_ptr<spdlog::logger> LOG;

  Config config;

  const ImageSaver& saver;
};

#endif  // RENDERER_HPP_
