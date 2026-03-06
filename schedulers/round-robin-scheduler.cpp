#include "round-robin-scheduler.h"

bool RoundRobinScheduler::scheduleTask(const Task& new_task, std::vector<Host>& nodes, double current_time) {
    if (nodes.empty()) {
        return false;
    }

    size_t start_idx = curr_node_idx;
    do {
        if (nodes[curr_node_idx].canRun(new_task)) {
            nodes[curr_node_idx].assignTask(new_task, current_time);

            curr_node_idx = (curr_node_idx + 1) % nodes.size();
            return true;
        }

        curr_node_idx = (curr_node_idx + 1) % nodes.size();
    } while (curr_node_idx != start_idx);

    return false;
}
