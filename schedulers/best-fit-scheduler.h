#ifndef CLOUD_SCHEDULER_EVALUATION_BEST_FIT_SCHEDULER_H
#define CLOUD_SCHEDULER_EVALUATION_BEST_FIT_SCHEDULER_H

#include <vector>
#include <queue>

#include "scheduler.h"
#include "host.h"
#include "task.h"

class BestFitScheduler : public Scheduler {
public:
    std::string getName() const override {
        return "BestFit";
    }

    bool scheduleTask(Task& new_task, std::vector<Host>& nodes, double current_time) override;
private:
    double calculateScore(const Host& host, const Task& task) const;
    Host* findBestNode(const Task& task, std::vector<Host>& nodes) const;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_BEST_FIT_SCHEDULER_H
