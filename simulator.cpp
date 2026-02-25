#include "simulator.h"

#include <fstream>
#include <iomanip>
#include <memory>

#include "request_queue.h"

Simulator::Simulator(std::unique_ptr<Scheduler> sched, const Mode mode, int nodes_num)
    : scheduler(std::move(sched)) {
    for (int i = 0; i < nodes_num/2; ++i) {
        nodes.push_back({i, 8, 16, mode});
    }
    for (int i = 0; i < nodes_num/2; ++i) {
        nodes.push_back({i + nodes_num/2, 4, 8, mode});
    }
}

void Simulator::run(std::vector<Task>& tasks, const std::string& file_path) {
    std::cout << "--------------------------\nRunning " << scheduler->getName() << "...\n";

    std::ofstream log(file_path);
    log << "time,host_id,used_CPU,used_RAM,num_running,total_energy\n"; 

    size_t next_idx = 0;
    double current_time = 0.0;
    
    QueuePolicy policy = (scheduler->getName() == "MBFD") ? QueuePolicy::DECREASING_CPU : QueuePolicy::FCFS;
    RequestQueue global_queue(policy); 

    while (next_idx < tasks.size() || !global_queue.empty() || !allNodesIdle()) {
        std::vector<Task> incoming_tasks;
        while (next_idx < tasks.size() && tasks[next_idx].arrival_time <= current_time) {
            incoming_tasks.push_back(tasks[next_idx]);
            ++next_idx;
        }

        global_queue.addRequests(incoming_tasks);

        auto it = global_queue.begin();
        while (it != global_queue.end()) {
            bool success = scheduler->scheduleTask(*it, nodes, current_time);
            if (success) {
                it = global_queue.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& node : nodes) {
            node.tick(current_time, time_step);
            const int TELEMETRY_INTERVAL = 10;
            if (static_cast<int>(current_time) % TELEMETRY_INTERVAL == 0 || allNodesIdle()) {
                node.logHostState(log, current_time);
            }
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
