#include <iostream>
#include <iomanip>

#include "host.h"
#include "utils.h"
#include "workload-generator.h"

int main(int argc, const char* argv[]) {
    WorkloadGenerator gen;

    auto tasks = gen.generate(
        20,
        DistributionType::NORMAL,
        1.0, 0.3,   // CPU mean, std
        2.0, 0.5,   // RAM mean, std
        10.0, 3.0,  // Duration mean, std - was 30, 10
        1.0         // Arrival rate
    );

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "ID\tArrival\tCPU\tRAM\tDuration\n"
              << "----------------------------------------\n";
    for (const auto& t : tasks) {
        std::cout << t.id << "\t" << t.arrival_time << "\t"
                  << t.cpu_required << "\t" 
                  << t.ram_required << "\t"
                  << t.duration << "\n";
    }

    runSimulation(SchedulingMethod::ROUND_ROBIN, Mode::QUEUE, tasks, "output/simulation-log-RR.csv");
    runSimulation(SchedulingMethod::FCFS, Mode::SLOT, tasks, "output/simulation-log-FCFS.csv");

    return 0;
}
