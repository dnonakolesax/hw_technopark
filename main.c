#include "point/point.h"
#include "k_means.h"
#include "kmeans_common/kmeans_common.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#define EXPERIMENTSAMOUNT 8

int main () {
    K_means* k_means = NULL;
    double deviation=0;
    double rms=0;
    time_t differs[EXPERIMENTSAMOUNT];
    int i=0;
    for (int i=0; i<EXPERIMENTSAMOUNT; i++) {
        create_points(&k_means, 3300000, 33);
        time_t start = time(NULL);
        proceed_algorithm(k_means);
        time_t end = time(NULL);
        differs[i]=difftime(end,start);
        delete_points (&k_means);
    }
    for (int i=0; i<EXPERIMENTSAMOUNT; i++) {
        printf ("%ld\n", differs[i]);
        rms+= pow (differs[i],2);
    }
    rms = sqrt (rms/EXPERIMENTSAMOUNT);
    for (int i=0; i<EXPERIMENTSAMOUNT; i++) {
        deviation+= pow ((differs[i]-rms),2);
    }
    deviation = sqrt (deviation/EXPERIMENTSAMOUNT);
    printf ("Approximate working time of algorithm in seconds: %lf +- %lf", rms, deviation);
    return 0;
}