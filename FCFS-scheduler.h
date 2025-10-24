#ifndef CLOUD_SCHEDULER_EVALUATION_FCFS_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_FCFS_SCHEDULER_H

#include <vector>
#include <queue>

#include "scheduler.h"
#include "node.h"
#include "task.h"

class FCFSScheduler : public Scheduler {
    std::queue<Task> queue;

public:
    std::string getName() const override {
        return "FCFS";
    }

    void scheduleTask(const Task new_task, std::vector<Node>& nodes, double current_time) override {
        queue.push(new_task);

        std::queue<Task> remaining;
        while (!queue.empty()) {
            Task t = queue.front();
            queue.pop();

            bool assigned = false;
            for (auto& n : nodes) {
                if (n.canRun(t)) {
                    n.assignTask(t, current_time);
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                remaining.push(t);
            }
        }

        queue = std::move(remaining);
    }
};

#endif  // CLOUD_SCHEDULER_EVALUATION_FCFS_SCHEDULER_H
