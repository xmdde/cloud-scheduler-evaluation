#ifndef CLOUD_SCHEDULER_EVALUATION_HOST_H
#define CLOUD_SCHEDULER_EVALUATION_HOST_H

#include <iostream>
#include <vector>
#include <queue>

#include "task.h"

enum PowerState {
    ACTIVE = 0,
    IDLE = 1,
    BOOTING = 2,
    SLEEP = 3
};

struct RunningTask {
    Task task;
    double finish_time;
};

class Host {
    PowerState state = PowerState::SLEEP;

    static constexpr double IDLE_TIMEOUT = 300.0;
    static constexpr double BOOT_DELAY = 55.0;
    
    double idle_timer = 0.0;
    double boot_timer = 0.0;

    std::vector<RunningTask> running;

    static constexpr double k = 0.3;
    static constexpr double P_MAX = 250.0;
    const double P_IDLE = k * P_MAX;
    const double P_BOOT = 350.0;

public:
    const int id;
    double total_CPU;
    double total_RAM;

    double used_CPU = 0.0;
    double used_RAM = 0.0;

    double total_energy = 0.0;  // [J]

    Host(int id_, double cpu, double ram)
        : id(id_), total_CPU(cpu), total_RAM(ram){}

    void logHostState(std::ofstream& log, const double current_time) const;
    bool canRun(const Task& t) const;
    void assignTask(Task& t, double current_time);
    void tick(double current_time, double time_step);
    void removeFinishedTasks(const double current_time);
    void updateState(const double time_step);
    double getInstantaneousPower() const;

    size_t getRunningNum() const {
        return running.size();
    }

    PowerState getState() const { return state; }
    double getExpectedPowerIncrease(const Task& t) const;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_HOST_H
