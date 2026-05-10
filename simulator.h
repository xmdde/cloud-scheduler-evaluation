#ifndef CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
#define CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H

#include <fstream>
#include <vector>
#include <memory>
#include <optional>
#include <ranges>

#include "scheduler.h"
#include "task.h"
#include "request_queue.h"

class Simulator {
    std::vector<Host> nodes;
    std::unique_ptr<Scheduler> scheduler;

    static constexpr double TIME_STEP = 1.0;
    static constexpr int TELEMETRY_INTERVAL = 30;

    double total_wait_time = 0.0;
    int completed_tasks = 0;

    void logClusterState(std::ofstream& log, const double current_time, const RequestQueue& global_queue);
    bool allNodesFinished();

public:
    Simulator(std::unique_ptr<Scheduler> sched, int nodes_num);
    void run(QueuePolicy policy, std::span<Task> tasks, std::optional<std::string> file_path);
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SIMULATOR_H
