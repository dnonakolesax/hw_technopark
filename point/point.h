#ifndef HW2_POINT_H
#define HW2_POINT_H
#define SUCCESS 0

typedef struct Point {
    double x;
    double y;
    double z;
} Point;

int square_of_euclidian_distance (Point const* first_point, Point const* second_point,  double* result);

#endif