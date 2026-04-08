#include <iostream>
#include <iomanip>

#include "host.h"
#include "utils.h"
#include "workload-generator.h"

constexpr uint16_t REQUESTS_NUM = 50;

int main(int argc, const char* argv[]) {
    WorkloadGenerator gen;
    auto tasks = gen.generateRealisticCloudTraffic();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "ID\tArrival\tCPU\tRAM\tDuration\n"
              << "----------------------------------------\n";
    for (const auto& t : tasks) {
        std::cout << t.id << "\t" << t.arrival_time << "\t"
                  << t.cpu_required << "\t" 
                  << t.ram_required << "\t"
                  << t.duration << "\n";
    }

    runSimulation(SchedulingMethod::ROUND_ROBIN, tasks, "output/simulation-log-RR.csv");
    runSimulation(SchedulingMethod::BEST_FIT, tasks, "output/simulation-log-BestFit.csv");
    runSimulation(SchedulingMethod::WORST_FIT, tasks, "output/simulation-log-WorstFit.csv");
    runSimulation(SchedulingMethod::BFD, tasks, "output/simulation-log-BFD.csv");
    runSimulation(SchedulingMethod::MBFD, tasks, "output/simulation-log-MBFD.csv");
    return 0;
}
