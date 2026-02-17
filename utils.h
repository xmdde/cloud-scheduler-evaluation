#ifndef CLOUD_SCHEDULER_EVALUATION_UTILS_H
#define CLOUD_SCHEDULER_EVALUATION_UTILS_H

#include <memory>

#include "simulator.h"
#include "scheduler.h"
#include "task.h"
#include "round-robin-scheduler.h"
#include "FCFS-scheduler.h"

enum class SchedulingMethod {
    FCFS,
    ROUND_ROBIN,
    BEST_FIT,
    LOAD,
    ENERGY_AWARE,
    ML
};

void runSimulation(const SchedulingMethod algorithm, const Mode mode, std::vector<Task>& tasks, const std::string& output_path) {
    std::unique_ptr<Scheduler> scheduler;

    switch (algorithm) {
    case SchedulingMethod::FCFS:
        scheduler = std::make_unique<FCFSScheduler>();
        break;
    
    case SchedulingMethod::ROUND_ROBIN:
        scheduler = std::make_unique<RoundRobinScheduler>();
        break;
    }

    Simulator sim(std::move(scheduler), mode, 4);
    sim.run(tasks, output_path);
}

#endif  // CLOUD_SCHEDULER_EVALUATION_UTILS_H
