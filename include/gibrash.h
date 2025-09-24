#if !defined(__FreeBSD__) && !defined(__OpenBSD__) && !defined(__NetBSD__)
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>

#ifndef PROG
#define PROG "gibrash"
#endif

#ifndef VERSION
#define VERSION "devel"
#endif

#ifndef COMMIT_HASH
#define COMMIT_HASH "none"
#endif

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

static int find_directory(char *path, const size_t path_size,
                          const char *directory);

static int find_reference(char *result, const size_t result_size,
                          const char *path);
