#ifndef CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H

#include <vector>

#include "task.h"
#include "node.h"

class Scheduler {
public:
    virtual void scheduleTask(const Task& task, std::vector<Node>& nodes, double current_time) = 0;
    virtual std::string getName() const = 0;
    virtual ~Scheduler() = default;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
