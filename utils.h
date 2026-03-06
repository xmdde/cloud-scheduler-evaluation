#ifndef CLOUD_SCHEDULER_EVALUATION_UTILS_H
#define CLOUD_SCHEDULER_EVALUATION_UTILS_H

#include <memory>

#include "simulator.h"
#include "scheduler.h"
#include "task.h"
#include "request_queue.h"

#include "round-robin-scheduler.h"
#include "best-fit-scheduler.h"
#include "worst-fit-scheduler.h"
#include "mbfd-scheduler.h"

constexpr int HOSTS_NUM = 16;

enum class SchedulingMethod {
    ROUND_ROBIN,
    BEST_FIT,
    BFD,
    WORST_FIT,
    LOAD,
    MBFD
};

void runSimulation(const SchedulingMethod algorithm, std::vector<Task>& tasks, const std::string& output_path) {
    std::unique_ptr<Scheduler> scheduler;
    QueuePolicy policy = QueuePolicy::FCFS;

    switch (algorithm) {
        case SchedulingMethod::ROUND_ROBIN:
            scheduler = std::make_unique<RoundRobinScheduler>();
            break;

        case SchedulingMethod::BEST_FIT:
            scheduler = std::make_unique<BestFitScheduler>();
            break;

        case SchedulingMethod::BFD:
            scheduler = std::make_unique<BestFitScheduler>();
            policy = QueuePolicy::DECREASING_CPU;
            break;
        
        case SchedulingMethod::WORST_FIT:
            scheduler = std::make_unique<WorstFitScheduler>();
            break;
        
        case SchedulingMethod::MBFD:
            scheduler = std::make_unique<MBFDScheduler>();
            policy = QueuePolicy::DECREASING_CPU;
            break;

        default:
            std::cerr << "Method to add!\n";
    }

    Simulator sim(std::move(scheduler), HOSTS_NUM);
    sim.run(policy, tasks, output_path);
}

#endif  // CLOUD_SCHEDULER_EVALUATION_UTILS_H
