#include "point/point.h"
<<<<<<< Updated upstream
#include "k_means_multiprocess/k_means.h"
#include <stdio.h>

int main () {
    K_means* k_means = NULL;
    create_points(&k_means);
    proceed_algorithm(k_means);
    delete_points (&k_means);
    printf ("Uzbek\n");
=======
#include "k_means.h"
#include <stdio.h>
#include <time.h>

int main () {
    K_means* k_means = NULL;
    create_points(&k_means, 33000000, 33);
    time_t start = time(NULL);
    proceed_algorithm(k_means);
    time_t end = time(NULL);
    printf ("Прошло времени: %f секунд", difftime(end,start));
    delete_points (&k_means);
>>>>>>> Stashed changes
    return 0;
}