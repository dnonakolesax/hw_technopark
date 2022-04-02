#include "k_means.h"

int create_points (K_means** k_means, size_t points, size_t clusters) {
    K_means* temp = (K_means*)malloc(sizeof(K_means));
    if (temp==NULL) {
      return (-1);
    }
    temp->amount_of_clusters=clusters;
    temp->amount_of_points=points;
    if (temp->amount_of_clusters>temp->amount_of_points) {
        free (temp);
        return (-1);
    }
    temp->amount_of_changed_points = temp->amount_of_points;
    temp->points=(point_in_cluster*)malloc(temp->amount_of_points*sizeof(point_in_cluster));
    if (temp->points==NULL) {
      return (-1);
    }
    temp->clusters=(Point*)malloc(temp->amount_of_clusters*sizeof(Point));
    if (temp->clusters==NULL) {
      return (-1);
    }
    for (size_t i=0; i<temp->amount_of_points;i++) {
        temp->points[i].point.x=rand()%50;
        temp->points[i].point.y=rand()%50;
        temp->points[i].point.z=rand()%50;
        temp->points[i].cluster_number=0;
    }
    *k_means = temp;
    return 0;
}

int proceed_algorithm (K_means* k_means) {
  if (k_means==NULL) {
    return (-1);
  } 
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
    if (sort_cluster(k_means, 0, k_means->amount_of_points)) {
      return (-1);
    }
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
      if (find_cluster_center(k_means, i)) {
        return 1;
      }
    }
  }
  return 0;
}


int delete_points (K_means** k_means) {
   if (k_means==NULL) {
    return (-1);
  } 
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
  *k_means = temp;
   return 0;
}

int sort_cluster(K_means* k_means, size_t batch_start, size_t batch_end) {
   if (k_means==NULL) {
    return (-1);
  } 
  if (batch_start>batch_end) {
    return (-1);
  }
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