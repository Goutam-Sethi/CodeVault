#include <stdio.h>
#include <string.h>

#include "diff.h"

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 512

int read_file_lines(const char *filename, char lines[][MAX_LINE_LENGTH]) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    return -1;
  }

  int count = 0;

  while (count < MAX_LINES &&
         fgets(lines[count], MAX_LINE_LENGTH, file) != NULL) {
    lines[count][strcspn(lines[count], "\n")] = '\0';
    count++;
  }

  fclose(file);

  return count;
}

void print_diff(
  const char committed[][MAX_LINE_LENGTH],
  int committed_count,
  const char current[][MAX_LINE_LENGTH],
  int current_count,
  const char *filename
) {
  int dp[MAX_LINES + 1][MAX_LINES + 1];

  for (int i = 0; i <= committed_count; i++) {
    dp[i][0] = 0;
  }

  for (int j = 0; j <= current_count; j++) {
    dp[0][j] = 0;
  }

  for (int i = 1; i <= committed_count; i++) {
    for (int j = 1; j <= current_count; j++) {
      if (strcmp(committed[i - 1], current[j - 1]) == 0) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      }
      else if (dp[i - 1][j] >= dp[i][j - 1]) {
        dp[i][j] = dp[i - 1][j];
      }
      else {
        dp[i][j] = dp[i][j - 1];
      }
    }
  }

  printf("--- %s (committed)\n", filename);
  printf("+++ %s (working tree)\n", filename);

  int i = committed_count;
  int j = current_count;

  char operations[2000];
  int operation_count = 0;

  while (i > 0 || j > 0) {
    if (i > 0 && j > 0 &&
        strcmp(committed[i - 1], current[j - 1]) == 0) {
      operations[operation_count++] = '=';
      i--;
      j--;
    }
    else if (j > 0 &&
             (i == 0 || dp[i][j - 1] > dp[i - 1][j])) {
      operations[operation_count++] = '+';
      j--;
    }
    else {
      operations[operation_count++] = '-';
      i--;
    }
  }

  i = 0;
  j = 0;

  for (int k = operation_count - 1; k >= 0; k--) {
    if (operations[k] == '=') {
      i++;
      j++;
    }
    else if (operations[k] == '-') {
      printf("- %s\n", committed[i]);
      i++;
    }
    else {
      printf("+ %s\n", current[j]);
      j++;
    }
  }
}

int diff_file(const char *filename, unsigned long long file_hash) {
  char object_path[256];

  snprintf(
    object_path,
    sizeof(object_path),
    ".codevault/objects/%llu",
    file_hash
  );

  char committed_lines[MAX_LINES][MAX_LINE_LENGTH];
  char current_lines[MAX_LINES][MAX_LINE_LENGTH];

  int committed_count = read_file_lines(
    object_path,
    committed_lines
  );

  if (committed_count < 0) {
    printf("Error: Committed object for '%s' not found.\n", filename);
    return 1;
  }

  int current_count = read_file_lines(
    filename,
    current_lines
  );

  if (current_count < 0) {
    printf("Error: Working file '%s' not found.\n", filename);
    return 1;
  }

  int different = 0;

  if (committed_count != current_count) {
    different = 1;
  }
  else {
    for (int i = 0; i < committed_count; i++) {
      if (strcmp(committed_lines[i], current_lines[i]) != 0) {
        different = 1;
        break;
      }
    }
  }

  if (!different) {
    printf("%s: no changes\n", filename);
    return 0;
  }

  print_diff(
    committed_lines,
    committed_count,
    current_lines,
    current_count,
    filename
  );

  return 0;
}

int diff_repository(const char *filename) {
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

  snprintf(
    commit_path,
    sizeof(commit_path),
    ".codevault/objects/%llu",
    commit_hash
  );

  FILE *commit_file = fopen(commit_path, "r");

  if (commit_file == NULL) {
    perror("Error opening latest commit");
    return 1;
  }

  char line[512];
  int reading_files = 0;
  int file_found = 0;

  while (fgets(line, sizeof(line), commit_file) != NULL) {
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

    if (filename != NULL &&
        strcmp(committed_filename, filename) != 0) {
      continue;
    }

    file_found = 1;

    diff_file(committed_filename, file_hash);

    if (filename != NULL) {
      break;
    }
  }

  fclose(commit_file);

  if (filename != NULL && !file_found) {
    printf(
      "Error: '%s' is not present in the latest commit.\n",
      filename
    );

    return 1;
  }

  if (!file_found) {
    printf("No files found in the latest commit.\n");
    return 1;
  }

  return 0;
}