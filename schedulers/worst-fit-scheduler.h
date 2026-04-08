#ifndef CLOUD_SCHEDULER_EVALUATION_WORST_FIT_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_WORST_FIT_SCHEDULER_H

#include <vector>
#include <queue>

#include "scheduler.h"
#include "host.h"
#include "task.h"

class WorstFitScheduler : public Scheduler {
public:
    std::string getName() const override {
        return "WorstFit";
    }

    bool scheduleTask(Task& new_task, std::vector<Host>& nodes, double current_time) override;
private:
    double calculateScore(const Host& host, const Task& task) const;
    Host* findTargetNode(const Task& task, std::vector<Host>& nodes) const;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_WORST_FIT_SCHEDULER_H
