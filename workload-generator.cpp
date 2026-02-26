#include "workload-generator.h"

std::vector<Task> WorkloadGenerator::generate(
    int count, DistributionType dist,
    double cpu_mean, double cpu_std,
    double ram_mean, double ram_std,
    double duration_mean, double duration_std,
    double arrival_rate) {  // inter arrival time
        std::vector<Task> tasks;
        tasks.reserve(count);

        // Rozkłady
        std::uniform_real_distribution<double> uniformCPU(cpu_mean - cpu_std, cpu_mean + cpu_std);
        std::uniform_real_distribution<double> uniformRAM(ram_mean - ram_std, ram_mean + ram_std);
        std::uniform_real_distribution<double> uniformDur(duration_mean - duration_std, duration_mean + duration_std);
        std::uniform_real_distribution<double> uniformArr(0.0, arrival_rate * count);

        std::normal_distribution<double> normalCPU(cpu_mean, cpu_std);
        std::normal_distribution<double> normalRAM(ram_mean, ram_std);
        std::normal_distribution<double> normalDur(duration_mean, duration_std);

        std::exponential_distribution<double> expDur(1.0 / duration_mean);
        std::exponential_distribution<double> expArr(1.0 / arrival_rate);

        std::poisson_distribution<int> poissonArr(arrival_rate);

        double current_arrival = 0.0;
        for (int i = 0; i < count; ++i) {
            double cpu = 1.0, ram = 1.0, duration = 1.0;

            switch (dist) {
                case DistributionType::UNIFORM:
                    cpu = std::max(0.1, uniformCPU(rng));
                    ram = std::max(0.1, uniformRAM(rng));
                    duration = std::max(0.1, uniformDur(rng));
                    current_arrival += uniformArr(rng) / count;
                    break;

                case DistributionType::NORMAL:
                    cpu = std::max(0.1, normalCPU(rng));
                    ram = std::max(0.1, normalRAM(rng));
                    duration = std::max(0.1, normalDur(rng));
                    current_arrival += std::abs(normalDur(rng) / 10.0);
                    break;

                case DistributionType::EXPONENTIAL:
                    cpu = std::max(0.1, normalCPU(rng)); // CPU i RAM z normalnego
                    ram = std::max(0.1, normalRAM(rng));
                    duration = std::max(0.1, expDur(rng));
                    current_arrival += expArr(rng);
                    break;

                case DistributionType::POISSON:
                    cpu = std::max(0.1, normalCPU(rng));
                    ram = std::max(0.1, normalRAM(rng));
                    duration = std::max(0.1, normalDur(rng));
                    current_arrival += poissonArr(rng);
                    break;
            }

            Task t;
            t.id = next_task_id++;
            t.arrival_time = current_arrival;
            t.cpu_required = cpu;
            t.ram_required = ram;
            t.duration = duration;

            tasks.push_back(t);
        }

        return tasks;
    }
