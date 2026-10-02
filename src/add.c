#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#include "add.h"
#include "hash.h"

int add_file(const char *filename) {
  struct stat file_info;

  if (stat(filename, &file_info) != 0) {
    perror("Error accessing file");
    return 1;
  }

  if (!S_ISREG(file_info.st_mode)) {
    printf("Error: '%s' is not a regular file.\n", filename);
    return 1;
  }

  FILE *file = fopen(filename, "rb");

  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  fclose(file);

  unsigned long long new_hash = calculate_hash(filename);

  FILE *index = fopen(".codevault/index", "r");

  if (index == NULL) {
    perror("Error opening index");
    return 1;
  }

  char temp_filename[256];
  unsigned long long old_hash;
  int found = 0;

  while (fscanf(index, "%255s %llu", temp_filename, &old_hash) == 2) {
    if (strcmp(temp_filename, filename) == 0) {
      found = 1;
      break;
    }
  }

  fclose(index);

  if (found && old_hash == new_hash) {
    printf("'%s' is already staged.\n", filename);
    return 0;
  }

  if (found) {
    FILE *input = fopen(".codevault/index", "r");
    FILE *output = fopen(".codevault/index.tmp", "w");

    if (input == NULL || output == NULL) {
      perror("Error updating index");

      if (input != NULL)
        fclose(input);

      if (output != NULL)
        fclose(output);

      return 1;
    }

    while (fscanf(input, "%255s %llu", temp_filename, &old_hash) == 2) {
      if (strcmp(temp_filename, filename) == 0) {
        fprintf(output, "%s %llu\n", filename, new_hash);
      }
      else {
        fprintf(output, "%s %llu\n", temp_filename, old_hash);
      }
    }

    fclose(input);
    fclose(output);

    if (remove(".codevault/index") != 0) {
      perror("Error replacing index");
      return 1;
    }

    if (rename(".codevault/index.tmp", ".codevault/index") != 0) {
      perror("Error renaming index");
      return 1;
    }

    printf("Updated '%s' in staging area.\n", filename);

    return 0;
  }

  index = fopen(".codevault/index", "a");

  if (index == NULL) {
    perror("Error opening index");
    return 1;
  }

  fprintf(index, "%s %llu\n", filename, new_hash);

  fclose(index);

  printf("Added '%s' to staging area.\n", filename);

  return 0;
}