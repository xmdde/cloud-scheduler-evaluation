#ifndef CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
#define CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H

#include <vector>
#include <memory>

#include "scheduler.h"

class Simulator {
    std::vector<Node> nodes;
    std::unique_ptr<Scheduler> scheduler;
    const double time_step = 1.0;
    double end_time;

    bool allTasksFinished();

public:
    Simulator(std::unique_ptr<Scheduler> sched, Mode mode, int nodes_num, double sim_end);
    void run(std::vector<Task>& tasks, const std::string& file_path);
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
