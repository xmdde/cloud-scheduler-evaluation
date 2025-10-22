#include "simulator.h"

#include <fstream>
#include <iomanip>

Simulator::Simulator(Scheduler* sched, Mode mode, int nodes_num, double sim_end)
    : scheduler(sched), end_time(sim_end) {
    for (int i = 0; i < nodes_num; ++i) {
        nodes.push_back({i, 8, 16, mode});
    }
}

void Simulator::run(std::vector<Task> tasks, const std::string& file_path) {
    std::ofstream log(file_path);
    log << "time,nodeId,usedCPU,usedRAM,numRunning\n";

    size_t nextTaskIndex = 0;
    std::cout << "Running " << scheduler->getName() << "...\n";

    for (current_time = 0; current_time <= end_time; current_time += time_step) {
        while (nextTaskIndex < tasks.size() && tasks[nextTaskIndex].arrival_time <= current_time) {
            scheduler->scheduleTask(tasks[nextTaskIndex], nodes, current_time);
            nextTaskIndex++;
        }

        for (auto& node : nodes) {
            node.update(current_time);

            // think about when to log info!
            // maybe log cumulative info somewhere?
            log << std::fixed << std::setprecision(2);
            log << current_time << "," << node.id << ","
                << node.used_CPU << "," << node.used_RAM << ","
                << node.getRunningNum() << "\n";
        }
    }

    log.close();
    std::cout << "Simulation complete.\n";
}
