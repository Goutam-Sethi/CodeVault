#include <stdio.h>

#include "commit.h"
#include "hash.h"
#include "object.h"

int commit_repository(const char *message) {
  FILE *index = fopen(".codevault/index", "r");

  if (index == NULL) {
    perror("Error opening index");
    return 1;
  }

  char filename[256];
  unsigned long long file_hash;
  int file_count = 0;

  while (fscanf(index, "%255s %llu", filename, &file_hash) == 2) {
    file_count++;
  }

  if (file_count == 0) {
    fclose(index);
    printf("Nothing to commit.\n");
    return 1;
  }

  fclose(index);

  index = fopen(".codevault/index", "r");

  if (index == NULL) {
    perror("Error opening index");
    return 1;
  }

  FILE *commit_file = fopen(".codevault/commit.tmp", "w");

  if (commit_file == NULL) {
    perror("Error creating commit");
    fclose(index);
    return 1;
  }

  fprintf(commit_file, "message: %s\n", message);

  FILE *branch = fopen(".codevault/refs/heads/main", "r");

  if (branch != NULL) {
    unsigned long long parent_hash;

    if (fscanf(branch, "%llu", &parent_hash) == 1) {
      fprintf(commit_file, "parent: %llu\n", parent_hash);
    }
    else {
      fprintf(commit_file, "parent: none\n");
    }

    fclose(branch);
  }
  else {
    fprintf(commit_file, "parent: none\n");
  }

  fprintf(commit_file, "\nfiles:\n");

  while (fscanf(index, "%255s %llu", filename, &file_hash) == 2) {
    if (store_object(filename, file_hash) != 0) {
      fclose(index);
      fclose(commit_file);
      remove(".codevault/commit.tmp");
      return 1;
    }

    fprintf(commit_file, "%s %llu\n", filename, file_hash);
  }

  fclose(index);
  fclose(commit_file);

  unsigned long long commit_hash = calculate_hash(".codevault/commit.tmp");

  if (store_object(".codevault/commit.tmp", commit_hash) != 0) {
    remove(".codevault/commit.tmp");
    return 1;
  }

  remove(".codevault/commit.tmp");

  branch = fopen(".codevault/refs/heads/main", "w");

  if (branch == NULL) {
    perror("Error updating main branch");
    return 1;
  }

  fprintf(branch, "%llu\n", commit_hash);

  fclose(branch);

  FILE *clear_index = fopen(".codevault/index", "w");
  if (clear_index == NULL) {
    perror("Error clearing index");
    return 1;
  }
  fclose(clear_index);

  printf("Committed successfully.\n");
  printf("Commit: %llu\n", commit_hash);
  printf("Message: %s\n", message);

  return 0;
}