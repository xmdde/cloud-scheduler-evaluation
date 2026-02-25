#ifndef CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H

#include <vector>

#include "task.h"
#include "host.h"

class Scheduler {
public:
    virtual bool scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) = 0;
    virtual std::string getName() const = 0;
    virtual ~Scheduler() = default;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
