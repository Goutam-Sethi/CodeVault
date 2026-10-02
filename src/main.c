#include <stdio.h>
#include <string.h>

#include "init.h"
#include "status.h"
#include "add.h"
#include "object.h"

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

  if (strcmp(argv[1], "object") == 0) {
    if (argc < 4) {
      printf("Usage: codevault object <file> <hash>\n");
      return 1;
    }

    unsigned long long hash = strtoull(argv[3], NULL, 10);

    return store_object(argv[2], hash);
  }

  printf("Unknown command: %s\n", argv[1]);

  return 0;
}