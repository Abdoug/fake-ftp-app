#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LINE_LENGTH 555

int main() {
  char line[LINE_LENGTH];
  char file_path[50];
  FILE *client_file;
  int pipe;
  int option = 2;
  char *full_content;
  int full_content_length = 1;

  printf("**************** Welcome to the client-server-app! ****************\n\n");
  printf("****************             Client                ****************\n");
  printf("Please choose one option (Make sure to have the server running): \n");
  printf("1 -> Send a file \n");
  printf("2 -> Exit \n");
  scanf("%d", &option);

  if (option != 1) {
    printf("See u next time!\n");
    exit(0);
  }

  printf("Please enter the path file : ");
  scanf("%s", &file_path);

  client_file = fopen(file_path, "r");

  if (client_file == NULL) {
    printf("The entered file couldn't be opened! \n");
    exit(1);
  }

  printf("Sending the file's lines..\n");
	
  full_content = (char *) malloc(full_content_length);  
  strcpy(full_content, "");

  while (fgets(line, sizeof line, client_file) != NULL) {
    full_content_length += sizeof(line);
    full_content = (char *) realloc(full_content, full_content_length);
    strcat(full_content, line);
  }

  mkfifo("/tmp/pipe1", S_IRWXU|S_IRGRP);

  pipe = open("/tmp/pipe1", O_WRONLY);
  
  write(pipe, full_content, strlen(full_content));

  close(pipe);
  fclose(client_file);

  printf("Done! The entered file has been sent successfully!\n");

  return 1;
}
