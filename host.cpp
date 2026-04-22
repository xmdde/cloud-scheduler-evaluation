#include "host.h"

#include "task.h"

#include <fstream>
#include <iomanip>

bool Host::canRun(const Task& t) const {
    return used_CPU + t.cpu_required <= total_CPU && used_RAM + t.ram_required <= total_RAM;
}

void Host::assignTask(Task& t, double current_time) {
    if (canRun(t)) {
        used_CPU += t.cpu_required;
        used_RAM += t.ram_required;

        double actual_start_time = current_time;

        if (state == PowerState::SLEEP) {
            state = PowerState::BOOTING;
            boot_timer = BOOT_DELAY;
            actual_start_time += BOOT_DELAY;
        } else if (state == PowerState::BOOTING) {
            actual_start_time += boot_timer;
        } else if (state == PowerState::IDLE) {
            state = PowerState::ACTIVE;
            idle_timer = 0.0;
        }

        t.start_time = actual_start_time;
        double finish_time = actual_start_time + t.duration;

        running.push_back({t, finish_time});
    } else {
        std::cerr << "ERR: Host " << id << " overloaded!\n";
    }
}

double Host::getInstantaneousPower() const {
    switch (state) {
        case PowerState::SLEEP:
            return 0.0; // or P_SLEEP?

        case PowerState::IDLE:
            return P_IDLE; 

        case PowerState::BOOTING:
            return P_BOOT;

        case PowerState::ACTIVE: {
            double u = used_CPU / total_CPU;
            if (u > 1.0)
                u = 1.0; 
            return P_IDLE + (P_MAX - P_IDLE) * u;
        }
    }
    return 0.0;
}

void Host::tick(double current_time, double time_step) {
    // E [J] = P [W] * t [s]
    double current_power = getInstantaneousPower();
    total_energy += current_power * time_step;

    removeFinishedTasks(current_time);
    updateState(time_step);
}

void Host::removeFinishedTasks(const double current_time) {
    // Check for finished tasks
    auto it = running.begin();
    while (it != running.end()) {
        if (it->finish_time <= current_time) {
            used_CPU -= it->task.cpu_required;
            used_RAM -= it->task.ram_required;

            constexpr double EPSILON = 0.001;
            if (used_CPU < EPSILON) used_CPU = 0.0;
            if (used_RAM < EPSILON) used_RAM = 0.0;

            it = running.erase(it);
        } else {
            ++it;
        }
    }
}

void Host::updateState(double time_step) {
    // State machine
    if (state == PowerState::BOOTING) {
        boot_timer -= time_step;
        if (boot_timer <= 0.0) {
            state = PowerState::ACTIVE;
            boot_timer = 0.0;
        }
    } else {
        if (running.empty()) {
            if (state == PowerState::ACTIVE) {
                state = PowerState::IDLE;
                idle_timer = 0.0;
            } else if (state == PowerState::IDLE) {
                idle_timer += time_step;
                if (idle_timer >= IDLE_TIMEOUT) {
                    state = PowerState::SLEEP;
                    idle_timer = 0.0;
                }
            }
        } else {
            state = PowerState::ACTIVE;
            idle_timer = 0.0;
        }
    }
}

double Host::getExpectedPowerIncrease(const Task& t) const {
    double current_util = used_CPU / total_CPU;
    if (current_util > 1.0) current_util = 1.0;

    double future_util = (used_CPU + t.cpu_required) / total_CPU;
    if (future_util > 1.0) future_util = 1.0;

    double delta_p = (P_MAX - P_IDLE) * (future_util - current_util);

    if (state == PowerState::SLEEP) {
        return P_IDLE + delta_p;
    } else {
        return delta_p;
    }
}
