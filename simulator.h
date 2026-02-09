#ifndef CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
#define CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H

#include <vector>
#include <memory>

#include "scheduler.h"
#include "task.h"

class Simulator {
    std::vector<Node> nodes;
    std::unique_ptr<Scheduler> scheduler;
    const double time_step = 1.0;

    bool allNodesIdle();

public:
    Simulator(std::unique_ptr<Scheduler> sched, const Mode mode, int nodes_num);
    void run(std::vector<Task>& tasks, const std::string& file_path);
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
