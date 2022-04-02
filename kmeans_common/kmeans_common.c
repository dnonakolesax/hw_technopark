#include "kmeans_common.h"

int find_cluster_center (K_means const* k_means, size_t cluster_number) {
    if (k_means==NULL) {
    return (-1);
  }
    size_t amount_of_points_in_cluster = 0;
    Point result = {0,0,0};
    for (size_t i=0; i<k_means->amount_of_points; i++) {
         if (k_means->points[i].cluster_number==cluster_number) {
            result.x+=k_means->points[i].point.x;
            result.y+=k_means->points[i].point.y;
            result.z+=k_means->points[i].point.z;  
            amount_of_points_in_cluster++;
        }
    }
    if (amount_of_points_in_cluster > 0) {
        k_means->clusters[cluster_number].x = result.x / amount_of_points_in_cluster;    
        k_means->clusters[cluster_number].y = result.y / amount_of_points_in_cluster;
        k_means->clusters[cluster_number].z = result.z / amount_of_points_in_cluster;
    }
   return 0;
}

int clusters_output (K_means const* k_means) {
     if (k_means==NULL) {
    return (-1);
  }
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
    printf("num: %zu, x: %f, y: %f\n", i, k_means->clusters[i].x, k_means->clusters[i].y);
        for (size_t j = 0; j < k_means->amount_of_points; ++j) {
            if (k_means->points[j].cluster_number == i) {
                printf("x: %f, y: %f\n", k_means->points[j].point.x, k_means->points[j].point.y);
      }
    }
  }
  return 0;
}