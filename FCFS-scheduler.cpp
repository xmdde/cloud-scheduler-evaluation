#include "FCFS-scheduler.h"

void FCFSScheduler::scheduleTask(const Task& new_task, std::vector<Node>& nodes, double current_time) {
    queue.push(new_task);

    std::queue<Task> remaining;
    while (!queue.empty()) {
        Task t = queue.front();
        queue.pop();

        bool assigned = false;
        for (auto& n : nodes) {
            if (n.canRun(t)) {
                n.assignTask(t, current_time);
                assigned = true;
                break;
            }
        }

        if (!assigned) {
            remaining.push(t);
        }
    }

    queue = std::move(remaining);
}
