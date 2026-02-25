#ifndef CLOUD_SCHEDULER_EVALUATION_UTILS_H
#define CLOUD_SCHEDULER_EVALUATION_UTILS_H

#include <memory>

#include "simulator.h"
#include "scheduler.h"
#include "task.h"

#include "round-robin-scheduler.h"
#include "best-fit-scheduler.h"
#include "worst-fit-scheduler.h"

constexpr int HOSTS_NUM = 10;

enum class SchedulingMethod {
    ROUND_ROBIN,
    BEST_FIT,
    WORST_FIT,
    LOAD,
    ENERGY_AWARE,
};

void runSimulation(const SchedulingMethod algorithm, const Mode mode, std::vector<Task>& tasks, const std::string& output_path) {
    std::unique_ptr<Scheduler> scheduler;

    switch (algorithm) {
        case SchedulingMethod::ROUND_ROBIN:
            scheduler = std::make_unique<RoundRobinScheduler>();
            break;

        case SchedulingMethod::BEST_FIT:
            scheduler = std::make_unique<BestFitScheduler>();
            break;
        
        case SchedulingMethod::WORST_FIT:
            scheduler = std::make_unique<WorstFitScheduler>();
            break;

        default:
            std::cerr << "Method to add!\n";
    }

    Simulator sim(std::move(scheduler), mode, HOSTS_NUM);
    sim.run(tasks, output_path);
}

#endif  // CLOUD_SCHEDULER_EVALUATION_UTILS_H
