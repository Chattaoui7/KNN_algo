#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include "common.h"
#include <omp.h>
#include <algorithm>
  

using namespace std;
using namespace std::chrono;


double max_range = 100000;
int num_point = 500000;
int num_cluster = 20;
int max_iterations = 20;



int main() {
    
    Knn algo(max_range, num_point, num_cluster); ;
    
    srand(time(NULL));

    auto start = high_resolution_clock::now();

    printf("Starting KNN with OpenMP\n");
    printf("Number of points %d\n", num_point);
    printf("Number of clusters %d\n", num_cluster);


    printf("Starting initialization..\n");

    printf("Creating points..\n");
    vector<Point> points = algo.init_point(num_point);
    printf("Points initialized \n");

    printf("Creating clusters..\n");
    vector<Cluster> clusters = algo.init_cluster(num_cluster);
    printf("Clusters initialized \n");


    bool conv = true;
    int iterations = 0;

    printf("Starting iterate..\n");


    //The algorithm stops when iterations > max_iteration or when the clusters didn't move
    while(conv && iterations < max_iterations){

        iterations ++;

        algo.compute_distance(points, clusters);

        conv = algo.update_clusters(clusters);

        printf("Iteration %d done \n", iterations);

    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<minutes>(end - start); // Calculate duration in milliseconds

    printf("Total execution time: %ld minutes\n", duration.count());

    return 0;

}