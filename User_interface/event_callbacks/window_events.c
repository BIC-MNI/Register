/**
 * \file User_interface/event_callbacks/window_events.c
 * \brief Handle basic window callbacks.
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

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( redraw_overlay_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    set_clear_and_update_flags( ui );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( redraw_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    set_clear_and_update_flags( ui );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( resize_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    resize_layout( ui );

    position_main_widgets( ui );

    set_recreate_all_slice_flags();

    set_clear_and_update_flags( ui );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;
#ifdef DEBUG
    set_quit_program_flag();
#else
    popup_quit_confirm( ui );
#endif
}

/* ---- a hook to force update of colour map, when other program has
        screwed up the colour map ------------------------------------ */

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( deiconize_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    set_window_colours( ui->graphics_window.window );
}

static DEFINE_EVENT_FUNCTION(divider_mouse_move)
{
  UI_struct *ui = (UI_struct *) callback_data;
  int x, y;
  int x_size, y_size;

  if ( ui->divider_vp_index < 0 )
    return;

  G_get_mouse_position( ui->graphics_window.window, &x, &y);
  if (x != ui->prev_divider_x || y != ui->prev_divider_y)
  {
    ui->prev_divider_x = x;
    ui->prev_divider_y = y;
    
    G_get_window_size( ui->graphics_window.window, &x_size, &y_size );

    int y_volume_panel_start = ui->tag_panel_height + ui->divider_width;
    int y_volume_panel_end = y_volume_panel_start + ui->volume_panel_height-1;
    int y_slice_end = y_size - 1;
    int y_slice_start = y_volume_panel_end + 1 + ui->divider_width;
    double new_d = (double) (y - y_slice_start) / (y_slice_end - y_slice_start);

    int n = 1 - ui->divider_vp_index;

    /* When dragging the dividers, limit how small the panels can get.
     * Otherwise we can get mathematically weird results in resize_layout(),
     * and these can lead to instability elsewhere.
     */
    if ((n == 0 && new_d > 0.01 && (ui->y_slice_divider[1] - new_d) > 0.01) ||
        (n == 1 && new_d < 0.99 && (new_d - ui->y_slice_divider[0]) > 0.01))
    {
      ui->y_slice_divider[n] = new_d;
      resize_layout(ui);
    }
  }
}

static DEFINE_EVENT_FUNCTION(divider_mouse_up)
{
  UI_struct *ui = (UI_struct *) callback_data;
  int x, y;

  G_get_mouse_position( ui->graphics_window.window, &x, &y);

  remove_global_event_callback( NO_EVENT, divider_mouse_move, callback_data );
  remove_global_event_callback( LEFT_MOUSE_UP_EVENT, divider_mouse_up, callback_data );
  set_interaction_in_progress(FALSE);
  ui->divider_vp_index = -1;
}

static DEFINE_EVENT_FUNCTION(divider_mouse_down)
{
  UI_struct *ui = (UI_struct *) callback_data;

  ui->divider_vp_index = event_viewport_index - Slice_1_vert_sep_viewport;

  G_get_mouse_position( ui->graphics_window.window, 
                        &ui->prev_divider_x, &ui->prev_divider_y );

  add_global_event_callback( NO_EVENT, divider_mouse_move,
                             ANY_MODIFIER, (void *) ui );

  add_global_event_callback( LEFT_MOUSE_UP_EVENT, divider_mouse_up,
                             ANY_MODIFIER, (void *) ui );

  set_interaction_in_progress(TRUE);
}

  void  install_window_events(
    UI_struct  *ui )
{
    int i;

    set_event_viewport( &ui->graphics_window.event_viewports,
                        Whole_window_event_viewport, -1, -1, -1, -1 );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 REDRAW_OVERLAY_EVENT, -1, -1, -1, -1,
                                 redraw_overlay_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_REDRAW_EVENT, -1, -1, -1, -1,
                                 redraw_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_RESIZE_EVENT, -1, -1, -1, -1,
                                 resize_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_QUIT_EVENT, -1, -1, -1, -1,
                                 quit_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_DEICONIZED_EVENT, -1, -1, -1, -1,
                                 deiconize_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    for (i = 0; i < 2; i++)
    {
      add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                   Slice_1_vert_sep_viewport + i,
                                   LEFT_MOUSE_DOWN_EVENT, -1, -1, -1, -1,
                                   divider_mouse_down, ANY_MODIFIER,
                                   (void *) ui );
    }
}
