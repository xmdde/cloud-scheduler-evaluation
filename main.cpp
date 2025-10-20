#include <iostream>
#include <iomanip>

#include "node.h"
#include "workload-generator.h"

int main(int argc, const char* argv[]) {
    WorkloadGenerator gen;

    auto tasks = gen.generate(
        100,
        DistributionType::NORMAL,
        1.0, 0.3,   // CPU mean, std
        2.0, 0.5,   // RAM mean, std
        30.0, 10.0, // Duration mean, std
        1.0         // Arrival rate
    );

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "ID\tArrival\tCPU\tRAM\tDuration\n";
    std::cout << "----------------------------------------\n";
    for (const auto& t : tasks) {
        std::cout << t.id << "\t"
                  << t.arrival_time << "\t"
                  << t.cpu_required << "\t"
                  << t.ram_required << "\t"
                  << t.duration << "\n";
    }

    return 0;
}
