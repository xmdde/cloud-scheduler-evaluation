#include <iostream>
#include <iomanip>
#include <fstream>

#include "host.h"
#include "utils.h"
#include "workload-generator.h"

void saveWorkload(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "ERR: cannot create file: " << filename << ".\n";
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
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " --generate\n"
                  << "  " << argv[0] << " --sim <load_file.csv> [output_prefix]\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "--generate") {
        for (int i = 0; i < 50; ++i) {
            WorkloadGenerator gen; 
            auto tasks = gen.generateRealisticCloudTraffic();

            std::string filename = "workloads/realistic/seed_" + std::to_string(i) + ".csv";
            saveWorkload(tasks, filename);
            std::cout << "Saved: " << filename << " (" << tasks.size() << " tasks)\n";
        }
        return 0;
    } else if (mode == "--sim") {
        if (argc < 3) {
            std::cerr << "Blad: Flaga --sim wymaga podania pliku wejsciowego .csv\n";
            return 1;
        }

        std::string workload_file = argv[2];
        auto tasks = loadWorkload(workload_file);

        std::optional<std::string> out_prefix = std::nullopt;
        if (argc >= 4) {
            out_prefix = argv[3];
        }

        auto get_path = [&](const std::string& suffix) -> std::optional<std::string> {
            if (out_prefix) {
                return *out_prefix + suffix;
            }
            return std::nullopt;
        };

        runSimulation(SchedulingMethod::ROUND_ROBIN, tasks, get_path("-RR.csv"));
        runSimulation(SchedulingMethod::BEST_FIT, tasks, get_path("-BestFit.csv"));
        runSimulation(SchedulingMethod::WORST_FIT, tasks, get_path("-WorstFit.csv"));
        runSimulation(SchedulingMethod::BFD, tasks, get_path("-BFD.csv"));
        runSimulation(SchedulingMethod::MBFD, tasks, get_path("-MBFD.csv"));

        return 0;
    } 
    else {
        std::cerr << "Nieznana flaga lub zla liczba argumentow.\n";
        return 1;
    }
}
