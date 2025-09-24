#include "include/gibrash.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/param.h>
#include <sys/stat.h>

#include "include/option_string.h"

static int find_directory(char *path, const size_t path_size,
                          const char *directory) {
  char current_path[MAXPATHLEN];
  struct stat statbuf;

  if (getcwd(current_path, sizeof(current_path)) == NULL) {
    perror("getcwd()");
    return errno;
  }

  for (;;) {
    if ((size_t)snprintf(path, path_size, "%s%c%s", current_path,
                         PATH_DELIMITER[0], directory) >= path_size)
      return ENAMETOOLONG;

    if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
      return 0;

    if (!strcmp(current_path, PATH_DELIMITER))
      return -2;

    char *slash = strrchr(current_path, PATH_DELIMITER[0]);
    if (!slash)
      return -3;

    if (slash == current_path)
      current_path[1] = '\0';
    else
      *slash = '\0';
  }
}

static int find_reference(char *result, const size_t result_size,
                          const char *path) {
  char file_path[MAXPATHLEN];

  if ((size_t)snprintf(file_path, sizeof(file_path), "%s%c%s", path,
                       PATH_DELIMITER[0], GIT_HEAD_FILE) >= sizeof(file_path))
    return ENAMETOOLONG;

  int fd = open(file_path, O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    return errno;

  ssize_t bytes_read = read(fd, result, result_size - 1);
  if (bytes_read < 0) {
    int rc = errno;
    close(fd);
    return rc;
  }
  result[bytes_read] = '\0';
  close(fd);

  char *src = strstr(result, REFS_HEAD);
  if (src) {
    src += sizeof(REFS_HEAD) - 1;
    char *dst = result;
    while (*src && *src != '\n' && (size_t)(dst - result) < (result_size - 1))
      *dst++ = *src++;
    *dst = '\0';
  } else if (bytes_read > SHORT_HASH_LENGTH)
    result[SHORT_HASH_LENGTH] = '\0';

  return 0;
}

static int print_version(int rc) {
  printf("%s %s (%s)\n", PROG, VERSION, COMMIT_HASH);
  return rc;
}

static int print_usage(int rc) {
  printf("Usage: %s\n"
         "\n"
         "Print the current git branch name in the fastest way.\n"
         "It is mostly intended to be used in a bash prompt.\n"
         "\n"
         "Options:\n"
         "      --version    Show version and exit\n"
         "  -h, --help       Show this help and exit\n"
         "\n",
         PROG);
  return rc;
}

int main(int argc, char **argv) {
  char path[MAXPATHLEN], ref_name[MAXPATHLEN + sizeof("ref: ")];

  if (argc > 1) {
    static struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 1},
        {NULL, 0, NULL, 0},
    };

    int opt, opt_idx = 0;
    char opt_string[option_string(long_opts, NULL, 0) + 1];
    option_string(long_opts, opt_string, sizeof opt_string);

    while ((opt = getopt_long(argc, argv, opt_string, long_opts, &opt_idx)) !=
           -1) {
      switch (opt) {
      case 'h':
        return print_usage(EXIT_SUCCESS);
      case 1: /* --version */
        return print_version(EXIT_SUCCESS);
      default:
        return print_usage(201);
      }
    }
  }

  if (find_directory(path, sizeof(path), GIT_DIRECTORY) == 0 &&
      find_reference(ref_name, sizeof(ref_name), path) == 0)
    printf(" %s ", ref_name);

  return EXIT_SUCCESS;
}
