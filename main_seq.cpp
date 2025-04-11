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

vector<Point> init_point(int num_point);
vector<Cluster> init_cluster(int num_cluster);
void compute_distance(vector<Point> &points, vector<Cluster> &clusters);
double euclidean_dist(Point point, Cluster cluster);
bool update_clusters(vector<Cluster> &clusters);


int main() {

    srand(time(NULL));

    auto start = high_resolution_clock::now();

    printf("Starting KNN with OpenMP\n");
    printf("Number of points %d\n", num_point);
    printf("Number of clusters %d\n", num_cluster);


    printf("Starting initialization..\n");

    printf("Creating points..\n");
    vector<Point> points = init_point(num_point);
    printf("Points initialized \n");

    printf("Creating clusters..\n");
    vector<Cluster> clusters = init_cluster(num_cluster);
    printf("Clusters initialized \n");


    bool conv = true;
    int iterations = 0;

    printf("Starting iterate..\n");


    //The algorithm stops when iterations > max_iteration or when the clusters didn't move
    while(conv && iterations < max_iterations){

        iterations ++;

        compute_distance(points, clusters);

        conv = update_clusters(clusters);

        printf("Iteration %d done \n", iterations);

    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<minutes>(end - start); // Calculate duration in milliseconds

    printf("Total execution time: %lld minutes\n", duration.count());

    return 0;

}

//Initialize num_point Points
vector<Point> init_point(int num_point){

    vector<Point> points(num_point);
    Point *ptr = &points[0];

    for(int i = 0; i < num_point; i++){

        Point* point = new Point(rand() % (int)max_range, rand() % (int)max_range);

        ptr[i] = *point;

    }

    return points;

}

//Initialize num_cluster Clusters
vector<Cluster> init_cluster(int num_cluster){

    vector<Cluster> clusters(num_cluster);
    Cluster* ptr = &clusters[0];
    // TODO: init clusters vector

    for(int i = 0; i < num_cluster; i++){

        Cluster* cluster = new Cluster(rand() % (int)max_range, rand() % (int)max_range);

        ptr[i] = *cluster;

    }
    return clusters;
}

//For each Point, compute the distance between each Cluster and assign the Point to the nearest Cluster
//The distance is compute through Euclidean Distance

void compute_distance(vector<Point> &points, vector<Cluster> &clusters) {
    unsigned long points_size = points.size();
    unsigned long clusters_size = clusters.size();
    vector<vector<Point>> cluster_assignment(num_cluster); // global storage cluster assignment

    #pragma omp parallel 
    {
        vector<vector<Point>> temporary_assignment(num_cluster); // local storage cluster assignment

        #pragma omp nowait
        {
            for (unsigned long i = 0; i < points_size; i++) {
                vector<pair<double, int>> distances; // Vector to store distances and indices

                for (unsigned long j = 0; j < clusters_size; j++) {
                    double distance = euclidean_dist(points[i], clusters[j]);
                    distances.push_back(make_pair(distance, j)); // Store distance and index
                }

                // Sort distances based on the first element of the pair (the distance)
                sort(distances.begin(), distances.end());

                // The nearest cluster is now the first element in the sorted distances
                int min_index = distances[0].second; // Get the index of the nearest cluster
                points[i].set_cluster_id(min_index);
                temporary_assignment[min_index].push_back(points[i]);
            }
        }

        #pragma omp critical // Ensure that only one thread accesses the cluster_assignment vector at a time
            // Merge temporary_assignment into cluster_assignment
            {
                for (unsigned long i = 0; i < clusters_size; i++) {
                    cluster_assignment[i].insert(cluster_assignment[i].end(), temporary_assignment[i].begin(), temporary_assignment[i].end());
                }
            }
    } // End of parallel region
            
    for(unsigned long i = 0; i < clusters_size; i++) { //add points to clusters
        for (unsigned long j = 0; j < cluster_assignment[i].size(); j++) {
            clusters[i].add_point(cluster_assignment[i][j]);
        }
    }
}

double euclidean_dist(Point point, Cluster cluster){

    // TODO:  Compute euclidean distance

    double x  = cluster.get_x_coord() - point.get_x_coord();
    double y  = cluster.get_y_coord() - point.get_y_coord();
    double distance = std::sqrt(std::pow(x,2) + std::pow(y,2)) ;
    return distance;
}

//For each cluster, update the coords. If only one cluster moves, conv will be TRUE
bool update_clusters(vector<Cluster> &clusters){

    bool conv = false;
    #pragma omp parallel reduction(|:conv)

    for(int i = 0; i < clusters.size(); i++){

        conv = clusters[i].update_coords();
        clusters[i].free_point();
    }

    return conv;
}

