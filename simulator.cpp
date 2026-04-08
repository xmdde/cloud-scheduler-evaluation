#include "simulator.h"

#include <fstream>
#include <iomanip>
#include <memory>

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

void Simulator::run(const QueuePolicy policy, std::vector<Task>& tasks, const std::string& file_path) {
    std::cout << "--------------------------\nRunning " << scheduler->getName() << "...\n";

    // 1. Plik logów dla Hostów (Szeregi czasowe)
    std::ofstream log(file_path);
    log << "time,host_id,used_CPU,used_RAM,num_running,state,total_energy\n"; 

    // 2. Plik logów dla Zadań (Czas oczekiwania / QoS)
    std::string tasks_log_path = file_path;
    size_t ext_pos = tasks_log_path.rfind(".csv");
    if (ext_pos != std::string::npos) {
        tasks_log_path.replace(ext_pos, 4, "_tasks.csv");
    } else {
        tasks_log_path += "_tasks.csv";
    }
    std::ofstream tasks_log(tasks_log_path);
    tasks_log << "task_id,arrival_time,start_time,wait_time\n";

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
            // UWAGA: upewnij się, że scheduleTask modyfikuje it->start_time!
            bool success = scheduler->scheduleTask(*it, nodes, current_time);
            
            if (success) {
                // Logowanie QoS do pliku zadań w momencie alokacji
                double wait_time = it->start_time - it->arrival_time;
                tasks_log << it->id << "," 
                          << it->arrival_time << "," 
                          << it->start_time << "," 
                          << wait_time << "\n";
                          
                it = global_queue.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& node : nodes) {
            node.tick(current_time, TIME_STEP);
        }

        current_time += TIME_STEP;

        if (static_cast<int>(current_time) % TELEMETRY_INTERVAL == 0) {
            for (auto& node : nodes) {
                node.logHostState(log, current_time);
            }
        }
    }

    for (auto& node : nodes) {
        node.logHostState(log, current_time);
    }

    log.close();
    tasks_log.close(); // Zamknięcie pliku z zadaniami

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
