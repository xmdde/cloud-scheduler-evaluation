#include "simulator.h"

#include <fstream>
#include <iomanip>
#include <memory>

Simulator::Simulator(std::unique_ptr<Scheduler> sched, Mode mode, int nodes_num, double sim_end)
    : scheduler(std::move(sched)), end_time(sim_end) {
    for (int i = 0; i < nodes_num; ++i) {
        nodes.push_back({i, 8, 16, mode});
    }
}

void Simulator::run(std::vector<Task>& tasks, const std::string& file_path) {
    std::cout << "Running " << scheduler->getName() << "...\n";

    std::ofstream log(file_path);
    log << "time,nodeId,usedCPU,usedRAM,numRunning\n";

    size_t next_idx = 0;
    double current_time = 0.0;
    while (next_idx < tasks.size() || !allTasksFinished()) {
        while (next_idx < tasks.size() && tasks[next_idx].arrival_time <= current_time) {
            scheduler->scheduleTask(tasks[next_idx], nodes, current_time);
            ++next_idx;
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

        current_time += time_step;
    }

    log.close();
    std::cout << "Simulation complete.\n";
}

bool Simulator::allTasksFinished() {
    for (const auto& node : nodes) {
        if (!node.isIdle()) {
            return false;
        }
    }
    return true;
}
