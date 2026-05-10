#ifndef CLOUD_SCHEDULER_EVALUATION_WORKLOAD_GENERATOR_H
#define CLOUD_SCHEDULER_EVALUATION_WORKLOAD_GENERATOR_H

#include <chrono>
#include <random>
#include <vector>

#include "task.h"

enum class DistributionType {
    UNIFORM,
    NORMAL,
    EXPONENTIAL,
    POISSON
};

class WorkloadGenerator {
    std::mt19937 rng;
    int next_task_id = 1;

public:
    WorkloadGenerator(uint64_t seed = std::random_device{}()) : rng(seed), next_task_id(1) {}

    std::vector<Task> generateRealisticCloudTraffic();
    std::vector<Task> generateLowLoad();
    std::vector<Task> generateSpikyLoad();
    std::vector<Task> generateHighLoad();
};

#endif  // CLOUD_SCHEDULER_EVALUATION_WORKLOAD_GENERATOR_H
