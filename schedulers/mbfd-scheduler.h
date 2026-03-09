#ifndef CLOUD_SCHEDULER_EVALUATION_MBFD_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_MBFD_SCHEDULER_H

#include "scheduler.h"
#include <vector>

class MBFDScheduler : public Scheduler {
public:
    bool scheduleTask(const Task& task, std::vector<Host>& nodes, double current_time) override;

    std::string getName() const override {
        return "MBFD";
    }
private:
    Host* findTargetNode(const Task& task, std::vector<Host>& nodes) const;
};

#endif // CLOUD_SCHEDULER_EVALUATION_MBFD_SCHEDULER_H
