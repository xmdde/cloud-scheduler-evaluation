#ifndef CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
#define CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H

#include <vector>

#include "scheduler.h"

class Simulator {
    std::vector<Node> nodes;
    Scheduler* scheduler;
    double current_time = 0.0;
    const double time_step = 1.0;
    double end_time;

public:
    Simulator(Scheduler* sched, Mode mode, int nodes_num, double sim_end);
    void run(std::vector<Task> tasks, const std::string& file_path);
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
