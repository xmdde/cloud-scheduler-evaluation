#include "round-robin-scheduler.h"

void RoundRobinScheduler::scheduleTask(const Task& new_task, std::vector<Node>& nodes, double current_time) {
    // assuming that nodes are in queue mode! change to while loop
    nodes[node_index].assignTask(new_task, current_time);
    node_index = (node_index + 1) % nodes.size();
}
