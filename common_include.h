#ifndef  DEF_COMMON_INCLUDE
#define  DEF_COMMON_INCLUDE

#define  N_VOLUMES                         2
#define  N_VOLUMES_DISPLAYED               3
#define  N_VIEWS                           3
#define  MERGED_VOLUME_INDEX       N_VOLUMES
#define  RESAMPLED_VOLUME_INDEX            1

typedef  enum  { ONE_ON_TWO, TWO_ON_ONE, BLEND_VOLUMES, WEIGHTED_VOLUMES }
                 Merge_methods;

#endif
