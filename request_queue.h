#pragma once

#include <algorithm>
#include <deque>
#include <vector>

#include "task.h"

enum class QueuePolicy {
    FCFS,
    DECREASING_CPU
};

class RequestQueue {
private:
    std::deque<Task> queue;
    QueuePolicy policy;

public:
    RequestQueue(QueuePolicy p = QueuePolicy::FCFS) : policy(p) {}

    bool empty() const { return queue.empty(); }

    template <typename R>
    void addRequests(R&& new_tasks) {
        if (std::ranges::empty(new_tasks))
            return;

        for (const auto& task : new_tasks) {
            queue.push_back(task);
        }

        if (policy == QueuePolicy::DECREASING_CPU) {
            std::ranges::sort(queue, [](const Task& a, const Task& b) {
                return a.cpu_required > b.cpu_required;
            });
        }
    }

    auto begin() {
        return queue.begin();
    }

    auto end() {
        return queue.end();
    }

    auto erase(decltype(queue.begin()) it) {
        return queue.erase(it);
    }

    auto size() const {
        return queue.size();
    }
};
