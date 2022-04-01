#ifndef HW2_K_MEANS_H
#define HW2_K_MEANS_H

#include "../point/point.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct point_in_cluster {
    Point point;
    size_t cluster_number;
} point_in_cluster;

typedef struct K_means {
    point_in_cluster* points;
    size_t amount_of_points;
    Point* clusters; 
    size_t amount_of_clusters;
    size_t amount_of_changed_points;
} K_means;

int create_points (K_means** k_means,size_t points,size_t clusters);s
int find_cluster_center (K_means const* k_means, size_t cluster_number);
int proceed_algorithm (K_means* k_means);
int clusters_output (K_means const* k_means);
int sort_cluster(K_means* k_means, size_t batch_start, size_t batch_end);
int delete_points (K_means** k_means);
#endif