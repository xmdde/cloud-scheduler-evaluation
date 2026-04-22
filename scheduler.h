#ifndef CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H

#include <vector>

#include "task.h"
#include "host.h"

class Scheduler {
protected:
    std::string name;
public:
    explicit Scheduler(std::string n) : name(std::move(n)) {}
    virtual std::string_view getName() const { return name; }
    virtual bool scheduleTask(Task& task, std::vector<Host>& nodes, double current_time) = 0;
    virtual ~Scheduler() = default;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_SCHEDULER_H
