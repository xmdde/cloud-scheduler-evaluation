#ifndef CLOUD_SCHEDULER_EVALUATION_NODE_H
#define CLOUD_SCHEDULER_EVALUATION_NODE_H

#include <queue>
#include <iostream>

#include "task.h"

enum class Mode { SLOT, QUEUE };

struct RunningTask {
    Task task;
    double finish_time;
};

class Node {
    int id;
    double total_CPU;
    double total_RAM;
    double used_CPU = 0;
    double used_RAM = 0;
    Mode mode;

    std::vector<RunningTask> running;
    std::queue<Task> queue;  // used only in QUEUE mode, change to pointer?

public:
    Node(int id_, double cpu, double ram, Mode m)
        : id(id_), total_CPU(cpu), total_RAM(ram), mode(m) {}

    bool canRun(const Task& t) const;
    void assignTask(const Task& t, const double current_time);
    void finishTask(const Task& t);
    void update(const double current_time);

    bool isIdle() const {
        return running.empty() && queue.empty();
    }
};

#endif  // CLOUD_SCHEDULER_EVALUATION_NODE_H
