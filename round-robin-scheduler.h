#ifndef CLOUD_SCHEDULER_EVALUATION_ROUND_ROBIN_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_ROUND_ROBIN_SCHEDULER_H

#include <vector>

#include "scheduler.h"
#include "node.h"
#include "task.h"

class RoundRobinScheduler : public Scheduler {
    int node_index = 0;

public:
    std::string getName() const override {
        return "RoundRobin";
    }

    void scheduleTask(const Task& new_task, std::vector<Node>& nodes, double current_time) override;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_ROUND_ROBIN_SCHEDULER_H
