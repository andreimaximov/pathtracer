#define TINYOBJLOADER_IMPLEMENTATION

#include "parser.hpp"
#include <memory>
#include <string>
#include <vector>
#include "area-light.hpp"
#include "glm/glm.hpp"
#include "scene.hpp"
#include "triangle.hpp"

std::shared_ptr<spdlog::logger> Parser::LOG = spdlog::stdout_color_mt("Parser");

Parser::Parser(Config config) : config(config) {}

Scene Parser::parse(std::string scene_file,
                    std::string materials_path) throw(std::logic_error) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string error;

  // Materials path should have trailing slash otherwise the OBJ loader cannot
  // find it. (Bug)
  if (!materials_path.empty() &&
      materials_path[materials_path.size() - 1] != '/') {
    materials_path += "/";
  }

  Scene scene;

  LOG->info("Loading {:s}...", scene_file);

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &error,
                              scene_file.c_str(), materials_path.c_str(), true);

  if (!ret) {
    throw std::logic_error("Could not load " + scene_file + ".");
  } else if (!error.empty()) {
    LOG->warn(error);
  }

  // Begin by loading vertices...
  auto V = std::make_shared<std::vector<glm::vec3>>();

  for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
    V->push_back(to_vec(attrib.vertices.data() + i * 3));
  }

  LOG->info("Loaded {:d} vertices.", V->size());

  // Then load normals...
  auto N = std::make_shared<std::vector<glm::vec3>>();

  for (size_t i = 0; i < attrib.normals.size() / 3; i++) {
    N->push_back(to_vec(attrib.normals.data() + i * 3));
  }

  LOG->info("Loaded {:d} normals.", N->size());

  // Then the texture coordinates...
  auto T = std::make_shared<std::vector<glm::vec2>>();

  for (size_t i = 0; i < attrib.texcoords.size() / 2; i++) {
    T->emplace_back(attrib.texcoords[i * 2], attrib.texcoords[i * 2 + 1]);
  }

  LOG->info("Loaded {:d} texture coordinates.", T->size());

  // Now load the materials...
  auto M = std::make_shared<std::vector<Material>>();
  Material def;
  def.Ke = Color(1, 0, 0);
  M->push_back(def);

  for (size_t i = 0; i < materials.size(); i++) {
    Material m;

    // Load basic material properties...
    m.Ke = to_color(materials[i].emission);
    m.Ka = to_color(materials[i].ambient);
    m.Kd = to_color(materials[i].diffuse);
    m.Ks = to_color(materials[i].specular);
    m.Kt = to_color(materials[i].transmittance);
    m.Pr = materials[i].roughness;
    m.Ni = materials[i].ior;

    // Load textures...
    if (config.loader.textures && !materials[i].diffuse_texname.empty()) {
      m.Kd_texture =
          load_texture(materials_path + "/" + materials[i].diffuse_texname);
    }

    M->push_back(m);
  }

  LOG->info("Loaded {:d} materials.", M->size());

  // Finally load the triangles...
  std::vector<Primitive::SharedPtr> S;

  for (size_t i = 0; i < shapes.size(); i++) {
    for (size_t j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
      // We only support triangulated meshes.
      if (shapes[i].mesh.num_face_vertices[j] != 3) {
        throw std::logic_error("Mesh contains a non-triangular face!");
      }

      int pid = S.size();

      const std::vector<tinyobj::index_t>& indices = shapes[i].mesh.indices;

      auto av = indices[j * 3 + 0].vertex_index;
      auto bv = indices[j * 3 + 1].vertex_index;
      auto cv = indices[j * 3 + 2].vertex_index;

      if (av < 0 || bv < 0 || cv < 0 || av >= V->size() || bv >= V->size() ||
          cv >= V->size()) {
        LOG->error("Mesh {:d} of shape {:d} has invalid vertex.", j, i);
        continue;
      }

      // Load vertex normal indices...
      auto an = indices[j * 3 + 0].normal_index;
      auto bn = indices[j * 3 + 1].normal_index;
      auto cn = indices[j * 3 + 2].normal_index;

      if (an < 0 || bn < 0 || cn < 0 || an >= N->size() || bn >= N->size() ||
          cn >= N->size() || !config.loader.normals) {
        an = -1;
        bn = -1;
        cn = -1;
      }

      // Load texture coordinate indices...
      auto at = indices[j * 3 + 0].texcoord_index;
      auto bt = indices[j * 3 + 1].texcoord_index;
      auto ct = indices[j * 3 + 2].texcoord_index;

      if (at < 0 || bt < 0 || ct < 0 || at >= T->size() || bt >= T->size() ||
          ct >= T->size()) {
        at = -1;
        bt = -1;
        ct = -1;
      }

      auto a = Vertex{av, an, at};
      auto b = Vertex{bv, bn, bt};
      auto c = Vertex{cv, cn, ct};

      // Load material...
      auto m = shapes[i].mesh.material_ids[j];

      // Use default material if object does not have one or index is invalid.
      if (m < 0 || m >= materials.size()) {
        m = 0;
      } else {
        m++;
      }

      auto triangle = std::make_shared<Triangle>(a, b, c, m, V, N, M, T);

      S.push_back(triangle);

      // Index the object as a light if it the material has emission.
      if (!M->at(m).Ke.isBlack()) {
        scene.lights.emplace_back(new AreaLight(triangle));
      }
    }
  }

  LOG->info("Loaded {:d} triangles.", S.size());

  if (S.empty()) {
    return scene;
  }

  scene.bvh = BVH::build(std::move(S));

  BoundingBox box = scene.bvh.get_bounds();
  LOG->info("Bounds: [{:.2f}, {:.2f}] x [{:.2f}, {:.2f}] x [{:.2f}, {:.2f}]",
            box.min.x, box.max.x, box.min.y, box.max.y, box.min.z, box.max.z);

  return scene;
}

std::shared_ptr<Image> Parser::load_texture(const std::string& filepath) {
  if (textures.find(filepath) == textures.end()) {
    std::shared_ptr<Image> copy(new Image(loader.load(filepath)));
    textures[filepath] = copy;
  }

  return textures[filepath];
}

void Parser::reset() {
  textures.clear();
}

Color Parser::to_color(const tinyobj::real_t* color) {
  return Color(color[0], color[1], color[2]);
}

glm::vec3 Parser::to_vec(const tinyobj::real_t* vec) {
  return glm::vec3(vec[0], vec[1], vec[2]);
}
