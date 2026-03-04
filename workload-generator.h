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
    WorkloadGenerator(uint64_t seed = std::random_device{}()) : rng(seed) {}

    std::vector<Task> generate(
        int count,
        DistributionType dist,
        double cpu_mean,
        double cpu_std,
        double ram_mean,
        double ram_std,
        double durationMean,
        double durationStd,
        double arrivalRate = 1.0);

    std::vector<Task> generateRealisticCloudTraffic();
};

#endif  // CLOUD_SCHEDULER_EVALUATION_WORKLOAD_GENERATOR_H
