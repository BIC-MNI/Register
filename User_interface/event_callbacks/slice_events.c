/**
 * \file slice_events.c
 * \brief Handle mouse and keyboard events in the slice viewports.
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

static void time_step(UI_struct *ui_info,
                       Viewport_types viewport,
                       int dir)
{
    int  volume_index, view_index;
    VIO_Real tpos;

    if( is_slice_viewport( viewport ) )
    {
        ui_get_volume_view_index( viewport, &volume_index, &view_index );

        if( IF_volume_is_loaded( volume_index ) )
        {
            IF_get_volume_time_position(volume_index, &tpos);

            tpos += (VIO_Real) dir;

            ui_set_volume_time_position(ui_info, volume_index, tpos);
        }
    }
}

static  void  update_move_voxel_cursor(
     UI_struct *ui_info )
{
    int    volume_index;
    VIO_Real   voxel_position[VIO_MAX_DIMENSIONS];

    if( get_voxel_under_mouse( ui_info, ui_info->interaction_viewport_index,
                               &volume_index, voxel_position ) )
    {
        ui_set_volume_voxel_position( ui_info, volume_index, voxel_position );
        xs_display(ui_info, ui_info->interaction_viewport_index, 0);
    }
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( update_voxel_cursor_callback )
{
    update_move_voxel_cursor( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( left_mouse_up_callback )
{
    update_move_voxel_cursor( get_ui_struct() );

    terminate_interaction( LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                           update_voxel_cursor_callback );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( left_mouse_down_callback )
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                       update_voxel_cursor_callback );
    update_move_voxel_cursor( get_ui_struct() );
}

/* ------------------------- middle mouse --------------------------- */

static  void  update_move_slice(
     UI_struct *ui_info )
{
    int   volume, view, x_mouse, y_mouse;
    VIO_Real  position[VIO_MAX_DIMENSIONS], delta_slice;

    ui_get_volume_view_index( ui_info->interaction_viewport_index,
                              &volume, &view );

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 ui_info->interaction_viewport_index,
                                 &x_mouse, &y_mouse);

    delta_slice = Move_slice_speed *
                  (VIO_Real) (y_mouse - ui_info->y_mouse_start);

    IF_get_volume_voxel_position( volume, position );

    position[IF_get_slice_axis(view)] =
                        ui_info->slice_position_start + delta_slice;

    ui_set_volume_voxel_position( ui_info, volume, position );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( update_move_slice_callback )
{
    update_move_slice( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( middle_mouse_up_callback )
{
    update_move_slice( get_ui_struct() );

    restore_mouse_position( get_ui_struct() );

    terminate_interaction( MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                           update_move_slice_callback );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( middle_mouse_down_callback )
{
    int   volume, view;
    VIO_Real  position[VIO_MAX_DIMENSIONS];

    start_interaction( get_ui_struct(), event_viewport_index,
                       MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                       update_move_slice_callback );

    ui_get_volume_view_index( event_viewport_index, &volume, &view );

    IF_get_volume_voxel_position( volume, position );

    get_ui_struct()->slice_position_start = position[IF_get_slice_axis(view)];
}

/* ------------------------- pan slice mouse --------------------------- */

static  void  update_pan_slice(
     UI_struct *ui_info )
{
    int   volume, view, x_mouse, y_mouse;

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 ui_info->interaction_viewport_index,
                                 &x_mouse, &y_mouse);

    if( x_mouse != ui_info->x_mouse_start ||
        y_mouse != ui_info->y_mouse_start )
    {
        ui_get_volume_view_index( ui_info->interaction_viewport_index,
                                  &volume, &view );

        IF_translate_slice( volume, view,
                            x_mouse - ui_info->x_mouse_start,
                            y_mouse - ui_info->y_mouse_start );
        ui_info->x_mouse_start = x_mouse;
        ui_info->y_mouse_start = y_mouse;
    }
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( pan_slice_callback )
{
    update_pan_slice( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( terminate_pan_slice_callback )
{
    update_pan_slice( get_ui_struct() );

    terminate_interaction( LEFT_MOUSE_UP_EVENT, terminate_pan_slice_callback,
                           pan_slice_callback );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( start_pan_slice_callback )
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       LEFT_MOUSE_UP_EVENT, terminate_pan_slice_callback,
                       pan_slice_callback );
}

/* ------------------------- scale slice --------------------------- */

static  void  update_scale_slice(
     UI_struct *ui_info )
{
    int   volume, view, x_mouse, y_mouse;
    VIO_Real  scale_factor;

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 ui_info->interaction_viewport_index,
                                 &x_mouse, &y_mouse);

    if( y_mouse != ui_info->prev_y_mouse )
    {
        ui_get_volume_view_index( ui_info->interaction_viewport_index,
                                  &volume, &view );

        scale_factor = pow( 2.0, 1.0 / Pixels_per_double_size *
                            (double) (y_mouse - ui_info->prev_y_mouse) );

        IF_scale_slice( volume, view, scale_factor );
        ui_info->prev_y_mouse = y_mouse;
    }
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( scale_slice_callback )
{
    update_scale_slice( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( terminate_scale_slice_callback )
{
    update_scale_slice( get_ui_struct() );

    restore_mouse_position( get_ui_struct() );

    terminate_interaction( MIDDLE_MOUSE_UP_EVENT,
                           terminate_scale_slice_callback,
                           scale_slice_callback );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( start_scale_slice_callback )
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       MIDDLE_MOUSE_UP_EVENT, terminate_scale_slice_callback,
                       scale_slice_callback );
    get_ui_struct()->prev_y_mouse = get_ui_struct()->y_mouse_start;
}

static  void  increment_slice(
     UI_struct        *ui_info,
     Viewport_types   viewport,
     int              increment )
{
    int   volume, view, axis;
    VIO_Real  position[VIO_MAX_DIMENSIONS];

    if( is_slice_viewport( viewport ) )
    {
        ui_get_volume_view_index( viewport, &volume, &view );

        if( IF_volume_is_loaded( volume ) )
        {
            IF_get_volume_voxel_position( volume, position );
            axis = IF_get_slice_axis( view );

            position[axis] += (VIO_Real) increment;
            position[axis] = (VIO_Real) VIO_ROUND( position[axis] );

            ui_set_volume_voxel_position( ui_info, volume, position );
        }
    }
}

/**
 * Change the blending weights (relative volume opacities) by a fixed
 * step size in either direction, updating the UI to reflect the
 * change in the weights.  Works only in the BLEND_VOLUMES merge mode
 * for now.
 *
 * \param ui_info The main user interface structure.
 * \param direction Indicates the sign and number of steps to change the
 * opacity.
 */
static void
change_blending_weights(UI_struct *ui_info,
                        int direction)
{
  VIO_Real value = IF_get_merged_volume_opacity(0);
  value += 0.02 * direction;
  if (value < 0.0)
    value = 0.0;
  if (value > 1.0)
    value = 1.0;

  set_volume_blend_value( ui_info, value );
  set_slider_values( get_merged_blend_widget(ui_info), value, value );
}

/* ARGSUSED */

  DEFINE_EVENT_FUNCTION( slice_key_down_callback )
{
    switch( keyboard_character )
    {
    case  LEFT_ARROW_KEY:
        increment_slice( get_ui_struct(), event_viewport_index, -1 );
        break;

    case  RIGHT_ARROW_KEY:
        increment_slice( get_ui_struct(), event_viewport_index, +1 );
        break;

    case  UP_ARROW_KEY:
        previous_current_tag_point( get_ui_struct() );
        break;

    case  DOWN_ARROW_KEY:
        advance_current_tag_point( get_ui_struct() );
        break;

    case 's':
    case 'S':
        save_current_viewport( get_ui_struct(), event_viewport_index );
        break;

    case '<':
        time_step(get_ui_struct(), event_viewport_index, -1);
        break;

    case '>':
        time_step(get_ui_struct(), event_viewport_index, 1);
        break;

    case 't':
    case 'T':
        xs_display(get_ui_struct(), event_viewport_index, 1);
        break;

    case 'r':
        change_blending_weights(get_ui_struct(), 1);
        break;

    case 'e':
        change_blending_weights(get_ui_struct(), -1);
        break;

    case 'R':
        change_blending_weights( get_ui_struct(), 1000);
        break;

    case 'E':
        change_blending_weights( get_ui_struct(), -1000);
        break;
    }
}

static DEFINE_EVENT_FUNCTION(scroll_up_callback)
{
    int volume, view;

    ui_get_volume_view_index( event_viewport_index, &volume, &view );
    if (volume < 0 || view < 0)
      return;
    IF_scale_slice( volume, view, 1.1 );
}

static DEFINE_EVENT_FUNCTION(scroll_down_callback)
{
    int volume, view;

    ui_get_volume_view_index( event_viewport_index, &volume, &view );
    if (volume < 0 || view < 0)
      return;
    IF_scale_slice( volume, view, 1.0/1.1);
}

  void  install_slice_events(
    event_viewports_struct   *event_table,
    int                     volume )
{
    int  view;

    for_less( view, 0, N_VIEWS )
    {
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     LEFT_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     left_mouse_down_callback, NO_SHIFT_ONLY,
                                     (void *) 0 );
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     MIDDLE_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     middle_mouse_down_callback, NO_SHIFT_ONLY,
                                     (void *) 0 );
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     LEFT_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     start_pan_slice_callback, SHIFT_ONLY,
                                     (void *) 0 );
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     MIDDLE_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     start_scale_slice_callback, SHIFT_ONLY,
                                     (void *) 0 );

        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     SCROLL_UP_EVENT,
                                     -1, -1, -1, -1,
                                     scroll_up_callback, NO_SHIFT_ONLY,
                                     (void *) 0 );
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     SCROLL_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     scroll_down_callback, NO_SHIFT_ONLY,
                                     (void *) 0 );
    }
}

  void  remove_slice_events(
    event_viewports_struct   *event_table,
    int                     volume )
{
    int  view;

    for_less( view, 0, N_VIEWS )
    {
        remove_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     LEFT_MOUSE_DOWN_EVENT,
                                     left_mouse_down_callback, (void *) 0 );
        remove_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     MIDDLE_MOUSE_DOWN_EVENT,
                                     middle_mouse_down_callback, (void *) 0 );
        remove_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     LEFT_MOUSE_DOWN_EVENT,
                                     start_pan_slice_callback, (void *) 0 );
        remove_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     MIDDLE_MOUSE_DOWN_EVENT,
                                     start_scale_slice_callback, (void *) 0 );
        remove_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     SCROLL_UP_EVENT,
                                     scroll_up_callback, (void *) 0 );
        remove_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     SCROLL_DOWN_EVENT,
                                     scroll_down_callback, (void *) 0 );
    }
}
