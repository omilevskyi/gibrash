#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/param.h>
#include <sys/stat.h>

#include "include/gibrash.h"

#ifndef PATH_DELIMITER
#define PATH_DELIMITER "/"
#endif

#ifndef GIT_DIRECTORY
#define GIT_DIRECTORY ".git"
#endif

#ifndef GIT_HEAD_FILE
#define GIT_HEAD_FILE "HEAD"
#endif

#ifndef REFS_HEAD
// "ref: refs/heads/"
#define REFS_HEAD "refs/heads/"
#endif

#ifndef SHORT_HASH_LENGTH
#define SHORT_HASH_LENGTH 7
#endif

// static int find_directory(char *path, const size_t path_size,
//                           const char *directory) {
//   char current_path[MAXPATHLEN];
//   struct stat statbuf;
//   for (;;) {
//     if (getcwd(current_path, sizeof(current_path)) == NULL) {
//       perror("getcwd()");
//       return errno;
//     }
//     snprintf(path, path_size, "%s%s%s", current_path, PATH_DELIMITER,
//              directory);
//     if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
//       return 0;
//     if (current_path[1] == '\0' && current_path[0] == PATH_DELIMITER[0])
//       return -2;
//     if (chdir("..")) {
//       perror("chdir()");
//       return errno;
//     }
//   }
// }

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

// static int find_reference(char *result, const size_t result_size,
//                           const char *path) {
//   char file_path[MAXPATHLEN];
//   snprintf(file_path, sizeof(file_path), "%s%s", path,
//            PATH_DELIMITER GIT_HEAD_FILE);
//   int fd = open(file_path, O_RDONLY);
//   if (fd < 0)
//     return errno;
//   ssize_t bytes_read = read(fd, result, result_size - 1);
//   if (bytes_read < 0) {
//     errno_t rc = errno;
//     if (close(fd))
//       perror("close()");
//     return rc;
//   }
//   result[bytes_read] = '\0';
//   char *src = strcasestr(result, REFS_HEAD);
//   if (src != NULL) {
//     src += sizeof(REFS_HEAD) - 1;
//     while (*src != '\0' && *src != '\n') {
//       *result++ = *src++;
//     };
//     *result = '\0';
//   } else
//     result[SHORT_HASH_LENGTH] = '\0';
//   if (close(fd))
//     perror("close()");
//   return 0;
// }

static int find_reference(char *result, const size_t result_size,
                          const char *path) {
  char file_path[MAXPATHLEN];
  int fd;
  ssize_t bytes_read;

  if ((size_t)snprintf(file_path, sizeof(file_path), "%s%c%s", path,
                       PATH_DELIMITER[0], GIT_HEAD_FILE) >= sizeof(file_path))
    return ENAMETOOLONG;

  fd = open(file_path, O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    return errno;

  bytes_read = pread(fd, result, result_size - 1, 0);
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

int main(void) {
  char path[MAXPATHLEN], ref_name[MAXPATHLEN + sizeof("ref: ")];

  if (find_directory(path, sizeof(path), GIT_DIRECTORY) == 0 &&
      find_reference(ref_name, sizeof(ref_name), path) == 0)
    printf(" %s ", ref_name);

  return EXIT_SUCCESS;
}
