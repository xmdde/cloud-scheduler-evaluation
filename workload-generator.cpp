#include "workload-generator.h"
#include <random>

constexpr float SECONDS_PER_DAY = 86400.0;

Task generateRealisticVM(std::mt19937& rng, int& next_task_id, double current_time) {
    std::discrete_distribution<> vm_type_dist({80.0, 15.0, 5.0});
    int type = vm_type_dist(rng);
    
    double duration = 0.0, cpu = 0.0, ram = 0.0;

    if (type == 0) { // Krótkie zadania (5-30 min)
        cpu = std::uniform_real_distribution<double>(1.0, 4.0)(rng);
        ram = std::uniform_real_distribution<double>(2.0, 8.0)(rng);
        duration = std::uniform_real_distribution<double>(300.0, 1800.0)(rng);
    } 
    else if (type == 1) { // Średnie zadania (2-6 h)
        cpu = std::uniform_real_distribution<double>(4.0, 8.0)(rng);
        ram = std::uniform_real_distribution<double>(8.0, 16.0)(rng);
        duration = std::uniform_real_distribution<double>(7200.0, 21600.0)(rng);
    } 
    else { // Długie zadania (8-12 h)
        cpu = std::uniform_real_distribution<double>(8.0, 16.0)(rng);
        ram = std::uniform_real_distribution<double>(16.0, 32.0)(rng);
        duration = std::uniform_real_distribution<double>(28800.0, 43200.0)(rng);
    }

    Task t;
    t.id = next_task_id++;
    t.arrival_time = current_time;
    t.cpu_required = cpu;
    t.ram_required = ram;
    t.duration = duration;
    return t;
}

// ------------------------------------------------------------------------
// SCENARIUSZ A: LOW LOAD
// ------------------------------------------------------------------------
std::vector<Task> WorkloadGenerator::generateLowLoad() {
    std::vector<Task> tasks;
    double current_arrival = 0.0;
    std::exponential_distribution<double> arrivalDist(1.0 / 200.0); 

    while (current_arrival < SECONDS_PER_DAY) {
        current_arrival += arrivalDist(rng);
        if (current_arrival < SECONDS_PER_DAY) {
            tasks.push_back(generateRealisticVM(rng, next_task_id, current_arrival));
        }
    }
    return tasks;
}

// ------------------------------------------------------------------------
// SCENARIUSZ B: HIGH LOAD
// ------------------------------------------------------------------------
std::vector<Task> WorkloadGenerator::generateHighLoad() {
    std::vector<Task> tasks;
    double current_arrival = 0.0;
    std::exponential_distribution<double> arrivalDist(1.0 / 35.0); 

    while (current_arrival < SECONDS_PER_DAY) {
        current_arrival += arrivalDist(rng);
        if (current_arrival < SECONDS_PER_DAY) {
            tasks.push_back(generateRealisticVM(rng, next_task_id, current_arrival));
        }
    }
    return tasks;
}

// ------------------------------------------------------------------------
// SCENARIUSZ C: REALISTIC MIXED TRAFFIC
// ------------------------------------------------------------------------
std::vector<Task> WorkloadGenerator::generateRealisticCloudTraffic() {
    std::vector<Task> tasks;
    double current_arrival = 0.0;
    
    std::exponential_distribution<double> nightDist(1.0 / 150.0);
    std::exponential_distribution<double> dayDist(1.0 / 40.0);
    std::exponential_distribution<double> eveningDist(1.0 / 90.0);

    while (current_arrival < SECONDS_PER_DAY) {
        if (current_arrival < 28800.0) {
            current_arrival += nightDist(rng);
        } else if (current_arrival < 57600.0) {
            current_arrival += dayDist(rng);
        } else {
            current_arrival += eveningDist(rng);
        }
        
        if (current_arrival < SECONDS_PER_DAY) {
            tasks.push_back(generateRealisticVM(rng, next_task_id, current_arrival));
        }
    }
    return tasks;
}

// ------------------------------------------------------------------------
// SCENARIUSZ D: SPIKY LOAD
// ------------------------------------------------------------------------
std::vector<Task> WorkloadGenerator::generateSpikyLoad() {
    std::vector<Task> tasks;
    double current_arrival = 0.0;
    
    std::exponential_distribution<double> spikeDist(1.0 / 15.0);
    std::exponential_distribution<double> backgroundDist(1.0 / 400.0); // Tło

    while(current_arrival < SECONDS_PER_DAY) {
        bool in_spike = (current_arrival >= 28800.0 && current_arrival < 36000.0) ||
                        (current_arrival >= 50400.0 && current_arrival < 57600.0) ||
                        (current_arrival >= 72000.0 && current_arrival < 79200.0);
        
        if (in_spike) {
            current_arrival += spikeDist(rng);
        } else {
            current_arrival += backgroundDist(rng);
        }

        if (current_arrival < SECONDS_PER_DAY) {
            tasks.push_back(generateRealisticVM(rng, next_task_id, current_arrival));
        }
    }
    return tasks;
}