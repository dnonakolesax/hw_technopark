#include "k_means.h"

static int phase_num = 0;

struct msg_buf {
    long mtype;
    char mtext[MAX_SEND_SIZE];
};

void send_message(int qid, long type, char *text) {
    static struct msg_buf q_buf;
    q_buf.mtype = type;
    strcpy(q_buf.mtext, text);
    if (msgsnd(qid, &q_buf, strlen(q_buf.mtext) + 1, 0) == -1) {
        printf("Failed to send message\n");
    }
}

void read_message(int qid, char* text, long type) {
    static struct msg_buf q_buf;
    q_buf.mtype = type;
    if (msgrcv(qid, &q_buf, MAX_SEND_SIZE, type, 0) < 0) {
        printf("Failed to read message from queue\n");
    } else {
       printf("Received message %s\n", q_buf.mtext);
    }
}

static void Handler(int sig_num) {
  switch (sig_num) {
    case SIGUSR1:
      phase_num = 1;
      break;
    case SIGUSR2:
      phase_num = 2;
      break;
    default:
      signal(sig_num, SIG_DFL);
      break;
  }
}

int create_points (K_means** k_means,size_t points,size_t clusters) {
    K_means* temp = (K_means*)mmap(NULL, sizeof(K_means), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    temp->amount_of_clusters=clusters;
    temp->amount_of_points=points; 
    if (temp->amount_of_clusters > temp->amount_of_points) {
        munmap(temp, sizeof(K_means));
        return -1;
    }
    temp->amount_of_changed_points = temp->amount_of_points;
    temp->points=(point_in_cluster*)mmap( NULL, temp->amount_of_points * sizeof(point_in_cluster), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    temp->clusters=(Point*)mmap(NULL, temp->amount_of_clusters * sizeof(Point), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
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

int delete_points (K_means** k_means) {
  if (k_means==NULL) {
    return (-1);
  } 
    K_means* temp = *k_means;
    if (temp->points != NULL) {
        munmap(temp->points, temp->amount_of_points*sizeof(point_in_cluster));
        temp->points = NULL;
    }
    if (temp->clusters != NULL) {
         munmap(temp->clusters, temp->amount_of_clusters*sizeof(Point));
        temp->clusters = NULL;
    }
    munmap(temp, sizeof(k_means));
    temp = NULL;
    *k_means = temp;
   return 0;
}

void start_child_work(int msgid, K_means* kmeans) {
  char send_tmp[MAX_SEND_SIZE] = {0};
  char recv_tmp[MAX_SEND_SIZE] = {0};
  signal(SIGUSR1, Handler);
  signal(SIGUSR2, Handler);
  raise(SIGSTOP);
  while (1) {
    switch (phase_num) {
      case 1:
        read_message(msgid, recv_tmp, SORT_MSG);
        size_t start_batch = 0;
        size_t end_batch = 0;
        sscanf(recv_tmp, "%zu %zu", &start_batch, &end_batch);
        size_t changed = 0;
        if (sort_cluster(kmeans, start_batch, end_batch, &changed)) {
          printf("ClusterSort error\n");
        }
        snprintf(send_tmp, MAX_SEND_SIZE, "%zu", changed);
        send_message(msgid, TO_PARENT_MSG, send_tmp);
        phase_num = 0;
        break;
      case 2:
        read_message(msgid, recv_tmp, CENTER_MSG);
        size_t cnt = 0;
        sscanf(recv_tmp, "%zu", &cnt);
        if (find_cluster_center(kmeans, cnt)) {
          printf("FindClusterCenter error\n");
        }
        send_message(msgid, TO_PARENT_MSG, "ok");
        phase_num = 0;
        break;
      default:
        break;
    }
  }
}

int proceed_algorithm (K_means* k_means) {
  if (k_means==NULL) {
    return (-1);
  } 
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
    k_means->clusters[i] = k_means->points[i].point;
  }
  size_t amount_of_processes = sysconf(_SC_NPROCESSORS_ONLN);
  int pids[amount_of_processes];
  int msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
  for (size_t i = 0; i < amount_of_processes; ++i) {
    pids[i] = fork();
    if (pids[i] == -1) {
      printf("fork failed\n");
      for (size_t j=0; j<i; j++) {
        kill(pids[i], SIGUSR1);
      }
      return -1;
    } else if (pids[i] == 0) {
      start_child_work(msgid, k_means);
    } else {
      exit (0);
    }
  }
  char send_tmp[MAX_SEND_SIZE] = {0};
  char recv_tmp[MAX_SEND_SIZE] = {0};
  for (size_t i = 0; i < amount_of_processes; ++i) {
    int status = 0;
    waitpid(-1, &status, WUNTRACED);
  }
  kill(0, SIGCONT);
  size_t changed = 0;
  do {
    for (size_t i = 0; i < amount_of_processes; ++i) {
      kill(pids[i], SIGUSR1);
    }
    size_t start_batch = 0;
    size_t end_batch = 0;
    for (size_t i = 0; i < amount_of_processes; ++i) {
      start_batch = end_batch;
      end_batch = start_batch + (k_means->amount_of_points - start_batch) / (amount_of_processes - i);
      send_message(msgid, SORT_MSG, send_tmp);
    }
    size_t changed_tmp = 0;
    changed = 0;
    for (size_t i = 0; i < amount_of_processes; ++i) {
      read_message(msgid, recv_tmp, TO_PARENT_MSG);
      sscanf(recv_tmp, "%zu", &changed_tmp);
      changed += changed_tmp;
    }
    for (size_t i = 0; i < amount_of_processes; ++i) {
      kill(pids[i], SIGUSR2);
    }
    for (size_t i = 0; i < k_means->amount_of_clusters; ++i) {
      snprintf(send_tmp, MAX_SEND_SIZE, "%zu", i);
      send_message(msgid, CENTER_MSG, send_tmp);
    }
    for (size_t i = 0; i < amount_of_processes; ++i) {
      read_message(msgid, recv_tmp, TO_PARENT_MSG);
    }
  } while (((double)changed / (double)k_means->amount_of_points) > ERRRATE);
  for (size_t i = 0; i < amount_of_processes; ++i) {
    kill(pids[i], SIGKILL);
  }
  msgctl(msgid, IPC_RMID, NULL);
  return 0;
}

int sort_cluster(K_means* k_means, size_t batch_start, size_t batch_end, size_t* changed) {
  if (k_means==NULL) {
    return (-1);
  } 
  k_means->amount_of_changed_points = 0;
  *changed = 0;
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
      ++(*changed);
      k_means->points[i].cluster_number = near_cluster;
    }
  }
  return 0;
}
