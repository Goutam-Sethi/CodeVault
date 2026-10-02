#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "status.h"

int status_repository(void) {
  DIR *dir = opendir(".");
  if (dir == NULL) {
    perror("Error opening current directory");
    return 1;
  }

  struct dirent *entry;

  printf("Untracked files:\n");

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (strcmp(entry->d_name, ".codevault") == 0) {
      continue;
    }

    printf(" %s\n", entry->d_name);
  }

  closedir(dir);

  return 0;
} 