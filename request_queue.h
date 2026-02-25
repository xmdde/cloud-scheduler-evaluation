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

    void addRequests(const std::vector<Task>& new_tasks);

    auto begin() {
        return queue.begin();
    }

    auto end() {
        return queue.end();
    }

    auto erase(decltype(queue.begin()) it) {
        return queue.erase(it);
    }
};
