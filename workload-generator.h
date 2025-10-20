#pragma once

#include <vector>
#include <random>
#include <chrono>

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
    WorkloadGenerator(unsigned seed = std::random_device{}()) : rng(seed) {}

    std::vector<Task> generate(
        int count,
        DistributionType dist,
        double cpuMean,
        double cpuStd,
        double ramMean,
        double ramStd,
        double durationMean,
        double durationStd,
        double arrivalRate = 1.0)
    {
        std::vector<Task> tasks;
        tasks.reserve(count);

        // Rozkłady
        std::uniform_real_distribution<double> uniformCPU(cpuMean - cpuStd, cpuMean + cpuStd);
        std::uniform_real_distribution<double> uniformRAM(ramMean - ramStd, ramMean + ramStd);
        std::uniform_real_distribution<double> uniformDur(durationMean - durationStd, durationMean + durationStd);
        std::uniform_real_distribution<double> uniformArr(0.0, arrivalRate * count);

        std::normal_distribution<double> normalCPU(cpuMean, cpuStd);
        std::normal_distribution<double> normalRAM(ramMean, ramStd);
        std::normal_distribution<double> normalDur(durationMean, durationStd);

        std::exponential_distribution<double> expDur(1.0 / durationMean);
        std::exponential_distribution<double> expArr(1.0 / arrivalRate);

        std::poisson_distribution<int> poissonArr(arrivalRate);

        double currentArrival = 0.0;

        for (int i = 0; i < count; ++i) {
            double cpu = 1.0, ram = 1.0, duration = 1.0;

            switch (dist) {
                case DistributionType::UNIFORM:
                    cpu = std::max(0.1, uniformCPU(rng));
                    ram = std::max(0.1, uniformRAM(rng));
                    duration = std::max(0.1, uniformDur(rng));
                    currentArrival += uniformArr(rng) / count;
                    break;

                case DistributionType::NORMAL:
                    cpu = std::max(0.1, normalCPU(rng));
                    ram = std::max(0.1, normalRAM(rng));
                    duration = std::max(0.1, normalDur(rng));
                    currentArrival += std::abs(normalDur(rng) / 10.0);
                    break;

                case DistributionType::EXPONENTIAL:
                    cpu = std::max(0.1, normalCPU(rng)); // CPU i RAM z normalnego
                    ram = std::max(0.1, normalRAM(rng));
                    duration = std::max(0.1, expDur(rng));
                    currentArrival += expArr(rng);
                    break;

                case DistributionType::POISSON:
                    cpu = std::max(0.1, normalCPU(rng));
                    ram = std::max(0.1, normalRAM(rng));
                    duration = std::max(0.1, normalDur(rng));
                    currentArrival += poissonArr(rng);
                    break;
            }

            Task t;
            t.id = next_task_id++;
            t.arrival_time = currentArrival;
            t.cpu_required = cpu;
            t.ram_required = ram;
            t.duration = duration;

            tasks.push_back(t);
        }

        return tasks;
    }
};
