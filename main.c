#include "point/point.h"
#include "k_means.h"
#include <stdio.h>

int main () {
    K_means* k_means = NULL;
    create_points(&k_means);
    proceed_algorithm(k_means);
    delete_points (&k_means);
    printf ("Uzbek2");
    return 0;
}