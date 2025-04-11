#include <queue>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <omp.h>
#include <algorithm>
#include <vector>
  
using namespace std;
// POINT

class Point {

public:
    Point(double x_coord, double y_coord){
        // ADD: Coords, cluster_id
        this->x_coord = x_coord;
        this->y_coord = y_coord;
        this->cluster_id =0;
    }

    Point(){
        // ADD: Coords, cluster_id initialised to 0
        this->x_coord = 0;
        this->y_coord = 0;
        this->cluster_id = 0;
    }

    double get_x_coord(){
        // returns x coord
        return x_coord;
    }

    double get_y_coord(){
       // retruns y coord
       return y_coord;
    }

    int get_cluster_id(){

        return cluster_id ;
    }

    void set_cluster_id(int cluster_id){
        // set cluster id
        this->cluster_id = cluster_id ;

    }

private:
    double x_coord;
    double y_coord;
    int cluster_id;
};


// CLUSTER

class Cluster {
public:
    Cluster(double x_coord, double y_coord){
        new_x_coord = 0;
        new_y_coord = 0;
        size = 0;
        this->x_coord = x_coord;
        this->y_coord = y_coord;
    }

    Cluster(){
        new_x_coord = 0;
        new_y_coord = 0;
        size = 0;
        this->x_coord = 0;
        this->y_coord = 0;
    }

    void add_point(Point point){

        new_x_coord += point.get_x_coord();
        new_y_coord += point.get_y_coord();
        size++;
    }

    void free_point(){
        this->size = 0;
        this->new_x_coord = 0;
        this->new_y_coord = 0;
    }

    double get_x_coord(){
        return x_coord;
    }

    double get_y_coord(){
        return y_coord;
    }

    bool update_coords(){


        if(this->x_coord == new_x_coord/this->size && this->y_coord == new_y_coord/this->size){
            return false;
        }

        this->x_coord = new_x_coord/this->size;
        this->y_coord = new_y_coord/this->size;

        return true;

    }

private:
    double x_coord;
    double y_coord;
    //Accumulate the Point coords here
    double new_x_coord;
    double new_y_coord;
    //Number of points inside this Cluster
    int size;

};


class Knn : public Point, public Cluster {

    int max_range ;
    int num_point ;
    int num_cluster ;

public:
    
    Knn(int max_range, int num_point, int num_cluster) : max_range(max_range), num_point(num_point), num_cluster(num_cluster) {}; 
    Knn() = default ;
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
};
