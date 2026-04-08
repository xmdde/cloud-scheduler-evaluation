#include "mbfd-scheduler.h"

#include <limits>
#include <cmath>

bool MBFDScheduler::scheduleTask(Task& task, std::vector<Host>& nodes, double current_time) {
    Host* best_host = findTargetNode(task, nodes);

    if (best_host != nullptr) {
        best_host->assignTask(task, current_time);
        return true;
    }

    return false;
}

Host* MBFDScheduler::findTargetNode(const Task& task, std::vector<Host>& nodes) const {
    Host* best_host = nullptr;
    double min_power_increase = std::numeric_limits<double>::max();
    double min_remaining_capacity = std::numeric_limits<double>::max(); 
    
    const double EPSILON = 0.0001;

    for (auto& host : nodes) {
        if (host.canRun(task)) {
            const double power_increase = host.getExpectedPowerIncrease(task);
            const double rem_cap = host.total_CPU - (host.used_CPU + task.cpu_required);

            if (power_increase < min_power_increase - EPSILON) {
                min_power_increase = power_increase;
                min_remaining_capacity = rem_cap;
                best_host = &host;
            } 

            else if (std::abs(power_increase - min_power_increase) <= EPSILON) {
                if (rem_cap < min_remaining_capacity) {
                    min_remaining_capacity = rem_cap;
                    best_host = &host;
                }
            }
        }
    }

    return best_host;
}
