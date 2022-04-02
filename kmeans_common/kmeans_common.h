#ifndef HW2_K_MEANS_COMMON_H
#define HW2_K_MEANS_COMMON_H

#include <stdlib.h>
#include "../point/point.h"
#include <stdio.h>

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

int find_cluster_center (K_means const* k_means, size_t cluster_number);
int clusters_output (K_means const* k_means);

#endif