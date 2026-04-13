#include "simulator.h"

#include <fstream>
#include <iomanip>
#include <memory>
#include <optional>

#include "request_queue.h"

Simulator::Simulator(std::unique_ptr<Scheduler> sched, int nodes_num)
    : scheduler(std::move(sched)) {
    for (int i = 0; i < nodes_num / 2; ++i) {
        nodes.push_back({i, 32.0, 128.0}); 
    }

    for (int i = 0; i < nodes_num / 2; ++i) {
        nodes.push_back({i + (nodes_num / 2), 16.0, 64.0}); 
    }
}

void Simulator::run(const QueuePolicy policy, std::vector<Task>& tasks, std::optional<std::string> file_path) {
    std::cout << "--------------------------\nRunning " << scheduler->getName() << "...\n";

    std::ofstream log;
    std::ofstream tasks_log;

    if (file_path.has_value()) {
        log.open(*file_path);
        if (log.is_open()) {
            log << "time,host_id,used_CPU,used_RAM,num_running,state,total_energy\n";
        }

        std::string tasks_log_path = *file_path;
        size_t ext_pos = tasks_log_path.rfind(".csv");
        if (ext_pos != std::string::npos) {
            tasks_log_path.replace(ext_pos, 4, "_tasks.csv");
        } else {
            tasks_log_path += "_tasks.csv";
        }

        tasks_log.open(tasks_log_path);
        if (tasks_log.is_open()) {
            tasks_log << "task_id,arrival_time,start_time,wait_time\n";
        }
    }

    size_t next_idx = 0;
    double current_time = 0.0;
    RequestQueue global_queue(policy);

    while (next_idx < tasks.size() || !global_queue.empty() || !allNodesFinished()) {
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
                if (tasks_log.is_open()) {
                    double wait_time = it->start_time - it->arrival_time;
                    tasks_log << it->id << "," 
                              << it->arrival_time << "," 
                              << it->start_time << "," 
                              << wait_time << "\n";
                }
                it = global_queue.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& node : nodes) {
            node.tick(current_time, TIME_STEP);
        }

        current_time += TIME_STEP;

        if (log.is_open() && static_cast<int>(current_time) % TELEMETRY_INTERVAL == 0) {
            for (auto& node : nodes) {
                node.logHostState(log, current_time);
            }
        }
    }

    if (log.is_open()) {
        for (auto& node : nodes) {
            node.logHostState(log, current_time);
        }
        log.close();
    }
    
    if (tasks_log.is_open()) {
        tasks_log.close();
    }

    std::cout << "Simulation complete. Makespan: " << current_time - TIME_STEP << '\n';
    double total_system_energy = 0.0;
    for (const auto& node : nodes) {
        std::cout << "|Host " << node.id << "| E = " << node.total_energy << " J\n";
        total_system_energy += node.total_energy;
    }
    std::cout << "Total System Energy: " << total_system_energy/1000 << " kJ\n";
}

bool Simulator::allNodesFinished() {
    for (const auto& node : nodes) {
        if (node.getRunningNum()) {
            return false;
        }
    }
    return true;
}
