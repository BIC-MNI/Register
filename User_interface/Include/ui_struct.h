#ifndef  DEF_UI_STRUCT
#define  DEF_UI_STRUCT

#include  <def_common_include.h>
#include  <def_graphics_window.h>
#include  <def_widgets.h>

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
    Volume_1_2_separator_viewport,
    Volume_2_merged_separator_viewport,
    Slice_1_2_separator_viewport,
    Slice_2_3_separator_viewport,
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
    Real                     slice_position_start;
} UI_struct;

typedef  struct
{
    int                       volume_index;
    volume_input_struct       input;
    volume_struct             volume;
    graphics_window_struct    graphics_window;
    polygons_struct           *meter_background;
    polygons_struct           *meter;
    widget_struct             *cancel_widget;
}
load_struct;

#endif
