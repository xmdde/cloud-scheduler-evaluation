#include "host.h"

#include "task.h"

#include <fstream>
#include <iomanip>

bool Host::canRun(const Task& t) const {
    return (used_CPU + t.cpu_required <= total_CPU && used_RAM + t.ram_required <= total_RAM);
}

void Host::logHostState(std::ofstream& log, double current_time) const {
    log << std::fixed << std::setprecision(2);
    log << current_time << "," << id << ","
        << used_CPU << "," << used_RAM << ","
        << getRunningNum() << "," << current_state << ","
        << total_energy_consumed << "\n";
}

void Host::assignTask(const Task& t, double current_time) {
    if (canRun(t)) {
        used_CPU += t.cpu_required;
        used_RAM += t.ram_required;

        double actual_duration = t.duration;

        if (current_state == PowerState::SLEEP) {
            current_state = PowerState::BOOTING;
            boot_timer = BOOT_DELAY;
            actual_duration += BOOT_DELAY;
        } else if (current_state == PowerState::BOOTING) {
            actual_duration += boot_timer;
        } else if (current_state == PowerState::IDLE) {
            current_state = PowerState::ACTIVE;
            idle_timer = 0.0;
        }

        running.push_back({t, current_time + actual_duration});
    } else {
        std::cerr << "ERR: Host " << id << " overloaded in SLOT mode!\n";
    }
}

double Host::getInstantaneousPower() const {
    switch (current_state) {
        case PowerState::SLEEP:
            return 0.0; // or P_SLEEP?

        case PowerState::IDLE:
            return P_IDLE; 

        case PowerState::BOOTING:
            return P_BOOT;

        case PowerState::ACTIVE: {
            double utilization = used_CPU / total_CPU;
            if (utilization > 1.0)
                utilization = 1.0; 
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

    // State machine
    if (current_state == PowerState::BOOTING) {
        boot_timer -= time_step;
        if (boot_timer <= 0.0) {
            current_state = PowerState::ACTIVE;
            boot_timer = 0.0;
        }
    } else {
        if (running.empty()) {
            if (current_state == PowerState::ACTIVE) {
                current_state = PowerState::IDLE;
                idle_timer = 0.0;
            } else if (current_state == PowerState::IDLE) {
                idle_timer += time_step;
                if (idle_timer >= IDLE_TIMEOUT) {
                    current_state = PowerState::SLEEP;
                    idle_timer = 0.0;
                }
            }
        } else {
            current_state = PowerState::ACTIVE;
            idle_timer = 0.0;
        }
    }
}

double Host::getExpectedPowerIncrease(const Task& t) const {
    if (current_state == PowerState::SLEEP) {
        return P_BOOT;  // - sleep
    } else {
        double current_util = used_CPU / total_CPU;
        if (current_util > 1.0)
            current_util = 1.0;

        double future_util = (used_CPU + t.cpu_required) / total_CPU;
        if (future_util > 1.0)
            future_util = 1.0;

        // Beloglazov:
        return (P_MAX - P_IDLE) * (future_util - current_util);
    }
}
