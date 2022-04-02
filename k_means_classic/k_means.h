#ifndef HW2_K_MEANS_H
#define HW2_K_MEANS_H

#include "../kmeans_common/kmeans_common.h"
#include <stdlib.h>
#include <stddef.h>
#include "../point/point.h"
#define ERRRATE 0.01

int create_points (K_means** k_means,size_t points,size_t clusters);
int proceed_algorithm (K_means* k_means);
int delete_points (K_means** k_means);
int sort_cluster(K_means* k_means, size_t batch_start, size_t batch_end);
#endif