#include "simulator.h"

#include <fstream>
#include <iomanip>
#include <memory>

Simulator::Simulator(std::unique_ptr<Scheduler> sched, const Mode mode, int nodes_num)
    : scheduler(std::move(sched)) {
    for (int i = 0; i < nodes_num/2; ++i) {
        nodes.push_back({i, 8, 16, mode});
    }
    for (int i = 0; i < nodes_num/2; ++i) {
        nodes.push_back({i, 4, 8, mode});
    }
}

void Simulator::run(std::vector<Task>& tasks, const std::string& file_path) {
    std::cout << "--------------------------\nRunning " << scheduler->getName() << "...\n";

    std::ofstream log(file_path);
    log << "time,hostId,usedCPU,usedRAM,numRunning,totalEnergy\n"; 

    size_t next_idx = 0;
    double current_time = 0.0;

    std::deque<Task> global_queue; 

    while (next_idx < tasks.size() || !global_queue.empty() || !allNodesIdle()) {
        // Add new tasks to the global queue
        while (next_idx < tasks.size() && tasks[next_idx].arrival_time <= current_time) {
            global_queue.push_back(tasks[next_idx]);
            ++next_idx;
        }

        // Try to schedule
        auto it = global_queue.begin();
        while (it != global_queue.end()) {
            bool success = scheduler->scheduleTask(*it, nodes, current_time);
            
            if (success) {
                it = global_queue.erase(it);
            } else {
                ++it;  // ?
            }
        }

        // Update nodes
        for (auto& node : nodes) {
            node.tick(current_time, time_step);

            log << std::fixed << std::setprecision(2);
            log << current_time << "," << node.id << ","
                << node.used_CPU << "," << node.used_RAM << ","
                << node.getRunningNum() << ","
                << node.total_energy_consumed << "\n";
        }

        current_time += time_step;
    }

    log.close();

    std::cout << "Simulation complete. Makespan: " << current_time - time_step << '\n';
    double total_system_energy = 0.0;
    for (const auto& node : nodes) {
        std::cout << "|Host " << node.id << "| E = " << node.total_energy_consumed << " J\n";
        total_system_energy += node.total_energy_consumed;
    }
    std::cout << "Total System Energy: " << total_system_energy << " J\n";
}

bool Simulator::allNodesIdle() {
    for (const auto& node : nodes) {
        if (!node.isIdle()) {
            return false;
        }
    }
    return true;
}
