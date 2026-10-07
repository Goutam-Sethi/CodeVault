#include <stdio.h>
#include <string.h>

#include "init.h"
#include "status.h"
#include "add.h"
#include "commit.h"
#include "restore.h"
#include "diff.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: codevault <command>\n");
    return 1;
  }

  if (strcmp(argv[1], "init") == 0) {
    return init_repository();
  }

  if (strcmp(argv[1], "status") == 0) {
    return status_repository();
  }

  if (strcmp(argv[1], "add") == 0) {
    if (argc < 3) {
      printf("Usage: codevault add <file>\n");
      return 1;
    }

    return add_file(argv[2]);
  }

  if (strcmp(argv[1], "commit") == 0) {
    if (argc < 3) {
      printf("Usage: codevault commit <message>\n");
      return 1;
    }

    return commit_repository(argv[2]);
  }

  if (strcmp(argv[1], "restore") == 0) {
    if (argc >= 3) {
      return restore_repository(argv[2]);
    }

    return restore_repository(NULL);
  }

  if (strcmp(argv[1], "diff") == 0) {
    if (argc >= 3) {
      return diff_repository(argv[2]);
    }

    return diff_repository(NULL);
  }

  printf("Unknown command: %s\n", argv[1]);

  return 0;
}