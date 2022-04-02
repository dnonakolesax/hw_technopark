#ifndef HW2_K_MEANS_H
#define HW2_K_MEANS_H

#include "../point/point.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include "../kmeans_common/kmeans_common.h"

#define MAX_SEND_SIZE 80
#define ERRRATE 0.01


#define SORT_MSG 100
#define CENTER_MSG 200
#define TO_PARENT_MSG 300

int create_points (K_means** k_means,size_t points,size_t clusters);
int proceed_algorithm (K_means* k_means);
void start_child_work(int msgid, K_means* kmeans);
int delete_points (K_means** k_means);
int sort_cluster(K_means* k_means, size_t batch_start, size_t batch_end, size_t* changed);
#endif