#include "node.h"

#include "task.h"

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
    } else {  // Mode::QUEUE
        if (running.empty() && canRun(t)) {
            used_CPU += t.cpu_required;
            used_RAM += t.ram_required;
            running.push_back({t, current_time + t.duration});
        } else {
            queue.push(t);
        }
    }
}

void Node::finishTask(const Task& t) {
    total_energy += calculateDeltaEnergy(t);

    used_CPU -= t.cpu_required;
    used_RAM -= t.ram_required;

    if (used_RAM < 0) {
        used_RAM = 0;
    }
    if (used_CPU < 0) {
        used_CPU = 0;
    }
}


void Node::update(double current_time) {
    // przy zalozeniu, ze zwiekszamy o time_step
    total_energy += 75.0;  // [J]

    auto it = running.begin();
    while (it != running.end()) {
        if (it->finish_time <= current_time) {
            finishTask(it->task);
            it = running.erase(it);

            // w trybie QUEUE: uruchom następny tylko, gdy node jest pusty
            if (mode == Mode::QUEUE && running.empty() && !queue.empty()) {
                Task next = queue.front();
                queue.pop();
                used_CPU += next.cpu_required;
                used_RAM += next.ram_required;
                running.push_back({next, current_time + next.duration});
            }
        } else {
            ++it;
        }
    }
}

double Node::calculateDeltaEnergy(const Task& t) const {
    const int tmp_k = 75;
    return tmp_k * t.cpu_required/total_CPU * t.duration;
}
