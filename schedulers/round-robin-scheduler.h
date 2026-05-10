#ifndef CLOUD_SCHEDULER_EVALUATION_ROUND_ROBIN_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_ROUND_ROBIN_SCHEDULER_H

#include <vector>

#include "scheduler.h"
#include "host.h"
#include "task.h"

class RoundRobinScheduler : public Scheduler {
    size_t curr_node_idx = 0;

public:
    explicit RoundRobinScheduler() 
        : Scheduler("RoundRobin") {}

    bool scheduleTask(Task& new_task, std::vector<Host>& nodes, double current_time) override;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_ROUND_ROBIN_SCHEDULER_H
