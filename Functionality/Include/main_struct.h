#ifndef  DEF_MAIN_STRUCT
#define  DEF_MAIN_STRUCT

#include  <def_common_include.h>

#define  TWO_BUFFERS            2
#define  N_VOXEL_VALUES       256
#define  N_MERGED               2

typedef  struct
{
    Real            x_translation, y_translation;
    Real            x_scale, y_scale;
    Boolean         pixels_are_up_to_date;
    int             n_pixels_alloced;
    pixels_struct   *pixels;
    lines_struct    *cursor_lines;
} one_volume_slice_struct;

typedef  struct
{
    Real            x_translation, y_translation;
    Real            x_scale, y_scale;
    Boolean         pixels_are_up_to_date;
    int             n_pixels_alloced;
    pixels_struct   *pixels;
    lines_struct    *cursor_lines;
} merged_slice_struct;

typedef struct
{
    Boolean                    input_flag;
    volume_struct              volume;
    one_volume_slice_struct    slices[N_VIEWS];
    Real                       position[N_DIMENSIONS];
    unsigned short             cmode_colour_map[N_VOXEL_VALUES];
    Colour                     rgb_colour_map[N_VOXEL_VALUES];
    int                        start_colour_map;
    int                        n_colour_entries;
    colour_coding_struct       colour_coding;
}
trislice_struct;

typedef  struct
{
    Boolean                active_flag;
    merged_slice_struct    slices[N_VIEWS];
    Real                   position[N_DIMENSIONS];
    unsigned short         **cmode_colour_map;
    Colour                 **rgb_colour_map;
    int                    start_colour_map;
    int                    n_colour_entries1;
    int                    n_colour_entries2;
    Real                   opacity[N_MERGED];
    colour_coding_struct   colour_coding[N_MERGED];
}
merged_struct;

typedef  struct
{
    window_struct            *window;
    graphics_struct          graphics;
    Boolean                  interpolation_flag;
    trislice_struct          trislice[N_VOLUMES];
    merged_struct            merged;
} main_struct;

#endif
