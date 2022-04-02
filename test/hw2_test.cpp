#include <gtest/gtest.h>

extern "C" {
    #include "k_means.h"
    #include "../point/point.h"
    #include "../kmeans_common/kmeans_common.h"
}

TEST (normal_test, normal_test) {
  K_means* k_means = NULL;
  EXPECT_EQ(create_points(&k_means, 12, 3), 0);
  EXPECT_EQ(proceed_algorithm(k_means), 0);
  Point cluster_heads [3];
  cluster_heads[0] = k_means->clusters[0];
  cluster_heads[1] = k_means->clusters[1];
  cluster_heads[2] = k_means->clusters[2];
  for (int i=0; i<k_means->amount_of_points; i++) {
      size_t point_cluster = k_means->points[i].cluster_number;
      double distToFirst=0, distToSecond=0, distToThird=0;
      square_of_euclidian_distance (&k_means->points[i].point, &cluster_heads[0], &distToFirst);
      square_of_euclidian_distance (&k_means->points[i].point, &cluster_heads[1], &distToSecond);
      square_of_euclidian_distance (&k_means->points[i].point, &cluster_heads[2], &distToThird);
       if (distToFirst < distToSecond && distToFirst < distToThird) {
            EXPECT_EQ (point_cluster, 0);
        } else if (distToSecond < distToFirst && distToSecond < distToThird) {
            EXPECT_EQ (point_cluster, 1);
        } else {
            EXPECT_EQ (point_cluster, 2);
        }
  }
  EXPECT_EQ(clusters_output(k_means), 0);
  EXPECT_EQ (delete_points(&k_means), 0);
}

TEST (crash_test, crash_test) {
  K_means* k_means = NULL;
  double result=0;
  EXPECT_EQ(create_points(&k_means, 12, 30), -1);
  EXPECT_EQ(proceed_algorithm(NULL), -1);
  EXPECT_EQ (delete_points(NULL), -1);
  EXPECT_EQ (find_cluster_center(NULL, 1), -1);
  EXPECT_EQ (clusters_output(NULL), -1);
}

TEST (point, point) {
  const Point p1 = {5,10,5};
  const Point p2= {1,6,3};
  double result=0;
  EXPECT_EQ (square_of_euclidian_distance (&p1, NULL, &result), -1);
  EXPECT_EQ (square_of_euclidian_distance (NULL, &p2, &result), -1);
  EXPECT_EQ (square_of_euclidian_distance (&p1, &p2, NULL), -1);
  square_of_euclidian_distance (&p1, &p2, &result);
  EXPECT_EQ (result, 36);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}