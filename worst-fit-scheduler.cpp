#include "worst-fit-scheduler.h"

#include <limits>

bool WorstFitScheduler::scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) {
    int best_host_idx = -1;
    double max_remaining_capacity = std::numeric_limits<double>::min();

    for (int i = 0; i < nodes.size(); ++i) {
        Host& host = nodes[i];

        if (host.canRun(task)) {
            const double score = calculateScore(host, task);

            if (score > max_remaining_capacity) {
                max_remaining_capacity = score;
                best_host_idx = i;
            }
        }
    }

    if (best_host_idx != -1) {
        nodes[best_host_idx].assignTask(task, current_time);
        return true;
    }

    return false;
}

double WorstFitScheduler::calculateScore(const Host &host, const Task &task) const {
    const double cpu_tb_left = (host.total_CPU - host.used_CPU + task.cpu_required);
    const double ram_tb_left = (host.total_RAM - host.used_RAM + task.ram_required);

    return cpu_tb_left + ram_tb_left;
}
