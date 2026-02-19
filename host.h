#ifndef CLOUD_SCHEDULER_EVALUATION_HOST_H
#define CLOUD_SCHEDULER_EVALUATION_HOST_H

#include <iostream>
#include <vector>
#include <queue>

#include "task.h"

enum class Mode { SLOT, QUEUE };

struct RunningTask {
    Task task;
    double finish_time;
};

class Host {
    const Mode mode;

    std::vector<RunningTask> running;
    std::queue<Task> waiting_queue;  // queue tasks if host is busy (QUEUE mode)

    const double P_IDLE = 75.0;
    const double P_MAX = 125.0;

public:
    const int id;
    double total_CPU;
    double total_RAM;

    double used_CPU = 0.0;
    double used_RAM = 0.0;
    double total_energy_consumed = 0.0;  // [J]

    Host(int id_, double cpu, double ram, Mode m)
        : id(id_), total_CPU(cpu), total_RAM(ram), mode(m) {}

    bool canRun(const Task& t) const;
    void assignTask(const Task& t, double current_time);

    void tick(double current_time, double time_step);

    double getInstantaneousPower() const;

    bool isIdle() const {
        return running.empty() && waiting_queue.empty();
    }

    size_t getRunningNum() const {
        return running.size();
    }
};

#endif  // CLOUD_SCHEDULER_EVALUATION_HOST_H
