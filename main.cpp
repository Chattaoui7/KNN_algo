#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include "Scheduler.h"
#include "knn_algo.h" 
#include <chrono>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

int main() {

    srand(time(NULL)); // Seed for random number generation
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Process> training_data = LoadprocessFromCSV("training_data.csv");
    std::vector<Process> testing_data = LoadprocessFromCSV("testing_data.csv");

    if (training_data.empty() || testing_data.empty()) {
        std::cerr << " ⛔ No training or testing data found. Exiting..." << std::endl;
        return 1;
    }

    std::cout << " ✅ Training data loaded successfully.\n";
    std::cout << " ➡️ ____KNN Scheduling Algorithm is Strating____ ..... \n";

    Knn knn(3);
    knn.fit(training_data);

    Scheduler scheduler;

    for (auto& p : testing_data) {
        int predicted_class = knn.predict(p);
        std::cout << " \n ➡️ The predicted class for the Process " << p.pid << "is: " << predicted_class << std::endl;
        p.queue_id = predicted_class; // Assign the predicted class to the Process
        scheduler.add_Process_to_queue(p);
    };

    scheduler.run();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\n 🕑 Execution time: " << duration << " ms" << std::endl;

    return 0;
    
} 