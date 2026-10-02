#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "init.h"

int init_repository(void) {
  if (mkdir(".codevault", 0755) == -1) {
    if (errno == EEXIST) {
      printf("CodeVault repository already exists.\n");
      return 1;
    }

    perror("Error creating .codevault");
    return 1;
  }

  mkdir(".codevault/objects", 0755);
  mkdir(".codevault/refs", 0755);
  mkdir(".codevault/refs/heads", 0755);

  FILE *index = fopen(".codevault/index", "w");
  if (index == NULL) {
    perror("Error creating index");
    return 1;
  }
  fclose(index);

  FILE *head = fopen(".codevault/HEAD", "w");
  if (head == NULL) {
    perror("Error creating HEAD");
    return 1;
  }
  fprintf(head, "ref: refs/heads/main\n");
  fclose(head);

  printf("Initialized empty CodeVault repository.\n");
  return 0;
}