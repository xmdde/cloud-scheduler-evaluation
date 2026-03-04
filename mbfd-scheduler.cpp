#include "mbfd-scheduler.h"

#include <limits>

bool MBFDScheduler::scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) {
    int best_host_idx = -1;
    double min_power_increase = std::numeric_limits<double>::max();
    double min_remaining_capacity = std::numeric_limits<double>::max(); // TIE-BREAKER

    for (int i = 0; i < nodes.size(); ++i) {
        Host& host = nodes[i];

        if (host.canRun(task)) {
            double power_increase = host.getExpectedPowerIncrease(task);
            double rem_cap = host.total_CPU - (host.used_CPU + task.cpu_required);

            if (power_increase < min_power_increase - 0.0001) {
                min_power_increase = power_increase;
                min_remaining_capacity = rem_cap;
                best_host_idx = i;
            } 
            else if (std::abs(power_increase - min_power_increase) <= 0.0001) {
                if (rem_cap < min_remaining_capacity) {
                    min_remaining_capacity = rem_cap;
                    best_host_idx = i;
                }
            }
        }
    }

    if (best_host_idx != -1) {
        nodes[best_host_idx].assignTask(task, current_time);
        return true;
    }

    return false;
}
