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

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/layout/layout.c,v 1.8 1995-10-02 18:34:54 david Exp $";
#endif

#include  <user_interface.h>

public  void  initialize_layout( UI_struct  *ui_info )
{
    ui_info->main_menu_width = Default_main_menu_width;
    ui_info->tag_panel_height = Default_tag_panel_height;
    ui_info->volume_panel_height = Default_volume_panel_height;
    ui_info->divider_width = Default_divider_width;

    ui_info->x_slice_divider[0] = Slice_left_view_width;
    ui_info->x_slice_divider[1] = Slice_left_view_width +
                                  Slice_middle_view_width;
    ui_info->y_slice_divider[0] = 1.0 - Slice_top_view_height -
                                        Slice_middle_view_height;
    ui_info->y_slice_divider[1] = 1.0 - Slice_top_view_height;

    resize_layout( ui_info );
}

public  void  resize_layout( UI_struct  *ui_info )
{
    graphics_struct         *graphics;
    event_viewports_struct  *event_viewports;
    Viewport_types          viewport_index;
    UI_colours              ui_colour;
    int  x_size, y_size, divider_width;
    int  divider1, divider2;
    int  x_min, x_max, y_min, y_max;
    int  x_main_start, x_main_end, x_volume_1_start, x_volume_1_end;
    int  x_volume_2_start, x_volume_2_end, x_merged_start, x_merged_end;
    int  y_tag_start, y_tag_end, y_main_start, y_main_end;
    int  y_volume_panel_start, y_volume_panel_end;
    int  y_slice_1_start, y_slice_1_end;
    int  y_slice_2_start, y_slice_2_end, y_slice_3_start, y_slice_3_end;

    graphics = &ui_info->graphics_window.graphics;
    event_viewports = &ui_info->graphics_window.event_viewports;

    G_get_window_size( ui_info->graphics_window.window, &x_size, &y_size );

    divider_width = ui_info->divider_width;

    x_main_start = 0;
    x_main_end = ui_info->main_menu_width-1;
    x_volume_1_start = x_main_end + 1 + divider_width;
    x_merged_end = x_size - 1;

    divider1 = ROUND( x_volume_1_start + ui_info->x_slice_divider[0] *
                      (x_merged_end - x_volume_1_start) );
    divider2 = ROUND( x_volume_1_start + ui_info->x_slice_divider[1] *
                      (x_merged_end - x_volume_1_start) );

    x_volume_1_end = divider1 - divider_width/2 - 1;
    x_volume_2_start = x_volume_1_end + divider_width + 1;
    x_volume_2_end = divider2 - divider_width/2 - 1;
    x_merged_start = x_volume_2_end + divider_width + 1;


    y_tag_start = 0;
    y_tag_end = ui_info->tag_panel_height-1;
    y_main_start = y_tag_end+1 + divider_width;
    y_main_end = y_size-1;
    y_volume_panel_start = y_tag_end + 1 + divider_width;
    y_volume_panel_end = y_volume_panel_start + ui_info->volume_panel_height-1;
    y_slice_3_start = y_volume_panel_end + 1 + divider_width;
    y_slice_1_end = y_size - 1;

    divider1 = ROUND( y_slice_3_start + ui_info->y_slice_divider[0] *
                      (y_slice_1_end - y_slice_3_start) );
    divider2 = ROUND( y_slice_3_start + ui_info->y_slice_divider[1] *
                      (y_slice_1_end - y_slice_3_start) );

    y_slice_3_end = divider1 - divider_width/2 - 1;
    y_slice_2_start = y_slice_3_end + divider_width + 1;
    y_slice_2_end = divider2 - divider_width/2 - 1;
    y_slice_1_start = y_slice_2_end + divider_width + 1;

    set_graphics_viewport( graphics, Main_menu_viewport,
                           x_main_start, x_main_end,
                           y_main_start, y_main_end );

    set_graphics_viewport( graphics, RMS_error_viewport,
                           x_main_start, x_main_end,
                           y_tag_start, y_tag_end );

    set_graphics_viewport( graphics, Volume_1_tags_viewport,
                           x_volume_1_start, x_volume_1_end,
                           y_tag_start, y_tag_end );

    set_graphics_viewport( graphics, Volume_2_tags_viewport,
                           x_volume_2_start, x_volume_2_end,
                           y_tag_start, y_tag_end );

    set_graphics_viewport( graphics, Tag_names_viewport,
                           x_merged_start, x_merged_end,
                           y_tag_start, y_tag_end );

    set_graphics_viewport( graphics, Volume_1_menu_viewport,
                           x_volume_1_start, x_volume_1_end,
                           y_volume_panel_start, y_volume_panel_end );

    set_graphics_viewport( graphics, Volume_2_menu_viewport,
                           x_volume_2_start, x_volume_2_end,
                           y_volume_panel_start, y_volume_panel_end );

    set_graphics_viewport( graphics, Merged_menu_viewport,
                           x_merged_start, x_merged_end,
                           y_volume_panel_start, y_volume_panel_end );

    /* volume 1 */

    IF_set_slice_viewport( 0, 0,
                           x_volume_1_start, x_volume_1_end,
                           y_slice_1_start, y_slice_1_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(0,0),
                           x_volume_1_start, x_volume_1_end,
                           y_slice_1_start, y_slice_1_end );

    IF_set_slice_viewport( 0, 1,
                           x_volume_1_start, x_volume_1_end,
                           y_slice_2_start, y_slice_2_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(0,1),
                           x_volume_1_start, x_volume_1_end,
                           y_slice_2_start, y_slice_2_end );

    IF_set_slice_viewport( 0, 2,
                           x_volume_1_start, x_volume_1_end,
                           y_slice_3_start, y_slice_3_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(0,2),
                           x_volume_1_start, x_volume_1_end,
                           y_slice_3_start, y_slice_3_end );

    /* volume 2 */

    IF_set_slice_viewport( 1, 0,
                           x_volume_2_start, x_volume_2_end,
                           y_slice_1_start, y_slice_1_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(1,0),
                           x_volume_2_start, x_volume_2_end,
                           y_slice_1_start, y_slice_1_end );

    IF_set_slice_viewport( 1, 1,
                           x_volume_2_start, x_volume_2_end,
                           y_slice_2_start, y_slice_2_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(1,1),
                           x_volume_2_start, x_volume_2_end,
                           y_slice_2_start, y_slice_2_end );

    IF_set_slice_viewport( 1, 2,
                           x_volume_2_start, x_volume_2_end,
                           y_slice_3_start, y_slice_3_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(1,2),
                           x_volume_2_start, x_volume_2_end,
                           y_slice_3_start, y_slice_3_end );

    /* merged volume */

    IF_set_slice_viewport( 2, 0,
                           x_merged_start, x_merged_end,
                           y_slice_1_start, y_slice_1_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(2,0),
                           x_merged_start, x_merged_end,
                           y_slice_1_start, y_slice_1_end );

    IF_set_slice_viewport( 2, 1,
                           x_merged_start, x_merged_end,
                           y_slice_2_start, y_slice_2_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(2,1),
                           x_merged_start, x_merged_end,
                           y_slice_2_start, y_slice_2_end );

    IF_set_slice_viewport( 2, 2,
                           x_merged_start, x_merged_end,
                           y_slice_3_start, y_slice_3_end );
    set_event_viewport( event_viewports, ui_get_slice_viewport_index(2,2),
                           x_merged_start, x_merged_end,
                           y_slice_3_start, y_slice_3_end );

    /* dividers */

    set_graphics_viewport( graphics, Main_volume_1_separator_viewport,
                           x_main_end+1, x_volume_1_start-1, 0, y_size-1 );

    set_graphics_viewport( graphics, Volume_1_2_separator_viewport,
                           x_volume_1_end+1, x_volume_2_start-1, 0, y_size-1 );

    set_graphics_viewport( graphics,
                           Volume_2_merged_separator_viewport,
                           x_volume_2_end+1, x_merged_start-1, 0, y_size-1 );

    set_graphics_viewport( graphics, Slice_1_2_separator_viewport,
                           x_volume_1_start+1, x_size-1,
                           y_slice_2_end+1, y_slice_1_start-1 );

    set_graphics_viewport( graphics, Slice_2_3_separator_viewport,
                           x_volume_1_start+1, x_size-1,
                           y_slice_3_end+1, y_slice_2_start-1 );

    set_graphics_viewport( graphics, Slice_3_menu_separator_viewport,
                           x_volume_1_start+1, x_size-1,
                           y_volume_panel_end+1, y_slice_3_start-1 );

    set_graphics_viewport( graphics, Tag_volume_menu_separator_viewport,
                           0, x_size-1,
                           y_tag_end+1, y_volume_panel_start-1 );

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
    {
        get_graphics_viewport( graphics, viewport_index,
                               &x_min, &x_max, &y_min, &y_max );

        if( viewport_index < Main_volume_1_separator_viewport )
            ui_colour = BACKGROUND_COLOUR;
        else
            ui_colour = DIVIDER_COLOUR;

        set_graphics_viewport_background( graphics, viewport_index,
                                          get_ui_rgb_colour(ui_colour),
                                          get_ui_colour_index(ui_colour) );

        set_event_viewport( event_viewports, viewport_index,
                            x_min, x_max, y_min, y_max );
    }
}

public  BOOLEAN  is_slice_viewport(
    int   view_index )
{
    return( view_index >= N_VIEWPORT_TYPES );
}

public  void  ui_get_volume_view_index(
    int   view_index,
    int   *volume,
    int   *view )
{
    *volume = (view_index - N_VIEWPORT_TYPES) / N_VIEWS;
    *view = (view_index - N_VIEWPORT_TYPES) % N_VIEWS;
}

public  int  ui_get_slice_viewport_index(
    int   volume,
    int   view )
{
    return( N_VIEWPORT_TYPES + volume * N_VIEWS + view );
}
