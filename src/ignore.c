#include <stdio.h>
#include <string.h>
#include <fnmatch.h>

#include "ignore.h"

int is_ignored(const char *path) {
  if (strncmp(path, ".codevault", 10) == 0) {
    return 1;
  }

  FILE *file = fopen(".codevaultignore", "r");

  if (file == NULL) {
    return 0;
  }

  char pattern[256];

  while (fgets(pattern, sizeof(pattern), file) != NULL) {
    pattern[strcspn(pattern, "\n")] = '\0';

    if (pattern[0] == '\0') {
      continue;
    }

    if (pattern[0] == '#') {
      continue;
    }

    int length = strlen(pattern);

    if (length > 0 && pattern[length - 1] == '/') {
      pattern[length - 1] = '\0';

      if (strstr(path, pattern) != NULL) {
        fclose(file);
        return 1;
      }
    }
    else {
      if (fnmatch(pattern, path, 0) == 0) {
        fclose(file);
        return 1;
      }
    }
  }

  fclose(file);

  return 0;
}