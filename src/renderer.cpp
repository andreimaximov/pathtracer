#include "renderer.hpp"
#include <chrono>
#include <thread>
#include "image.hpp"
#include "worker.hpp"

std::shared_ptr<spdlog::logger> Renderer::LOG =
    spdlog::stdout_color_mt("Renderer");

Renderer::Renderer(Config config, const ImageSaver& saver)
    : config(config), saver(saver) {}

void Renderer::render(const Scene& scene, const std::string& output) const {
  int width = scene.camera.width;
  int height = scene.camera.height;
  Image image(width, height);
  image.set_gamma(2.2);

  // Split up image into regions and shuffle for even load - some parts of the
  // image may be more expensive than others.
  Worker::Queue queue;
  int total = width * height;
  int partitionSize = total / config.job.partitions;
  int partitions = 0;

  for (int i = 0; i < total; i += partitionSize, partitions++) {
    Worker::Work work{i, std::min(i + partitionSize, total), 0};
    queue.push(work);
  }

  // Track the number of running workers.
  std::atomic<int> running(config.job.threads);

  auto autosave = [&]() {
    int progress = 0;

    while (running > 0) {
      saver.save(output, image);

      Worker::Work work;
      if (queue.peek(work)) {
        progress = work.samples;
      }

      LOG->info("{:d} samples...", progress);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  };

  LOG->info("Rendering with {:d} threads...", config.job.threads);

  // Spawn workers + thread that periodically saves the image and logs progress.
  std::vector<std::thread> threads;
  threads.emplace_back(autosave);
  for (int i = 0; i < config.job.threads; i++) {
    threads.emplace_back(Worker(config, scene, image, queue, running));
  }

  std::for_each(threads.begin(), threads.end(), mem_fn(&std::thread::join));
  saver.save(output, image);
}
