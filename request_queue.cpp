#include "request_queue.h"

void RequestQueue::addRequests(const std::vector<Task>& new_tasks) {
    if (new_tasks.empty())
        return;

    for (const auto& task : new_tasks) {
        queue.push_back(task);
    }

    if (policy == QueuePolicy::DECREASING_CPU) {
        std::sort(queue.begin(), queue.end(), [](const Task& a, const Task& b) {
            return a.cpu_required > b.cpu_required;
        });
    }
}