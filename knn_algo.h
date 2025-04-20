#ifndef __KNN_ALGO_H__
#define __KNN_ALGO_H__

#include <queue>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <omp.h>

using namespace std;
struct Process {

    int pid ; 
    int arrival_time ;
    int memory_required ;
    double cpu_brust_time ;
    int priority ;
    int queue_id = -1 ; // Default value for queue_id

    Process(int pid, int arrival_time, int memory_required, double cpu_brust_time, int priority, int queue_id) {
        this->pid = pid;
        this->arrival_time = arrival_time;
        this->memory_required = memory_required;
        this->cpu_brust_time = cpu_brust_time;
        this->priority = priority;
        this->queue_id = queue_id;
    }
} ;

std::vector<Process>LoadprocessFromCSV(const std::string& filename) {
    std::vector<Process> processes;
    std::ifstream file(filename);
    std::string line;

    if(!file.is_open()){
        std::cout << " ⛔ Error opening file :" << filename << std::endl; 
        return processes;
    }
    // Skip the header line
    std::getline(file ,line);
    std::cout << " 📂 Loading data from file : " << filename << "......" << std::endl;
    
    while(std::getline(file, line )){
        std::istringstream ss(line);
        std::string token ;
        std::vector<int> values ;

        while(std::getline(ss ,token , ',')){

            values.push_back(std::stoi(token));
        }

        if (values.size() == 6) {
            Process p(values[0], values[1], values[2], values[3], values[4], values[5]);
            processes.push_back(p);
        }
    }
    file.close();
    std::cout << " ✅ Data loaded successfully from file: " << filename << std::endl;

    return processes;
}

class Knn {

    private: 
        int k; // Number of neighbors to consider
        std::vector<Process> data; // Data points for training
        double euclidean_distance(const Process& p1 , const Process& p2) {
        
            // Calculate the Euclidean distance between two Processes
            double dis_1 = pow(p1.arrival_time - p2.arrival_time, 2) + pow(p1.memory_required - p2.memory_required, 2);
            double dis_2 = pow(p1.priority - p2.priority, 2) + pow(p1.cpu_brust_time - p2.cpu_brust_time, 2) ;
            double distance = sqrt(dis_1 + dis_2) ;
            return distance;
        }
    
    public :

        Knn(int k ) : k(k) {} ;

        void fit(const std::vector<Process>& data) {
            // Store the data points in the class for later use
            this->data = data;
        } ;

        int predict(const Process& new_Process) {
            // Calculate distances from the new Process to all data points
            std::vector<std::vector<pair<double, int>>> local_threads_shared_distances(omp_get_max_threads());
            #pragma omp parallel  // Parallelize the loop for better performance
            {   
                int thrad_id = omp_get_thread_num(); // Get the thread ID
                std::vector<std::pair<double, int>>& local = local_threads_shared_distances[thrad_id]; // Get the thread-local vector for distances
                #pragma omp for

                for (const auto& previeus_Process  : data) {
                    double distance = euclidean_distance(new_Process, previeus_Process);
                    local.push_back(std::make_pair(distance, previeus_Process.queue_id)); // Store distance and index of class fils
                };
            }   
            std::vector<std::pair<double, int>> distances;
            
            for (auto &local : local_threads_shared_distances) {
                
                distances.insert(distances.end() , local.begin()  , local.end()); // Store distance and index of class fils
            }
            // Sort distances and get the indices of the k nearest neighbors
            std::sort(distances.begin(), distances.end());

            unordered_map<int ,int > neighbors_indices;

            for (int i = 0; i < k; ++i) {
                neighbors_indices[distances[i].second]++;
            };

            int best_class = -1;
            int max_count = 0;
            for(const auto& count : neighbors_indices) {
                // Find the class with the most occurrences among the k nearest neighbors
                if (count.second > max_count) {
                    max_count = count.second; // Update max count
                    best_class = count.first; // Update best class
                };
            };
            return best_class; // Return the class with the most occurrences among the k nearest neighbors
            
        };
};

#endif 