#ifndef CLOUD_SCHEDULER_EVALUATION_HOST_H
#define CLOUD_SCHEDULER_EVALUATION_HOST_H

#include <iostream>
#include <vector>
#include <queue>

#include "task.h"

enum class Mode { SLOT, QUEUE };

enum class PowerState {
    ACTIVE,
    IDLE,
    SLEEP
};

struct RunningTask {
    Task task;
    double finish_time;
};

class Host {
    const Mode mode;
    PowerState current_state = PowerState::IDLE;

    static constexpr double IDLE_TIMEOUT = 30.0;
    double idle_timer = 0.0;

    std::vector<RunningTask> running;

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

    void logHostState(std::ofstream& log, const double current_time) const;
    bool canRun(const Task& t) const;
    void assignTask(const Task& t, double current_time);
    void tick(double current_time, double time_step);
    double getInstantaneousPower() const;

    bool isIdle() const {  // to replace with enum checking
        return running.empty();
    }

    size_t getRunningNum() const {
        return running.size();
    }
};

#endif  // CLOUD_SCHEDULER_EVALUATION_HOST_H
