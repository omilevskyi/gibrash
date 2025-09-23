#include <stdio.h>

static int find_directory(char *path, const size_t path_size,
                          const char *directory);

static int find_reference(char *result, const size_t result_size,
                          const char *path);
