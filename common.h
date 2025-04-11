
#include <queue>
#include <stdio.h>

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

