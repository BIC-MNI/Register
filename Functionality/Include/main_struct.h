#ifndef  DEF_MAIN_STRUCT
#define  DEF_MAIN_STRUCT

#include  <def_common_include.h>

#define  TWO_BUFFERS            2
#define  N_VOXEL_VALUES       256
#define  N_MERGED               2

typedef enum { BACKGROUND_COLOUR,
               TAG_INSIDE_COLOUR,
               TAG_OUTSIDE_COLOUR,
               TAG_INSIDE_INACTIVE_COLOUR,
               TAG_OUTSIDE_INACTIVE_COLOUR,
               CURSOR_INSIDE_COLOUR,
               CURSOR_OUTSIDE_COLOUR,
               N_MAIN_COLOURS } Main_colours;

typedef  struct
{
    Real            x_translation, y_translation;
    Real            x_scale, y_scale;
    Real            lower_display_limits[N_DIMENSIONS-1];
    Real            upper_display_limits[N_DIMENSIONS-1];
    Filter_types    filter_type;
    Real            filter_width;
    Boolean         pixels_are_up_to_date;
    int             n_pixels_alloced;
    pixels_struct   *pixels;
    lines_struct    *cursor_lines;
} slice_struct;

typedef struct
{
    Boolean                    input_flag;
    Volume                     volume;
    String                     filename;
    slice_struct               slices[N_VIEWS];
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
    slice_struct           slices[N_VIEWS];
    Real                   position[N_DIMENSIONS];
    unsigned short         **cmode_colour_map;
    Colour                 **rgb_colour_map;
    int                    start_colour_map;
    int                    n_colour_entries1;
    int                    n_colour_entries2;
    Merge_methods          merge_method;
    Real                   opacity[N_MERGED];
    colour_coding_struct   colour_coding[N_MERGED];
}
merged_struct;

typedef  struct
{
    Real                   rms_error;
    Boolean                position_exists[N_VOLUMES];
    Point                  position[N_VOLUMES];
    String                 name;
    Boolean                activity;
    object_struct          *objects[N_VOLUMES_DISPLAYED][N_VIEWS];
} tag_point_struct;

typedef  struct
{
    int                    n_tag_points;
    tag_point_struct       *tag_points;
    Boolean                transform_out_of_date;
    General_transform      v2_to_v1_transform;
    Real                   avg_rms_error;
    Boolean                transform_exists;
    Boolean                saved_flag;
    Boolean                tags_visible;
} tag_list_struct;

typedef  struct
{
    window_struct            *window;
    graphics_struct          graphics;
    Boolean                  interpolation_flag;
    trislice_struct          trislice[N_VOLUMES];
    merged_struct            merged;

    Boolean                  resampled_file_loaded;
    String                   original_volume_filename;
    General_transform        resampling_transform;

    int                      start_colour_index;

    tag_list_struct          tags;
} main_struct;

#endif
