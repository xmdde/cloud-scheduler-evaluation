#ifndef CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
#define CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H

#include <vector>
#include <memory>
#include <optional>

#include "scheduler.h"
#include "task.h"
#include "request_queue.h"

class Simulator {
    std::vector<Host> nodes;
    std::unique_ptr<Scheduler> scheduler;

    const double TIME_STEP = 1.0;
    static constexpr int TELEMETRY_INTERVAL = 30;

    bool allNodesFinished();

public:
    Simulator(std::unique_ptr<Scheduler> sched, int nodes_num);
    void run(QueuePolicy policy, std::vector<Task>& tasks, std::optional<std::string> file_path);
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
