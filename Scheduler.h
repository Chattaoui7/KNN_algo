#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "knn_algo.h"
#include <queue>
#include <unordered_map>
#include <iostream>
#include <cmath>


class Scheduler {
    private :
        // Number of queues
        unordered_map<int, vector<Process>> queues;

public:

    void add_Process_to_queue(const Process& p){
        // Check if the queue for the given queue_id exists, if not create it
        queues[p.queue_id].push_back(p);
    }

    void run(){

        for(auto queue_pair : queues){
            // Process the queue
                std::cout << "\n ➡️ Running queue " << queue_pair.first << ":\n"; 

                if ( queue_pair.first == 0 ){  // class 0 --> algo FIFO
                    FIFO(queue_pair.second);  // --> add to FIFO
                } else if (queue_pair.first == 1) { // class 1 --> algo Round Robin
                    RounfRobin(queue_pair.second, 2); // --> add to RR , time_quantum = 2
                } else { // class 2 --> algo SJF
                   SJF(queue_pair.second); // add to algo SJF
            }
        }
    };

    void FIFO(std::vector<Process>& queue) {
        // Implement FIFO scheduling algorithm here
        // This is just a placeholder for the actual implementation
        std::sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
            return a.arrival_time < b.arrival_time;
        });

        int current_time = 0;

        std::cout << "____FIFO scheduling algorithm___:\n";

        for (const auto& process : queue) {
            if (current_time < process.arrival_time) {
                current_time = process.arrival_time;

                int waiting_time = current_time - process.arrival_time;
                int turnaround_time = waiting_time + process.cpu_brust_time;
                current_time += process.cpu_brust_time; // Simulate processing time

                std::cout << "\n Process " << process.pid
                          << " | Waiting Time: " << waiting_time
                          << " | Turnaround Time: " << turnaround_time << "\n";
            }
        }
        std::cout << "\n ✅All processes completed at time " << current_time << "\n";
    };

    void RounfRobin(vector<Process>& queue, int time_quantum) {
        // Implement Round Robin scheduling algorithm here
        // This is just a placeholder for the actual implementation
        std::queue<Process> rr_process_queue;

        int time = 0 ; 
        std::map<int , int> raimaning_time ;

        for (const auto& process : queue) {
            rr_process_queue.push(process);
            raimaning_time[process.pid] = process.cpu_brust_time ; // Initialize remaining time for each process
        };
        // Process the queue in a round-robin manner
        std::cout << "____Round Robin scheduling algorithm___:\n";

        while (!rr_process_queue.empty()){

            Process current_process = rr_process_queue.front();
            rr_process_queue.pop();

            if(raimaning_time[current_process.pid] > time_quantum){

                int run_time  = std::min(time_quantum , raimaning_time[current_process.pid]);
                std::cout << "\n Process " << current_process.pid << " runs from time "
                          << time << " to " << time + run_time << "\n";

                time += run_time ; // Update the current time
                raimaning_time[current_process.pid] -= run_time ; // Decrease the remaining time
                

                if (raimaning_time[current_process.pid] > 0 ) {
                    rr_process_queue.push(current_process); // Re-add the process to the queue if it still has remaining time
                } else {
                    std::cout << "\n Process " << current_process.pid << " completed at time " << time << "\n";
                }
            }
        }
        std::cout << "\n ✅ All processes completed at time " << time << "\n";
    } ;

    void SJF(std::vector<Process>& queue) {
        // Implement Shortest Job First (SJF) scheduling algorithm here
        std::sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
            return a.cpu_brust_time < b.cpu_brust_time;
        });

        int current_time = 0;

        std::cout << "____Shortest Job First scheduling algorithm___:\n";

        for (const auto& process : queue) {
            if (current_time < process.arrival_time) {
                current_time = process.arrival_time;
            }

            int waiting_time = current_time - process.arrival_time;
            int turnaround_time = waiting_time + process.cpu_brust_time;
            current_time += process.cpu_brust_time; // Simulate processing time

            std::cout << "\n Process " << process.pid
                      << " | Waiting Time: " << waiting_time
                      << " | Turnaround Time: " << turnaround_time << "\n";
        }

        std::cout << "\n ✅All processes completed at time " << current_time << "\n";
    };


};

#endif // SCHEDULER_H