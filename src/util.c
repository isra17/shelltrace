#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "util.h"

int readfile(char* filename, char** buf) {
  int fd = open(filename, O_RDONLY);
  if(fd < 0) {
    return -1;
  }

  *buf = 0;
  int res = 0;

  struct stat sb;
  if(fstat(fd, &sb)) {
    res = -1;
    goto CLEANUP;
  }

  *buf = malloc(sb.st_size);

  int nread = 0;
  int n = 0;

  do {
    n = read(fd, *buf, sb.st_size);
    if(n < 0) {
      res = -1;
      goto CLEANUP;
    }
    nread += n;
  } while(n != 0 && nread < sb.st_size);

  res = nread;

CLEANUP:
  close(fd);
  return res;
}

