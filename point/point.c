#include "point.h"
#include <stdlib.h>

int square_of_euclidian_distance (Point const* first_point, Point const* second_point, double* result) {
    if ((first_point==NULL) || (second_point==NULL) || (result==NULL)) {
        return (-1);
    }
    *result=(first_point->x-second_point->x)*(first_point->x-second_point->x)+
        (first_point->y-second_point->y)*(first_point->y-second_point->y)+
        (first_point->z-second_point->z)*(first_point->z-second_point->z);  
    if (result==NULL) {
        return -1;
    }
    return 0;
};