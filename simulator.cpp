#include "simulator.h"

#include <fstream>
#include <iomanip>
#include <memory>
#include <optional>
#include <numeric>
#include <ranges>
#include <format>
#include <span>

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

void Simulator::run(const QueuePolicy policy, std::span<Task> tasks, std::optional<std::string> file_path) {
    std::cout << scheduler->getName() << " ";

    std::ofstream log;
    std::ofstream tasks_log;

    if (file_path.has_value()) {
        log.open(*file_path);
        if (log.is_open()) {
            log << "time,active_nodes,idle_nodes,sleep_nodes,booting_nodes,total_power_W,queue_size,active_tasks\n";
        }

        std::string tasks_log_path = *file_path;
        if (tasks_log_path.ends_with(".csv")) {
            tasks_log_path.replace(tasks_log_path.rfind(".csv"), 4, "-tasks.csv");
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
        auto incoming = tasks.subspan(next_idx) 
                      | std::views::take_while([=](auto& t) { return t.arrival_time <= current_time; });

        auto count = std::ranges::distance(incoming); 
        global_queue.addRequests(incoming);
        next_idx += count;

        auto it = global_queue.begin();
        while (it != global_queue.end()) {
            if (scheduler->scheduleTask(*it, nodes, current_time)) {
                if (tasks_log.is_open()) {
                    tasks_log << std::format("{},{},{},{}\n", it->id, it->arrival_time, it->start_time, it->start_time - it->arrival_time);
                }
                total_wait_time += (it->start_time - it->arrival_time);
                ++completed_tasks;
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
            logClusterState(log, current_time, global_queue);
        }
    }

    if (log.is_open()) {
        logClusterState(log, current_time, global_queue);
        log.close();
    }
    
    if (tasks_log.is_open()) {
        tasks_log.close();
    }

    auto energy_view = nodes | std::views::transform(&Host::total_energy);
    double total_system_energy = std::accumulate(
        energy_view.begin(), 
        energy_view.end(), 
        0.0
    );
    const double avg_wait_time = completed_tasks > 0 ? total_wait_time / completed_tasks : 0.0;

    std::cout << std::format("{:.2f} {:.2f} {:.2f}\n", total_system_energy, current_time - TIME_STEP, avg_wait_time);  // total energy [J], makespan
}

void Simulator::logClusterState(std::ofstream& log, double current_time, const RequestQueue& global_queue) {
    auto count_state = [&](PowerState s) {
        return std::ranges::count_if(nodes, [s](const auto& n) { return n.getState() == s; });
    };

    log << std::format("{:.2f},{},{},{},{},{:.2f},{},{}\n",
        current_time,
        count_state(PowerState::ACTIVE),
        count_state(PowerState::IDLE),
        count_state(PowerState::SLEEP),
        count_state(PowerState::BOOTING),
        std::accumulate(nodes.begin(), nodes.end(), 0.0, [](double sum, const auto& n){ return sum + n.getInstantaneousPower(); }),
        global_queue.size(),
        std::accumulate(nodes.begin(), nodes.end(), 0, [](int sum, const auto& n){ return sum + n.getRunningNum(); })
    );
}

bool Simulator::allNodesFinished() {
    return std::ranges::all_of(nodes, [](const auto& node) { 
        return node.getRunningNum() == 0; 
    });
}
