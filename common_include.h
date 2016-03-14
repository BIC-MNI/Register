#ifndef  DEF_COMMON_INCLUDE
#define  DEF_COMMON_INCLUDE

#include  <volume_io.h>
#include  <graphics.h>

#define  N_VOLUMES                 4
#define  N_VOLUMES_DISPLAYED       (N_VOLUMES+1)
#define  N_VIEWS                   3
extern int MERGED_VOLUME_INDEX;
#define  RESAMPLED_VOLUME_INDEX    1

/* Define the location of the time dimension for our volume_io calls.
 */
#ifndef VIO_T
#define VIO_T 3
#endif

#define  TAG_FILE_SUFFIX                "tag"

typedef  enum  { ONE_ON_TWO, TWO_ON_ONE, BLEND_VOLUMES, WEIGHTED_VOLUMES }
                 Merge_methods;

#endif
