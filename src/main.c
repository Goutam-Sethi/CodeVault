#include <stdio.h>
#include <string.h>

#include "init.h"
#include "status.h"
#include "hash.h"

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

  printf("Unknown command: %s\n", argv[1]);

  return 0;
}