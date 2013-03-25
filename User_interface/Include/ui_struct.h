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
static char ui_struct_rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/Include/ui_struct.h,v 1.15 1998-06-29 15:01:52 david Exp $";
#endif

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

    widgets_struct           widget_list[N_UI_viewports];
    int                      position_text_start_index[N_VOLUMES_DISPLAYED];

    tag_points_struct        tag_points;

    int                      main_menu_width;
    Real                     x_slice_divider[N_VOLUMES_DISPLAYED-1];
    Real                     y_slice_divider[N_VIEWS-1];
    int                      tag_panel_height;
    int                      volume_panel_height;
    int                      divider_width;

    Viewport_types           interaction_viewport_index;
    int                      x_mouse_start;
    int                      y_mouse_start;
    int                      prev_y_mouse;
    Real                     slice_position_start;

    VIO_General_transform        resampling_transform;
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
    VIO_BOOL                   this_is_resampled_volume;
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
