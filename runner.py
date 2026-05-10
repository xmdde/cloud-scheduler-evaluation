import subprocess
import csv
import os
from concurrent.futures import ThreadPoolExecutor, as_completed
from tqdm import tqdm

# ==========================================
# KONFIGURACJA EKSPERYMENTU
# ==========================================
EXEC_PATH = "./cloud-sim"
SCENARIO = "realistic" # high/low/spiky/realistic

WORKLOAD_DIR = f"workloads/{SCENARIO}"
OUTPUT_DIR = f"output/{SCENARIO}"
SUMMARY_FILE = f"output/summary_results_{SCENARIO}.csv"

ALGORITHMS = ["RoundRobin", "BestFit", "WorstFit", "BFD", "MBFD"]

SEEDS = list(range(50)) 

MAX_WORKERS = 8

# ==========================================
# FUNKCJA ODPALAJĄCA SYMULACJĘ DLA DANEGO PLIKU
# ==========================================
def run_simulation(seed_val):
    workload_path = os.path.join(WORKLOAD_DIR, f"seed_{seed_val}.csv")

    out_prefix = os.path.join(OUTPUT_DIR, f"seed_{seed_val}")

    # Budujemy komendę: ./cloud-sim --sim workloads/realistic/seed_X.csv output/realistic/seed_X
    cmd = [EXEC_PATH, "--sim", workload_path, out_prefix]

    results = []

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)

        for line in result.stdout.strip().split('\n'):
            tokens = line.split()
            if not tokens:
                continue

            if tokens[0] in ALGORITHMS and len(tokens) >= 5:
                results.append({
                    "Algorithm": tokens[0],
                    "Seed": seed_val,
                    "Total_Energy": float(tokens[1]),
                    "Makespan": float(tokens[2]),
                    "Avg_Wait_Time": float(tokens[3]),
                    "Avg_Utilization": float(tokens[4])
                })

        if not results:
            return {"Error": f"Brak wyników dla {workload_path}. Sprawdź wyjście C++."}

        return results

    except subprocess.CalledProcessError as e:
        return {"Error": f"Crash (błąd C++) na pliku {workload_path}:\n{e.stderr}"}
    except Exception as e:
        return {"Error": str(e)}

# ==========================================
# GŁÓWNA PĘTLA
# ==========================================
if __name__ == "__main__":
    fieldnames = [
        "Algorithm", "Seed", "Total_Energy", "Makespan",
        "Avg_Wait_Time", "Avg_Utilization"
    ]

    os.makedirs(OUTPUT_DIR, exist_ok=True)

    print(f"======================================================")
    print(f" START: Wielowątkowy Runner Symulacji")
    print(f" Scenariusz: {SCENARIO}")
    print(f"======================================================")

    with open(SUMMARY_FILE, mode='w', newline='') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()
        
        with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
            futures = {executor.submit(run_simulation, s): s for s in SEEDS}

            for future in tqdm(as_completed(futures), total=len(SEEDS), desc="Postęp Symulacji"):
                output = future.result()

                if isinstance(output, dict) and "Error" in output:
                    print(f"\n[BŁĄD] {output['Error']}")
                else:
                    for row in output:
                        writer.writerow(row)
                    csv_file.flush()

    print(f"\nGotowe! Zbiorcze wyniki zapisane w: {SUMMARY_FILE}")
    print(f"Szczegółowe logi zadań (*-tasks.csv) znajdziesz w: {OUTPUT_DIR}/")
