# Cloud-Sim: Symulator Szeregowania Zadań

## Opis projektu
Cloud-Sim to narzędzie do symulacji algorytmów szeregowania zadań w chmurze. Pozwala generować obciążenia o różnej charakterystyce i testować wydajność algorytmów pod kątem efektywności energetycznej.

## Kompilacja
Projekt korzysta z `make` oraz kompilatora `g++`.
- `make`       : Kompiluje projekt i tworzy plik wykonywalny `cloud-sim`.
- `make clean` : Usuwa plik wykonywalny i czyści projekt.

## Instrukcja obsługi

### 1. Generowanie danych wejściowych
Tryb tworzy 50 plików `.csv` z danymi dla wybranego scenariusza.
- **Komenda:** `./cloud-sim --generate <scenariusz>`
- **Scenariusze:** `realistic`, `low`, `high`, `spiky`
- **Przykład:** `./cloud-sim --generate spiky`

### 2. Uruchomienie symulacji
Tryb wykonuje obliczenia dla przygotowanego pliku wejściowego przy użyciu dostępnych algorytmów.
- **Komenda:** `./cloud-sim --sim <plik_wejsciowy.csv> [prefiks_wynikow]`
- **Przykład:** `./cloud-sim --sim workloads/spiky/seed_0.csv test_1`

Po zakończeniu symulacji program generuje raport zawierający kluczowe metryki wydajności dla każdego algorytmu, obejmujące zużycie energii, całkowity czas trwania symulacji, średni czas oczekiwania zadań oraz poziom utylizacji aktywnych węzłów.