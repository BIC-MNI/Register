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

#ifndef lint
static char ui_struct_rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/Include/ui_struct.h,v 1.13 1995-12-11 19:31:32 david Exp $";
#endif

#include  <common_include.h>
#include  <graphics_window.h>
#include  <widgets.h>

typedef  enum
{
    Main_menu_viewport,
    RMS_error_viewport,
    Volume_1_tags_viewport,
    Volume_2_tags_viewport,
    Tag_names_viewport,
    Volume_1_menu_viewport,
    Volume_2_menu_viewport,
    Merged_menu_viewport,
    Main_volume_1_separator_viewport,
    Volume_1_2_separator_viewport,
    Volume_2_merged_separator_viewport,
    Slice_1_2_separator_viewport,
    Slice_2_3_separator_viewport,
    Slice_3_menu_separator_viewport,
    Tag_volume_menu_separator_viewport,
    N_UI_viewports,
    Whole_window_event_viewport = N_UI_viewports,
    N_VIEWPORT_TYPES
} Viewport_types;

typedef struct
{
    int   n_tags_displayed;
    int   first_tag_displayed;
    int   current_tag_index;
} tag_points_struct;

typedef  struct
{
    graphics_window_struct   graphics_window;

    BOOLEAN                  volumes_synced;

    widgets_struct           widget_list[N_UI_viewports];
    int                      position_text_start_index[N_VOLUMES_DISPLAYED];

    tag_points_struct        tag_points;

    int                      main_menu_width;
    Real                     x_slice_divider[N_VOLUMES_DISPLAYED-1];
    Real                     y_slice_divider[N_VIEWS-1];
    int                      tag_panel_height;
    int                      volume_panel_height;
    int                      divider_width;

    int                      interaction_viewport_index;
    int                      x_mouse_start;
    int                      y_mouse_start;
    int                      prev_y_mouse;
    Real                     slice_position_start;

    General_transform        resampling_transform;
    STRING                   original_filename_volume_2;
    STRING                   resampled_filename;

    int                      which_volume_readout;
    int                      x_mouse_readout;
    int                      y_mouse_readout;
} UI_struct;

typedef  struct popup_struct
{
    graphics_window_struct  graphics;
    widgets_struct          widgets;
}
popup_struct;

typedef  struct
{
    BOOLEAN                   this_is_resampled_volume;
    STRING                    filename;
    int                       volume_index;
    popup_struct              popup;
    polygons_struct           *meter_background;
    polygons_struct           *meter;
}
load_struct;

typedef  struct
{
    resample_struct           resample;
    Volume                    resampled_volume;
    popup_struct              popup;
    polygons_struct           *meter_background;
    polygons_struct           *meter;
}
resample_volume_struct;

#endif
