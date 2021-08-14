#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
#define ALLOWED_THREADS_LENGTH 255

void *thread_function(void *arg) {
  char *data;
  data = (char *) arg;
  FILE *server_file;
  char file_path[] = "server.txt";  

  pthread_mutex_lock(&mtx);
  server_file = fopen(file_path, "a");

  if (server_file == NULL) {
    printf("Please make sure that the server file exists!");
    exit(1);
  }

  printf("File's contents to be saved : %s", data);
  fprintf(server_file, "%s", data);

  fclose(server_file);
  pthread_mutex_unlock(&mtx);
  pthread_exit(NULL);
}

int main() {
  int pipe;
  char *data[555];
  char file_path[] = "server.txt";
  FILE *server_file;
  pthread_t th[ALLOWED_THREADS_LENGTH];
  int i = 0;

  printf("**************** Server ****************\n");
  printf("Waiting for the client to send the content..\n");

  while (1) {
    mkfifo("/tmp/pipe1", S_IRWXU|S_IRGRP);

    pipe = open("/tmp/pipe1", O_RDONLY);
  
    if (server_file == NULL) {
      printf("Please make sure the server file exists!");
      exit(1);
    }

    while (read(pipe, data, sizeof(data)) > 0) {
      if (pthread_create(&th[i], NULL, thread_function, (void *) data) == -1) {
        printf("Error while trying to create a thread!\n");
        exit(1);
      }
      i++;
    }

    for (int j = 0; j < i; j++) {
      pthread_join(th[j], NULL);
    }

    pthread_mutex_destroy(&mtx);
    close(pipe);
    unlink("/tmp/pipe1");

    printf("The file's contents has been saved successfully!\n");
  }
  
  return 1;
}
