#ifndef  DEF_COMMON_INCLUDE
#define  DEF_COMMON_INCLUDE

#include  <volume_io.h>
#include  <graphics.h>

/**
 * This is the _maximum_ number of volumes that can be loaded at once.
 */
#define  N_VOLUMES                 8

/**
 * This is the _maximum_ number of volumes that can be displayed, including
 * the final "merged" view.
 */
#define  N_VOLUMES_DISPLAYED       (N_VOLUMES + 1)

#define  N_VIEWS                   3

extern int MERGED_VOLUME_INDEX;

#define  RESAMPLED_VOLUME_INDEX    1

/**
 * We only enable the merged display if there are at least this many
 * loaded volumes.
 */
#define MIN_MERGED_VOLUMES 2

/**
 * Define the location of the time dimension for our volume_io calls.
 */
#ifndef VIO_T
#define VIO_T 3
#endif

#define  TAG_FILE_SUFFIX                "tag"

typedef  enum  { DISABLED, BLENDED, OPAQUE } Merge_methods;

#endif
