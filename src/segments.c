#include <stdlib.h>
#include <unicorn/unicorn.h>

#include "segments.h"
#include "util.h"

 int st_segments_from_file(struct st_segments** psegment, char* filename) {
  *psegment = 0;
  struct st_segments* segment = malloc(sizeof(struct st_segments));
  if(!segment) {
    return -1;
  }

  int size = readfile(filename, &segment->mem);
  if(size < 0) {
    st_segments_destroy(segment);
    return -1;
  }

  segment->address = -1;
  segment->perms = UC_PROT_ALL;
  segment->next = 0;

  *psegment = segment;
  return 0;
}

struct st_segments *st_segments_insert(struct st_segments *root,
                                       struct st_segments *segment) {
  segment->next = root;
  return segment;
}

struct st_segments* st_segments_next(struct st_segments* segments);

void st_segments_destroy(struct st_segments* segments);

