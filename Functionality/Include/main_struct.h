#ifndef  DEF_MAIN_STRUCT
#define  DEF_MAIN_STRUCT

#define  TWO_BUFFERS   2

typedef  struct
{
    Real            x_translation, y_translation;
    Real            x_scale, y_scale;
    Boolean         pixels_are_up_to_date;
    int             n_pixels_alloced;
    pixels_struct   *pixels;
    lines_struct    *cursor_lines;
} slice_struct;

#define  N_VIEWS     3

#define  N_VOXEL_VALUES  256

typedef struct
{
    Boolean                input_flag;
    volume_struct          volume;
    slice_struct           slices[N_VIEWS];
    Real                   position[N_DIMENSIONS];
    unsigned short         cmode_colour_map[N_VOXEL_VALUES];
    Colour                 rgb_colour_map[N_VOXEL_VALUES];
    int                    start_colour_map;
    int                    n_colour_entries;
    colour_coding_struct   colour_coding;
    colour_coding_struct   second_colour_coding;
}
trislice_struct;

#define  N_VOLUMES   3

typedef  struct
{
    window_struct            *window;
    graphics_struct          graphics;
    Boolean                  interpolation_flag;
    trislice_struct          trislice[N_VOLUMES];
} main_struct;

#endif
