#pragma once

#include <vector>
#include <queue>

#include "scheduler.h"
#include "node.h"
#include "task.h"

class FCFSScheduler : public Scheduler {
public:
    std::queue<Task> queue;  // pamiętamy o zaległych zadaniach

    std::string getName() const override {
        return "FCFS";
    }

    void scheduleTask(const Task& new_task, std::vector<Node>& nodes, double current_time) override {
        // 1. dodaj nowe zadanie do kolejki (bo FCFS)
        queue.push(new_task);

        // 2. spróbuj przydzielić zadania z kolejki po kolei
        std::queue<Task> remaining;

        while (!queue.empty()) {
            Task t = queue.front();
            queue.pop();

            bool assigned = false;
            for (auto& n : nodes) {
                if (n.canRun(t)) {
                    n.assignTask(t, current_time);
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                // brak wolnych zasobów — zostaje w kolejce
                remaining.push(t);
            }
        }

        // 3. zaktualizuj kolejkę
        queue = std::move(remaining);
    }
};
