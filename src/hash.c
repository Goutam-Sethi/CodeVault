#include <stdio.h>

#include "hash.h"

unsigned long long calculate_hash(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }

  unsigned long long hash = 14695981039346656037ULL;
  int byte;

  while ((byte = fgetc(file)) != EOF) {
    hash ^= (unsigned char)byte;
    hash *= 1099511628211ULL;
  }

  fclose(file);

  return hash;
}