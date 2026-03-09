#include "best-fit-scheduler.h"

#include <limits>

bool BestFitScheduler::scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) {
    Host* best_host = findBestNode(task, nodes);

    if (best_host != nullptr) {
        best_host->assignTask(task, current_time);
        return true;
    }

    return false;
}

Host* BestFitScheduler::findBestNode(const Task& task, std::vector<Host>& nodes) const {
    Host* best_host = nullptr;
    double min_score = std::numeric_limits<double>::max();

    for (auto& host : nodes) {
        if (host.getState() != PowerState::SLEEP && host.canRun(task)) {
            const double score = calculateScore(host, task);
            if (score < min_score) {
                min_score = score;
                best_host = &host;
            }
        }
    }

    if (best_host != nullptr) {
        return best_host;
    }

    for (auto& host : nodes) {
        if (host.getState() == PowerState::SLEEP && host.canRun(task)) {
            const double score = calculateScore(host, task);
            if (score < min_score) {
                min_score = score;
                best_host = &host;
            }
        }
    }

    return best_host;
}

double BestFitScheduler::calculateScore(const Host& host, const Task& task) const {
    const double cpu_tb_left = host.total_CPU - (host.used_CPU + task.cpu_required);
    const double ram_tb_left = host.total_RAM - (host.used_RAM + task.ram_required);

    return cpu_tb_left + ram_tb_left;
}
