#include "node.h"

#include "task.h"

bool Node::canRun(const Task& t) const {
    if (mode == Mode::SLOT) {
        return (used_CPU + t.cpu_required <= total_CPU && used_RAM + t.ram_required <= total_RAM);
    } else {
        return true;
    }
}

void Node::assignTask(const Task& t, double current_time) {
    if (mode == Mode::SLOT) {
        if (canRun(t)) {
            used_CPU += t.cpu_required;
            used_RAM += t.ram_required;
            running.push_back({t, current_time + t.duration});
        } else {
            std::cerr << "ERR: Node " << id << " overloaded in SLOT mode!\n";
        }
    } else { 
        if (running.empty()) {
            used_CPU += t.cpu_required;
            used_RAM += t.ram_required;
            running.push_back({t, current_time + t.duration});
        } else {
            waiting_queue.push(t);
        }
    }
}

// [W]
double Node::getInstantaneousPower() const {
    if (used_CPU == 0) {
        return P_IDLE;
        // to think about 0.0?
    }

    // P(u) = k * P_max + (1-k) * P_max * u 
    //      = P_idle + (P_max - P_idle) * utilization
    double utilization = used_CPU / total_CPU;
    if (utilization > 1.0) {
        utilization = 1.0;
    }

    return P_IDLE + (P_MAX - P_IDLE) * utilization;
}

void Node::tick(double current_time, double time_step) {
    // 1. FIZYKA: Oblicz zużytą energię w tym kroku czasowym
    // Energia [J] = Moc [W] * Czas [s]
    double current_power = getInstantaneousPower();
    total_energy_consumed += current_power * time_step;

    // Check for finished tasks
    auto it = running.begin();
    while (it != running.end()) {
        if (it->finish_time <= current_time) {
            used_CPU -= it->task.cpu_required;
            used_RAM -= it->task.ram_required;

            if (used_CPU < 0.001) used_CPU = 0.0;
            if (used_RAM < 0.001) used_RAM = 0.0;

            it = running.erase(it);

            // Start the next task if in queue mode
            if (mode == Mode::QUEUE && !waiting_queue.empty() && running.empty()) {
                Task next = waiting_queue.front();
                waiting_queue.pop();

                used_CPU += next.cpu_required;
                used_RAM += next.ram_required;
                running.push_back({next, current_time + next.duration});
            }
        } else {
            ++it;
        }
    }
}
