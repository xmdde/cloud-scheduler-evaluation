#include <iostream>
#include <iomanip>
#include <fstream>

#include "host.h"
#include "utils.h"
#include "workload-generator.h"

void saveWorkload(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Blad: Nie mozna utworzyc pliku " << filename << ".\n";
        return;
    }

    out << "id,arrival,cpu,ram,duration\n";
    for(const auto& t : tasks) {
        out << t.id << "," << t.arrival_time << "," 
            << t.cpu_required << "," << t.ram_required << "," 
            << t.duration << "\n";
    }
    out.close();
}

std::vector<Task> loadWorkload(const std::string& filename) {
    std::vector<Task> tasks;
    std::ifstream in(filename);
    std::string line;
    
    std::getline(in, line);
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        Task t;

        std::getline(ss, token, ','); t.id = std::stoi(token);
        std::getline(ss, token, ','); t.arrival_time = std::stod(token);
        std::getline(ss, token, ','); t.cpu_required = std::stod(token);
        std::getline(ss, token, ','); t.ram_required = std::stod(token);
        std::getline(ss, token, ','); t.duration = std::stod(token);
        t.start_time = -1.0;

        tasks.push_back(t);
    }
    return tasks;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uzycie:\n"
                  << "  " << argv[0] << " --generate\n"
                  << "  " << argv[0] << " --simulate <plik_obciazenia.csv> <prefix_wyniku>\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "--generate") {
        std::cout << "Generowanie 50 wektorow obciazenia Monte Carlo (Scenariusz Realistic)...\n";
        
        for (int i = 0; i < 5; ++i) {
            WorkloadGenerator gen; 
            auto tasks = gen.generateRealisticCloudTraffic();

            std::string filename = "workloads/seed_" + std::to_string(i) + ".csv";
            saveWorkload(tasks, filename);
            std::cout << "Saved: " << filename << " (" << tasks.size() << " tasks)\n";
        }
        return 0;
    } 
    else if (mode == "--simulate" && argc == 4) {
        std::string workload_file = argv[2];
        std::string out_prefix = argv[3];

        std::cout << "Wczytywanie zadan z pliku: " << workload_file << "\n";
        auto tasks = loadWorkload(workload_file);

        runSimulation(SchedulingMethod::ROUND_ROBIN, tasks, out_prefix + "-RR.csv");
        runSimulation(SchedulingMethod::BEST_FIT, tasks, out_prefix + "-BestFit.csv");
        runSimulation(SchedulingMethod::WORST_FIT, tasks, out_prefix + "-WorstFit.csv");
        runSimulation(SchedulingMethod::BFD, tasks, out_prefix + "-BFD.csv");
        runSimulation(SchedulingMethod::MBFD, tasks, out_prefix + "-MBFD.csv");

        return 0;
    } else {
        std::cerr << "Nieznana flaga lub zla liczba argumentow.\n";
        return 1;
    }
}
