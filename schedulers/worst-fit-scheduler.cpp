#include "worst-fit-scheduler.h"

#include <limits>

bool WorstFitScheduler::scheduleTask(Task& task, std::vector<Host>& nodes, double current_time) {
    Host* target_host = findTargetNode(task, nodes);

    if (target_host != nullptr) {
        target_host->assignTask(task, current_time);
        return true;
    }

    return false;
}

Host* WorstFitScheduler::findTargetNode(const Task& task, std::vector<Host>& nodes) const {
    Host* target_host = nullptr;
    double max_remaining_capacity = -1.0;

    for (auto& host : nodes) {
        if (host.getState() != PowerState::SLEEP && host.canRun(task)) {
            const double score = calculateScore(host, task);

            if (score > max_remaining_capacity) {
                max_remaining_capacity = score;
                target_host = &host;
            }
        }
    }

    if (target_host == nullptr) {
        for (auto& host : nodes) {
            if (host.getState() == PowerState::SLEEP && host.canRun(task)) {
                const double score = calculateScore(host, task);

                if (score > max_remaining_capacity) {
                    max_remaining_capacity = score;
                    target_host = &host;
                }
            }
        }
    }

    return target_host;
}

double WorstFitScheduler::calculateScore(const Host &host, const Task &task) const {
    const double cpu_tb_left = host.total_CPU - (host.used_CPU + task.cpu_required);
    const double ram_tb_left = host.total_RAM - (host.used_RAM + task.ram_required);

    return cpu_tb_left + ram_tb_left;
}
