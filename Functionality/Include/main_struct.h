#ifndef  DEF_MAIN_STRUCT
#define  DEF_MAIN_STRUCT

#include  <common_include.h>

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
    int             prev_viewport_x_size;
    int             prev_viewport_y_size;
    int             used_viewport_x_size;
    int             used_viewport_y_size;
    Filter_types    filter_type;
    Real            filter_width;
    BOOLEAN         pixels_are_up_to_date;
    int             n_pixels_alloced;
    pixels_struct   *pixels;
    lines_struct    *cursor_lines;
} slice_struct;

typedef struct
{
    BOOLEAN                    input_flag;
    Volume                     volume;
    STRING                     filename;
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
    BOOLEAN                active_flag;
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
    BOOLEAN                position_exists[N_VOLUMES];
    Point                  position[N_VOLUMES];
    STRING                 name;
    BOOLEAN                activity;
    object_struct          *objects[N_VOLUMES_DISPLAYED][N_VIEWS];
} tag_point_struct;

typedef  struct
{
    Trans_type             transform_type;
    int                    n_tag_points;
    tag_point_struct       *tag_points;
    BOOLEAN                transform_out_of_date;
    General_transform      v2_to_v1_transform;
    Real                   avg_rms_error;
    BOOLEAN                transform_exists;
    BOOLEAN                saved_flag;
    BOOLEAN                tags_visible;
} tag_list_struct;

typedef  struct
{
    window_struct            *window;
    graphics_struct          graphics;
    int                      degrees_continuity;
    trislice_struct          trislice[N_VOLUMES];
    merged_struct            merged;

    BOOLEAN                  resampled_file_loaded;
    STRING                   original_volume_filename;
    General_transform        resampling_transform;

    int                      start_colour_index;

    tag_list_struct          tags;

    void                     *render_storage;
} main_struct;

#endif
