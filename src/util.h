#ifndef ST_UTIL_H
#define ST_UTIL_H

#define MIN(a, b) ((a) < (b)? (a) : (b))
#define MAX(a, b) ((a) > (b)? (a) : (b))

int readfile(char* filename, char** buf);

#endif
