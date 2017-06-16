#include "worker.hpp"

/**
 * ============================================================
 *                            WORKER
 * ============================================================
 */

Worker::Worker(Config config, const Scene& scene, Image& image,
               Worker::Queue& queue, std::atomic<int>& running)
    : pathtracer(config),
      config(config),
      scene(scene),
      image(image),
      queue(queue),
      running(running) {}

void Worker::operator()() const {
  Work work;

  while (queue.poll(work)) {
    for (int i = work.begin; i < work.end; i++) {
      int x = i % scene.camera.width;
      int y = i / scene.camera.width;

      Ray ray = scene.camera.pixel_ray(x, y);
      Color pixel = pathtracer.trace(scene, ray);
      float factor = static_cast<float>(work.samples) / (work.samples + 1);

      Color current = image.get_pixel(x, y);
      image.set_pixel(x, y, current * factor + pixel * (1 - factor));
    }

    work.samples++;
    if (work.samples < config.rendering.samples) {
      queue.push(work);
    }
  }

  running--;
}

/**
 * ============================================================
 *                            QUEUE
 * ============================================================
 */

void Worker::Queue::push(const Work& work) {
  std::lock_guard<std::mutex> lock(mutex);
  queue.push(work);
}

bool Worker::Queue::poll(Work& work) {
  std::lock_guard<std::mutex> lock(mutex);

  if (queue.empty()) return false;

  work = std::move(queue.front());
  queue.pop();
  return true;
}

bool Worker::Queue::peek(Work& work) {
  std::lock_guard<std::mutex> lock(mutex);

  if (queue.empty()) return false;

  work = queue.front();
  return true;
}
