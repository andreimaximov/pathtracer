#include <args.hxx>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include "config.hpp"
#include "parser.hpp"
#include "png-saver.hpp"
#include "renderer.hpp"
#include "scene.hpp"

int main(int argc, char* argv[]) {
  // Setup CLI.
  args::ArgumentParser args("pathtracer");
  args::HelpFlag help_arg(args, "help", "display this help menu",
                          {'h', "help"});
  args::Positional<std::string> scene_arg(args, "scene", "the scene file");
  args::Positional<std::string> mat_arg(args, "materials",
                                        "the materials directory");
  args::Positional<std::string> config_arg(args, "config", "the config file");
  args::Positional<std::string> out_arg(args, "output", "the output file",
                                        "render.png");

  // Parse args.
  try {
    args.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cout << args;
    return 0;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (args::ValidationError e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Load config file.
  std::ifstream config_file(args::get(config_arg));
  if (!config_file) {
    std::cerr << "Please specify an existing config file." << std::endl;
    return 1;
  }

  Config config;
  try {
    nlohmann::json config_json;
    config_file >> config_json;
    config = config_json;
    config_file.close();
  } catch (nlohmann::detail::exception e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (config.job.threads < 1) {
    std::cerr << "Please specify at least 1 render thread." << std::endl;
    return 1;
  } else if (config.job.partitions < 1) {
    std::cerr << "Please specify at least 1 partition." << std::endl;
    return 1;
  } else if (config.debug.normals || config.debug.diffuse) {
    config.rendering.samples = 1;
  }

  // Parse scene.
  Scene scene;

  try {
    Parser parser(config);
    scene = parser.parse(args::get(scene_arg), args::get(mat_arg));
  } catch (std::logic_error e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Setup camera.
  scene.camera.set_position(config.camera.position, config.camera.center,
                            config.camera.up);

  scene.camera.set_view(glm::radians(config.camera.fovy), config.camera.width,
                        config.camera.height);

  // Render scene.
  Renderer renderer(config, PNGSaver());
  renderer.render(scene, args::get(out_arg));

  return 0;
}
