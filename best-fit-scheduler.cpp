#include "best-fit-scheduler.h"

#include <limits>

bool BestFitScheduler::scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) {
    int best_host_idx = -1;
    double min_remaining_capacity = std::numeric_limits<double>::max(); //calculateScore(nodes[0], task);

    for (int i = 1; i < nodes.size(); ++i) {
        Host& host = nodes[i];

        if (host.canRun(task)) {
            const double score = calculateScore(host, task);

            if (score < min_remaining_capacity) {
                min_remaining_capacity = score;
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

double BestFitScheduler::calculateScore(const Host& host, const Task& task) const {
    const double cpu_tb_left = (host.total_CPU - host.used_CPU + task.cpu_required); // host.total_CPU;
    const double ram_tb_left = (host.total_RAM - host.used_RAM + task.ram_required); // host.total_RAM;
    return cpu_tb_left + ram_tb_left;
}
