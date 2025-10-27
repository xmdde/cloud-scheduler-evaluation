#include <iostream>
#include <iomanip>
#include <memory>

#include "node.h"
#include "workload-generator.h"
#include "simulator.h"
#include "FCFS-scheduler.h"
#include "round-robin-scheduler.h"

int main(int argc, const char* argv[]) {
    WorkloadGenerator gen;

    auto tasks = gen.generate(
        20,
        DistributionType::NORMAL,
        1.0, 0.3,   // CPU mean, std
        2.0, 0.5,   // RAM mean, std
        10.0, 2.0,  // Duration mean, std  // was 30, 10
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
    
    //Simulator sim(std::make_unique<FCFSScheduler>(), Mode::SLOT, 3);
    Simulator sim(std::make_unique<RoundRobinScheduler>(), Mode::QUEUE, 3);
    sim.run(tasks, "output/simulation_log.csv");

    return 0;
}
