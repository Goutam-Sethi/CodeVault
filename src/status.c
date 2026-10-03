#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "status.h"
#include "hash.h"

void scan_directory(const char *directory, char tracked_files[][256], unsigned long long tracked_hashes[], int tracked_count, char committed_files[][256], unsigned long long committed_hashes[], int committed_count, int depth) {
  DIR *dir = opendir(directory);

  if (dir == NULL) {
    perror("Error opening directory");
    return;
  }

  struct dirent *entry;

  if (strcmp(directory, ".") != 0) {
    printf("\n");

    for (int i = 0; i < depth; i++) {
      printf("  ");
    }

    const char *directory_name = strrchr(directory, '/');

    if (directory_name != NULL) {
      printf("%s/\n", directory_name + 1);
    }
    else {
      printf("%s/\n", directory);
    }
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char path[1024];

    if (strcmp(directory, ".") == 0) {
      snprintf(path, sizeof(path), "%s", entry->d_name);
    }
    else {
      snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
    }

    struct stat file_info;

    if (stat(path, &file_info) != 0) {
      continue;
    }

    if (S_ISDIR(file_info.st_mode)) {
      scan_directory(path, tracked_files, tracked_hashes, tracked_count, committed_files, committed_hashes, committed_count, depth + 1);
      continue;
    }

    if (!S_ISREG(file_info.st_mode)) {
      continue;
    }

    int staged = 0;
    int committed = 0;
    unsigned long long committed_hash = 0;

    for (int i = 0; i < tracked_count; i++) {
      if (strcmp(tracked_files[i], path) == 0) {
        staged = 1;
        break;
      }
    }

    for (int i = 0; i < committed_count; i++) {
      if (strcmp(committed_files[i], path) == 0) {
        committed = 1;
        committed_hash = committed_hashes[i];
        break;
      }
    }

    unsigned long long current_hash = calculate_hash(path);

    for (int j = 0; j < depth + 1; j++) {
      printf("  ");
    }

    if (staged) {
      printf("%-25s [staged]\n", entry->d_name);
    }
    else if (committed) {
      if (current_hash == committed_hash) {
        printf("%-25s [committed]\n", entry->d_name);
      }
      else {
        printf("%-25s [modified]\n", entry->d_name);
      }
    }
    else {
      printf("%-25s [untracked]\n", entry->d_name);
    }
  }

  closedir(dir);
}

int status_repository(void) {
  FILE *index = fopen(".codevault/index", "r");

  if (index == NULL) {
    perror("Error opening index");
    return 1;
  }

  char tracked_files[100][256];
  unsigned long long tracked_hashes[100];
  int tracked_count = 0;

  while (tracked_count < 100 && fscanf(index, "%255s %llu", tracked_files[tracked_count], &tracked_hashes[tracked_count]) == 2) {
    tracked_count++;
  }

  fclose(index);

  char committed_files[100][256];
  unsigned long long committed_hashes[100];
  int committed_count = 0;

  FILE *branch = fopen(".codevault/refs/heads/main", "r");

  if (branch != NULL) {
    unsigned long long commit_hash;

    if (fscanf(branch, "%llu", &commit_hash) == 1) {
      fclose(branch);

      char commit_path[256];

      snprintf(commit_path, sizeof(commit_path), ".codevault/objects/%llu", commit_hash);

      FILE *commit_file = fopen(commit_path, "r");

      if (commit_file != NULL) {
        char line[512];
        int reading_files = 0;

        while (fgets(line, sizeof(line), commit_file) != NULL) {
          if (strncmp(line, "files:", 6) == 0) {
            reading_files = 1;
            continue;
          }

          if (reading_files && committed_count < 100) {
            if (sscanf(line, "%255s %llu", committed_files[committed_count], &committed_hashes[committed_count]) == 2) {
              committed_count++;
            }
          }
        }

        fclose(commit_file);
      }
    }
    else {
      fclose(branch);
    }
  }

  printf("CodeVault status\n");
  printf("================\n\n");

  scan_directory(".", tracked_files, tracked_hashes, tracked_count, committed_files, committed_hashes, committed_count, 0);

  return 0;
}