#include <atomic>
#include <mutex>
#include <queue>
#include <vector>
#include "config.hpp"
#include "image.hpp"
#include "pathtracer.hpp"
#include "scene.hpp"

#ifndef WORKER_HPP_
#define WORKER_HPP_

class Worker {
 public:
  /**
   * Region of the image from the begin (inclusive) to end (exclusive) pixel.
   */
  struct Work {
    int begin;

    int end;

    int samples;

    bool operator>(const Work& other) const;
  };

  /**
   * Thread safe queue that prioritizes partitions with less samples.
   */
  class Queue {
   private:
    std::priority_queue<Work, std::vector<Work>, std::greater<Work>> queue;

    std::mutex mutex;

   public:
    /**
     * Pushes a unit of work onto the end of the queue.
     */
    void push(const Work& work);

    /**
     * Moves and pops the first element of the queue into work. Returns true
     * on success and false if the queue was empty.
     */
    bool poll(Work& work);

    /**
     * Copies the first element of the queue into work. Returns true on success
     * and false if the queue was empty.
     */
    bool peek(Work& work);
  };

  /**
   * Create a worker that will consume the queue and update the image.
   */
  Worker(Config config, const Scene& scene, Image& image, Queue& queue,
         std::atomic<int>& running);

  /**
   * Runs the worker until the queue is empty.
   */
  void operator()() const;

 private:
  PathTracer pathtracer;

  Config config;

  const Scene& scene;

  Image& image;

  Queue& queue;

  std::atomic<int>& running;
};

#endif  // WORKER_HPP_
