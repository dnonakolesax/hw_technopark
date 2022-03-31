#include "k_means.h"
#include "../point/point.h"
#include "stdio.h"
#define ERRRATE 0.01

int find_cluster_center (K_means const* k_means, size_t cluster_number) {
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
   // printf ("Success found center!\n");
   return 0;
}

int create_points (K_means** k_means) {
    K_means* temp = (K_means*)malloc(sizeof(K_means));
    temp->amount_of_clusters=33;
    temp->amount_of_points=33000000;
    if (temp->amount_of_clusters>temp->amount_of_points) {
        free (temp);
        return (-1);
    }
    temp->amount_of_changed_points = temp->amount_of_points;
    temp->points=(point_in_cluster*)malloc(temp->amount_of_points*sizeof(point_in_cluster));
    temp->clusters=(Point*)malloc(temp->amount_of_clusters*sizeof(Point));
    for (size_t i=0; i<temp->amount_of_points;i++) {
        temp->points[i].point.x=rand()%50;
        temp->points[i].point.y=rand()%50;
        temp->points[i].point.z=rand()%50;
        temp->points[i].cluster_number=0;
    }
    *k_means = temp;
    printf ("Success create!\n");
    return 0;
}

int proceed_algorithm (K_means* k_means) {
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
    k_means->clusters[i] = k_means->points[i].point;
  }
    for (size_t i = 0, k = 0; i < k_means->amount_of_clusters; ++i) {
        for (size_t j = i + 1; j < k_means->amount_of_clusters; j++) {
        if ((k_means->clusters[j].x == k_means->clusters[i].x) &&
           (k_means->clusters[j].y == k_means->clusters[i].y) &&
           (k_means->clusters[j].z == k_means->clusters[i].z)) {
        k_means->clusters[i] = k_means->points[k_means->amount_of_clusters + k++].point;
        i = 0;
        break;
      }
    }
  }

  while (((double)k_means->amount_of_changed_points / (double)k_means->amount_of_points )>ERRRATE) {
    sort_cluster(k_means, 0, k_means->amount_of_points) ;
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
      if (find_cluster_center(k_means, i)) {
        return 1;
      }
    }
  }
  return 0;
}


int delete_points (K_means** k_means) {
    K_means* temp = *k_means;
    if (temp->points != NULL) {
    free(temp->points);
    temp->points = NULL;
  }
  if (temp->clusters != NULL) {
    free(temp->clusters);
    temp->clusters = NULL;
  }
  free(temp);
  temp = NULL;
  *k_means = temp;
   return 0;
}

int clusters_output (K_means const* k_means) {
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
   // printf("num: %zu, x: %f, y: %f\n", i, k_means->clusters[i].x, k_means->clusters[i].y);
        for (size_t j = 0; j < k_means->amount_of_points; ++j) {
            if (k_means->points[j].cluster_number == i) {
               // printf("x: %f, y: %f\n", k_means->points[j].point.x, k_means->points[j].point.y);
      }
    }
  }
  return 0;
}

int sort_cluster(K_means* k_means, size_t batch_start, size_t batch_end) {
  k_means->amount_of_changed_points = 0;
  for (size_t i = batch_start; i < batch_end; ++i) {
    double dist_min = 0;
    size_t near_cluster = 0;
    double tmp = 0;
    square_of_euclidian_distance(&k_means->points[i].point, &k_means->clusters[0], &tmp);
    dist_min = tmp;
    for (size_t j = 1; j < k_means->amount_of_clusters; ++j) {
      if (square_of_euclidian_distance(&k_means->points[i].point, &k_means->clusters[j], &tmp)) {
        return -1;
      }
      if (tmp < dist_min) {
        dist_min = tmp;
        near_cluster = j;
      }
    }
    if (k_means->points[i].cluster_number != near_cluster) {
      ++k_means->amount_of_changed_points;
      k_means->points[i].cluster_number = near_cluster;
    }
  }
  return 0;
}