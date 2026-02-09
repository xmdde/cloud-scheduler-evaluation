#ifndef CLOUD_SCHEDULER_EVALUATION_FCFS_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_FCFS_SCHEDULER_H

#include <vector>
#include <queue>

#include "scheduler.h"
#include "node.h"
#include "task.h"

class FCFSScheduler : public Scheduler {
public:
    std::string getName() const override {
        return "FCFS";
    }

    bool scheduleTask(const Task& new_task, std::vector<Node>& nodes, double current_time) override;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_FCFS_SCHEDULER_H
