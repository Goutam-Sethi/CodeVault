#include <stdio.h>
#include <string.h>

#include "restore.h"

int restore_repository(const char *filename) {
  FILE *branch = fopen(".codevault/refs/heads/main", "r");

  if (branch == NULL) {
    perror("Error opening main branch");
    return 1;
  }

  unsigned long long commit_hash;

  if (fscanf(branch, "%llu", &commit_hash) != 1) {
    printf("Error: No valid commit found.\n");
    fclose(branch);
    return 1;
  }

  fclose(branch);

  char commit_path[256];

  snprintf(commit_path, sizeof(commit_path), ".codevault/objects/%llu", commit_hash);

  FILE *commit_file = fopen(commit_path, "r");

  if (commit_file == NULL) {
    perror("Error opening latest commit");
    return 1;
  }

  char line[512];
  int reading_files = 0;
  int restored_count = 0;
  int file_found = 0;

  unsigned long long parent_hash = 0;
  int has_parent = 0;

  while (fgets(line, sizeof(line), commit_file) != NULL) {

    if (strncmp(line, "parent:", 7) == 0) {
      char parent_value[256];

      if (sscanf(line, "parent: %255s", parent_value) == 1) {
        if (strcmp(parent_value, "none") != 0) {
          if (sscanf(parent_value, "%llu", &parent_hash) == 1) {
            has_parent = 1;
          }
        }
      }

      continue;
    }

    if (strncmp(line, "files:", 6) == 0) {
      reading_files = 1;
      continue;
    }

    if (!reading_files) {
      continue;
    }

    char committed_filename[256];
    unsigned long long file_hash;

    if (sscanf(line, "%255s %llu", committed_filename, &file_hash) != 2) {
      continue;
    }

    if (filename != NULL && strcmp(committed_filename, filename) != 0) {
      continue;
    }

    file_found = 1;

    char object_path[256];

    snprintf(object_path, sizeof(object_path), ".codevault/objects/%llu", file_hash);

    FILE *object = fopen(object_path, "rb");

    if (object == NULL) {
      printf("Error: Object for '%s' not found.\n", committed_filename);
      fclose(commit_file);
      return 1;
    }

    FILE *output = fopen(committed_filename, "wb");

    if (output == NULL) {
      perror("Error restoring file");
      fclose(object);
      fclose(commit_file);
      return 1;
    }

    char buffer[4096];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), object)) > 0) {
      size_t bytes_written = fwrite(buffer, 1, bytes_read, output);

      if (bytes_written != bytes_read) {
        perror("Error writing restored file");
        fclose(object);
        fclose(output);
        fclose(commit_file);
        return 1;
      }
    }

    if (ferror(object)) {
      perror("Error reading file object");
      fclose(object);
      fclose(output);
      fclose(commit_file);
      return 1;
    }

    fclose(object);
    fclose(output);

    printf("Restored '%s'.\n", committed_filename);

    restored_count++;

    if (filename != NULL) {
      break;
    }
  }

  fclose(commit_file);

  if (filename != NULL && !file_found) {
    printf("Error: '%s' is not present in the latest commit.\n", filename);
    return 1;
  }

  if (restored_count == 0) {
    printf("No files found in the latest commit.\n");
    return 1;
  }

  if (filename == NULL) {

    if (has_parent) {
      branch = fopen(".codevault/refs/heads/main", "w");

      if (branch == NULL) {
        perror("Error updating main branch");
        return 1;
      }

      fprintf(branch, "%llu\n", parent_hash);

      fclose(branch);

      printf("Moved main to parent commit: %llu\n", parent_hash);
    }
    else {
      printf("Reached the first commit. No parent commit exists.\n");
    }
  }

  printf("Restore completed successfully.\n");

  return 0;
}