# 🤖 KNN-Based CPU Scheduling Simulator

This project implements a **machine learning-based CPU scheduling simulator** using the **K-Nearest Neighbors (KNN)** algorithm to dynamically choose the most suitable scheduling strategy for each process: **FIFO** or **Round Robin (RR)**.

## 🚀 Project Overview

In traditional OS schedulers, processes are scheduled using fixed algorithms. Here, we enhance flexibility by using KNN classification to predict the best scheduling class (queue 0 = FIFO, queue 1 = Round Robin , queue 2 = SJF ) based on process features.

### 📊 Input Features for Each Process
- `pid`: Process ID
- `arrival_time`: Time when process enters the system
- `memory_required`: Amount of memory needed
- `cpu_brust_time`: Estimated CPU time required
- `priority`: Priority of the process
- `queue_id`: Target queue (used for training data)

---

## 🧠 How It Works

### ⚙️ KNN Classification
1. **Training Phase**:
   - Load historical process data (`training_data.csv`)
   - Fit KNN with process attributes and their target queue

2. **Prediction Phase**:
   - Load new processes (`testing_data.csv`)
   - Predict the best scheduling queue using KNN
   - Assign each process to either FIFO or RR queue

### 🗂 Scheduling Phase

- **FIFO Scheduler**: Executes processes in the order of arrival .
- **Round Robin Scheduler**: Executes with time slicing (quantum = 2) .
- **Shortest Job First** : Executes with  CPU burst time.

### ⛓ Example Output

 📂 Loading data from file : training_data.csv......
 ✅ Data loaded successfully from file: training_data.csv
 📂 Loading data from file : testing_data.csv......
 ✅ Data loaded successfully from file: testing_data.csv
 ✅ Training data loaded successfully.
 👉 ____KNN Scheduling Algorithm is Strating____ .....

 ➡️ The predicted class for the Process 30 is: 1

 ➡️ The predicted class for the Process 31 is: 2

 ➡️ The predicted class for the Process 32 is: 2

 ➡️ The predicted class for the Process 33 is: 2

 ➡️ The predicted class for the Process 34 is: 2

 ➡️ The predicted class for the Process 35 is: 2

 ➡️ The predicted class for the Process 36 is: 2

 ➡️ Running queue 2:
____Shortest Job First scheduling algorithm___:

 Process 32 | Waiting Time: 0 | Turnaround Time: 2

 Process 31 | Waiting Time: 4 | Turnaround Time: 7

 Process 33 | Waiting Time: 3 | Turnaround Time: 7

 Process 34 | Waiting Time: 5 | Turnaround Time: 10

 Process 35 | Waiting Time: 8 | Turnaround Time: 14

 Process 36 | Waiting Time: 12 | Turnaround Time: 19

 ✅All processes completed at time 51

 ➡️ Running queue 1:
____Round Robin scheduling algorithm___:

 ✅ All processes completed at time 0

 🕑 Execution time: 15 ms

## 📁 File Structure

KNN_algo/ 
├── main.cpp # Main execution file .
├── Scheduler.h # Process structure and scheduled algorithms .
├── knn_algo.h # KNN logic and Predected queue . 
├── training_data.csv # Labeled dataset for KNN training .
├── testing_data.csv # New data to classify and schedule  .
└── README.md # This file .

## ⚡️ Features
- Machine learning-driven decision making
- Parallelized KNN using OpenMP for faster prediction
- Custom scheduling logic with separate FIFO and RR handling
- Real-time simulation-like output with execution time tracking

---

## 🔧 How to Run


1. **Compile**

g++ -fopenmp main.cpp -o main  

2. **Run**

./main 

> ✅ Make sure you have your `training_data.csv` and `testing_data.csv` ready in the same directory.

---

## 📈 Future Improvements
- Add support for more scheduling algorithms.
- Visualize predictions and execution timelines .
- Dynamically tune `k` in KNN .
- Use a real dataset for training .

---

## 👨‍💻 Author
Built by an Embedded Systems Engineering student passionate about AI x OS integration.  
Feel free to contribute or get in touch for collaboration!

---

## 🧠 References
- [OpenMP Documentation](https://www.openmp.org/)
- [K-Nearest Neighbors - scikit-learn](https://scikit-learn.org/stable/modules/neighbors.html)