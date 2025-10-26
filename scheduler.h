#ifndef CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H

#include <vector>

#include "task.h"
#include "node.h"

class Scheduler {
    // training data: task.cpu_required << "," << task.ram_required << "," << node.used_CPU << "," << node.used_RAM << "," << node.total_energy << ","
    // << (finish_time - task.arrival_time)??? << "," << delta_energy << "\n";
public:
    virtual void scheduleTask(const Task task, std::vector<Node>& nodes, double current_time) = 0;
    virtual std::string getName() const = 0;
    virtual ~Scheduler() = default;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
