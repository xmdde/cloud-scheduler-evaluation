#include "node.h"

bool Node::canRun(const Task& t) const {
    if (mode == Mode::SLOT) {
        return (used_CPU + t.cpu_required <= total_CPU && used_RAM + t.ram_required <= total_RAM);
    } else {
        return true;
    }
}

void Node::assignTask(const Task& t, const double current_time) {
    if (mode == Mode::SLOT) {
        if (canRun(t)) {
            used_CPU += t.cpu_required;
            used_RAM += t.ram_required;
            running.push_back({t, current_time + t.duration});
        } else {
            std::cerr << "|Node " << id << "| Not enough resources for task " << t.id << "\n";
        }
    } else { // Mode::QUEUE
        if (running.empty() && canRun(t)) {
            // used_CPU += t.cpu_required;
            // used_RAM += t.ram_required;
            running.push_back({t, current_time + t.duration});
        } else {
            queue.push(t);
        }
    }
}

void Node::finishTask(const Task& t) {
    if (mode == Mode::SLOT) {  // ?
        used_CPU -= t.cpu_required;
        used_RAM -= t.ram_required;
    }
}

void Node::update(const double current_time) {
    auto it = running.begin();
    while (it != running.end()) {
        if (it->finish_time <= current_time) {
            finishTask(it->task);
            it = running.erase(it);

            // if in QUEUE mode, start next task
            if (mode == Mode::QUEUE && !queue.empty()) {  // ? running.empty()
                Task next = queue.front();
                queue.pop();
                running.push_back({next, current_time + next.duration});
            }
        } else {
            ++it;
        }
    }
}
