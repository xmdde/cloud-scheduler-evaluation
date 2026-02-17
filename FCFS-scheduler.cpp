#include "FCFS-scheduler.h"

bool FCFSScheduler::scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) {
    for (auto& node : nodes) {
        if (node.canRun(task)) {
            node.assignTask(task, current_time);
            return true;
        }
    }
    return false;
}
