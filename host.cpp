#include "host.h"

#include "task.h"

#include <fstream>
#include <iomanip>

bool Host::canRun(const Task& t) const {
    if (mode == Mode::SLOT) {
        return (used_CPU + t.cpu_required <= total_CPU && used_RAM + t.ram_required <= total_RAM);
    } else if (running.empty()) {
        return true;
    }
    return false;
}

void Host::logHostState(std::ofstream& log, double current_time) const {
    log << std::fixed << std::setprecision(2);
    log << current_time << "," << id << ","
        << used_CPU << "," << used_RAM << ","
        << getRunningNum() << ","
        << total_energy_consumed << "\n";
}

void Host::assignTask(const Task& t, double current_time) {
    if (mode == Mode::SLOT) {
        if (canRun(t)) {
            used_CPU += t.cpu_required;
            used_RAM += t.ram_required;
            running.push_back({t, current_time + t.duration});
        } else {
            std::cerr << "ERR: Host " << id << " overloaded in SLOT mode!\n";
        }
    } else {
        // TODO
    }
}

double Host::getInstantaneousPower() const {
    switch (current_state) {
        case PowerState::SLEEP:
            return 0.0; 

        case PowerState::IDLE:
            return P_IDLE; 

        case PowerState::ACTIVE: {
            // P(u) = P_idle + (P_max - P_idle) * u
            double utilization = used_CPU / total_CPU;
            if (utilization > 1.0) {
                utilization = 1.0; 
            }

            return P_IDLE + (P_MAX - P_IDLE) * utilization;
        }
    }
    return 0.0; 
}

void Host::tick(double current_time, double time_step) {
    // E [J] = P [W] * t [s]
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
        } else {
            ++it;
        }
    }

    if (running.empty()) {
        if (current_state == PowerState::ACTIVE) {
            current_state = PowerState::IDLE;
            idle_timer = 0.0;
        } 
        else if (current_state == PowerState::IDLE) {
            idle_timer += time_step;
            if (idle_timer >= IDLE_TIMEOUT) {
                current_state = PowerState::SLEEP;
                std::cout << "usypiam node id " << id << "!\n";
                idle_timer = 0.0;
            }
        }
    } else {
        current_state = PowerState::ACTIVE;
        idle_timer = 0.0;
    }
}
