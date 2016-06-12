#ifndef ST_SEGMENTS_H
#define ST_SEGMENTS_H

#include <stdint.h>

typedef struct st_segments {
  char* mem;
  uint64_t size;
  uint64_t address;
  uint64_t perms;
  struct st_segments* next;
} segments;

int st_segments_from_file(struct st_segments** segment, char* filename);

struct st_segments *st_segments_insert(struct st_segments *root,
                                        struct st_segments *segment);
struct st_segments* st_segments_next(struct st_segments* segments);

void st_segments_destroy(struct st_segments* segments);

#endif

