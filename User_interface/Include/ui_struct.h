/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#ifndef  DEF_UI_STRUCT
#define  DEF_UI_STRUCT

#include  <common_include.h>
#include  <graphics_window.h>
#include  <widgets.h>
#include  <viewports.h>

typedef struct
{
    int   n_tags_displayed;
    int   first_tag_displayed;
    int   current_tag_index;
} tag_points_struct;

typedef  struct
{
    graphics_window_struct   graphics_window;

    VIO_BOOL                  volumes_synced;

    widgets_struct           widget_list[N_VIEWPORT_TYPES];
    int                      position_text_start_index[N_VOLUMES_DISPLAYED];

    tag_points_struct        tag_points;

    int                      main_menu_width;
    VIO_Real                     x_slice_divider[N_VOLUMES_DISPLAYED-1];
    VIO_Real                     y_slice_divider[N_VIEWS-1];
    int                      tag_panel_height;
    int                      volume_panel_height;
    int                      divider_width;

    Viewport_types           interaction_viewport_index;
    int                      x_mouse_start;
    int                      y_mouse_start;
    int                      prev_y_mouse;
    VIO_Real                     slice_position_start;

    VIO_General_transform        resampling_transform;
    VIO_STR                   original_filename_volume_2;
    VIO_STR                   resampled_filename;

    int                      which_volume_readout;
    int                      x_mouse_readout;
    int                      y_mouse_readout;
    /** Number of real, loaded or loadable volumes. */
    int                      n_volumes_loaded;
    /** One more than n_volumes_loaded, to account for the merged volume. */
    int                      n_volumes_displayed;
    int                      prev_divider_x;
    int                      prev_divider_y;
    int                      divider_vp_index;
} UI_struct;

typedef  struct popup_struct
{
    graphics_window_struct  graphics;
    widgets_struct          widgets;
}
popup_struct;

typedef  struct
{
    VIO_BOOL                   this_is_resampled_volume;
    VIO_STR                    filename;
    int                       volume_index;
    popup_struct              popup;
    polygons_struct           *meter_background;
    polygons_struct           *meter;
}
load_struct;

typedef  struct
{
    resample_struct           resample;
    VIO_Volume                    resampled_volume;
    popup_struct              popup;
    polygons_struct           *meter_background;
    polygons_struct           *meter;
}
resample_volume_struct;

#endif
