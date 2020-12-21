/**
 * \file User_interface/layout/layout.c
 * \brief Calculate window and widget placement and size.
 *
 * \copyright
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
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <user_interface.h>

unsigned int N_UI_viewports = N_VIEWPORT_TYPES - 1;

  void  initialize_layout( UI_struct  *ui_info )
{
    int i;

    ui_info->main_menu_width = Default_main_menu_width;
    ui_info->tag_panel_height = Default_tag_panel_height;
    ui_info->volume_panel_height = Default_volume_panel_height;
    ui_info->divider_width = Default_divider_width;

    for_less(i, 0, ui_info->n_volumes_displayed)
    {
        ui_info->x_slice_divider[i] = (double) (i+1) / ui_info->n_volumes_displayed;
    }
    ui_info->y_slice_divider[0] = 1.0 - Slice_top_view_height -
                                        Slice_middle_view_height;
    ui_info->y_slice_divider[1] = 1.0 - Slice_top_view_height;
    resize_layout( ui_info );
}

  void  resize_layout( UI_struct  *ui_info )
{
    graphics_struct         *graphics;
    event_viewports_struct  *event_viewports;
    Viewport_types          viewport_index;
    UI_colours              ui_colour;
    int  x_size, y_size, divider_width;
    int  divider1, divider2;
    int  x_min, x_max, y_min, y_max;
    int  x_main_start, x_main_end;
    int  x_volume_start[N_VOLUMES_DISPLAYED];
    int  x_volume_end[N_VOLUMES_DISPLAYED];
    int  y_tag_start, y_tag_end, y_main_start, y_main_end;
    int  y_volume_panel_start, y_volume_panel_end;
    int  y_slice_start[N_VIEWS];
    int  y_slice_end[N_VIEWS];
    int  i, j;

    graphics = &ui_info->graphics_window.graphics;
    event_viewports = &ui_info->graphics_window.event_viewports;

    G_get_window_size( ui_info->graphics_window.window, &x_size, &y_size );

    divider_width = ui_info->divider_width;

    x_main_start = 0;
    x_main_end = ui_info->main_menu_width-1;

    x_volume_start[0] = x_main_end + 1 + divider_width;
    x_volume_end[MERGED_VOLUME_INDEX] = x_size - 1;
    for (i = 1; i < ui_info->n_volumes_displayed; i++)
    {
      divider1 = VIO_ROUND( x_volume_start[0] + ui_info->x_slice_divider[i-1] *
                            (x_volume_end[MERGED_VOLUME_INDEX] - x_volume_start[0]) );

      x_volume_end[i-1] = divider1 - divider_width/2 - 1;
      x_volume_start[i] = x_volume_end[i-1] + divider_width + 1;
    }

    y_tag_start = 0;
    y_tag_end = ui_info->tag_panel_height-1;
    y_main_start = y_tag_end+1 + divider_width;
    y_main_end = y_size-1;
    y_volume_panel_start = y_tag_end + 1 + divider_width;
    y_volume_panel_end = y_volume_panel_start + ui_info->volume_panel_height-1;
    y_slice_start[2] = y_volume_panel_end + 1 + divider_width;
    y_slice_end[0] = y_size - 1;

    divider1 = VIO_ROUND( y_slice_start[2] + ui_info->y_slice_divider[0] *
                          (y_slice_end[0] - y_slice_start[2]) );
    divider2 = VIO_ROUND( y_slice_start[2] + ui_info->y_slice_divider[1] *
                          (y_slice_end[0] - y_slice_start[2]) );

    y_slice_end[2] = divider1 - divider_width/2 - 1;
    y_slice_start[1] = y_slice_end[2] + divider_width + 1;
    y_slice_end[1] = divider2 - divider_width/2 - 1;
    y_slice_start[0] = y_slice_end[1] + divider_width + 1;

    set_graphics_viewport( graphics, Main_menu_viewport,
                           x_main_start, x_main_end,
                           y_main_start, y_main_end );

    set_graphics_viewport( graphics, RMS_error_viewport,
                           x_main_start, x_main_end,
                           y_tag_start, y_tag_end );

    for_less( i, 0, ui_info->n_volumes_displayed )
    {
      set_graphics_viewport( graphics, Volume_1_tags_viewport + i,
                             x_volume_start[i], x_volume_end[i],
                             y_tag_start, y_tag_end );

      set_graphics_viewport( graphics, Volume_1_menu_viewport + i,
                             x_volume_start[i], x_volume_end[i],
                             y_volume_panel_start, y_volume_panel_end );

      for_less( j, 0, N_VIEWS )
      {
        IF_set_slice_viewport( i, j,
                               x_volume_start[i], x_volume_end[i],
                               y_slice_start[j], y_slice_end[j] );

        set_event_viewport( event_viewports, ui_get_slice_viewport_index(i, j),
                            x_volume_start[i], x_volume_end[i],
                            y_slice_start[j], y_slice_end[j] );
      }
    }

    /* dividers */

    for (i = 0; i < ui_info->n_volumes_displayed; i++) {
      set_graphics_viewport( graphics, Volume_1_horz_sep_viewport + i,
                             x_volume_start[i]-(divider_width+1), x_volume_start[i]-1, 0, y_size-1 );
    }

    set_graphics_viewport( graphics, Slice_1_vert_sep_viewport + 0,
                           x_volume_start[0]+1, x_size-1,
                           y_slice_end[1]+1, y_slice_start[0]-1 );

    set_graphics_viewport( graphics, Slice_1_vert_sep_viewport + 1,
                           x_volume_start[0]+1, x_size-1,
                           y_slice_end[2]+1, y_slice_start[1]-1 );

    set_graphics_viewport( graphics, Slice_1_vert_sep_viewport + 2,
                           x_volume_start[0]+1, x_size-1,
                           y_volume_panel_end+1, y_slice_start[2]-1 );

    set_graphics_viewport( graphics, Slice_1_vert_sep_viewport + 3,
                           0, x_size-1,
                           y_tag_end+1, y_volume_panel_start-1 );

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
    {
        get_graphics_viewport( graphics, (int) viewport_index,
                               &x_min, &x_max, &y_min, &y_max );

        if( viewport_index < Volume_1_horz_sep_viewport )
            ui_colour = BACKGROUND_COLOUR;
        else
            ui_colour = DIVIDER_COLOUR;

        set_graphics_viewport_background( graphics, (int) viewport_index,
                                          get_ui_rgb_colour(ui_colour),
                                          get_ui_colour_index(ui_colour) );

        set_event_viewport( event_viewports, viewport_index,
                            x_min, x_max, y_min, y_max );
    }
}

  VIO_BOOL  is_slice_viewport(
    Viewport_types   viewport )
{
    return( viewport >= N_VIEWPORT_TYPES );
}

  void  ui_get_volume_view_index(
    Viewport_types   view_index,
    int              *volume,
    int              *view )
{
    *volume = ((int) view_index - N_VIEWPORT_TYPES) / N_VIEWS;
    *view = ((int) view_index - N_VIEWPORT_TYPES) % N_VIEWS;
}

  Viewport_types  ui_get_slice_viewport_index(
    int   volume,
    int   view )
{
    return( (Viewport_types) (N_VIEWPORT_TYPES + volume * N_VIEWS + view) );
}
