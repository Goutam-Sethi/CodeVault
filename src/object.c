#include <stdio.h>

#include "object.h"

int store_object(const char *filename, unsigned long long hash) {
  char object_path[256];

  snprintf(object_path, sizeof(object_path), ".codevault/objects/%llu", hash);

  FILE *existing = fopen(object_path, "rb");

  if (existing != NULL) {
    fclose(existing);
    return 0;
  }

  FILE *input = fopen(filename, "rb");

  if (input == NULL) {
    perror("Error opening file");
    return 1;
  }

  FILE *object = fopen(object_path, "wb");

  if (object == NULL) {
    perror("Error creating object");
    fclose(input);
    return 1;
  }

  char buffer[4096];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), input)) > 0) {
    fwrite(buffer, 1, bytes_read, object);
  }

  fclose(input);
  fclose(object);

  return 0;
}