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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/event_callbacks/slice_events.c,v 1.12 1995-07-31 19:54:21 david Exp $";
#endif

#include  <user_interface.h>

private  void  update_move_voxel_cursor(
     UI_struct *ui_info )
{
    int    volume_index;
    Real   voxel_position[N_DIMENSIONS];

    if( get_voxel_under_mouse( ui_info, ui_info->interaction_viewport_index,
                               &volume_index, voxel_position ) )
    {
        ui_set_volume_voxel_position( ui_info, volume_index, voxel_position );
    }
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( update_voxel_cursor_callback )
{
    update_move_voxel_cursor( get_ui_struct() );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( left_mouse_up_callback )
{
    update_move_voxel_cursor( get_ui_struct() );

    terminate_interaction( LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                           update_voxel_cursor_callback );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( left_mouse_down_callback )
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                       update_voxel_cursor_callback );
    update_move_voxel_cursor( get_ui_struct() );
}

/* ------------------------- middle mouse --------------------------- */

private  void  update_move_slice(
     UI_struct *ui_info )
{
    int   volume, view, x_mouse, y_mouse;
    Real  position[N_DIMENSIONS], delta_slice;

    ui_get_volume_view_index( ui_info->interaction_viewport_index,
                              &volume, &view );

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 ui_info->interaction_viewport_index,
                                 &x_mouse, &y_mouse);

    delta_slice = Move_slice_speed *
                  (Real) (y_mouse - ui_info->y_mouse_start);

    IF_get_volume_voxel_position( volume, position );

    position[IF_get_slice_axis(view)] =
                        ui_info->slice_position_start + delta_slice;

    ui_set_volume_voxel_position( ui_info, volume, position );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( update_move_slice_callback )
{
    update_move_slice( get_ui_struct() );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( middle_mouse_up_callback )
{
    update_move_slice( get_ui_struct() );

    restore_mouse_position( get_ui_struct() );

    terminate_interaction( MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                           update_move_slice_callback );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( middle_mouse_down_callback )
{
    int   volume, view;
    Real  position[N_DIMENSIONS];

    start_interaction( get_ui_struct(), event_viewport_index,
                       MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                       update_move_slice_callback );

    ui_get_volume_view_index( event_viewport_index, &volume, &view );

    IF_get_volume_voxel_position( volume, position );

    get_ui_struct()->slice_position_start = position[IF_get_slice_axis(view)];
}

/* ------------------------- pan slice mouse --------------------------- */

private  void  update_pan_slice(
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

private  DEFINE_EVENT_FUNCTION( pan_slice_callback )
{
    update_pan_slice( get_ui_struct() );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( terminate_pan_slice_callback )
{
    update_pan_slice( get_ui_struct() );

    terminate_interaction( LEFT_MOUSE_UP_EVENT, terminate_pan_slice_callback,
                           pan_slice_callback );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( start_pan_slice_callback )
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       LEFT_MOUSE_UP_EVENT, terminate_pan_slice_callback,
                       pan_slice_callback );
}

/* ------------------------- scale slice --------------------------- */

private  void  update_scale_slice(
     UI_struct *ui_info )
{
    int   volume, view, x_mouse, y_mouse;
    Real  scale_factor;

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

private  DEFINE_EVENT_FUNCTION( scale_slice_callback )
{
    update_scale_slice( get_ui_struct() );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( terminate_scale_slice_callback )
{
    update_scale_slice( get_ui_struct() );

    restore_mouse_position( get_ui_struct() );

    terminate_interaction( MIDDLE_MOUSE_UP_EVENT,
                           terminate_scale_slice_callback,
                           scale_slice_callback );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( start_scale_slice_callback )
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       MIDDLE_MOUSE_UP_EVENT, terminate_scale_slice_callback,
                       scale_slice_callback );
    get_ui_struct()->prev_y_mouse = get_ui_struct()->y_mouse_start;
}

private  void  increment_slice(
     UI_struct        *ui_info,
     Viewport_types   viewport,
     int              increment )
{
    int   volume, view, axis;
    Real  position[N_DIMENSIONS];

    if( is_slice_viewport( viewport ) )
    {
        ui_get_volume_view_index( viewport, &volume, &view );

        if( IF_volume_is_loaded( volume ) )
        {
            IF_get_volume_voxel_position( volume, position );
            axis = IF_get_slice_axis( view );

            position[axis] += (Real) increment;
            position[axis] = (Real) ROUND( position[axis] );

            ui_set_volume_voxel_position( ui_info, volume, position );
        }
    }
}

/* ARGSUSED */

public  DEFINE_EVENT_FUNCTION( slice_key_down_callback )
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

    case  'S':
        save_current_viewport( get_ui_struct(), event_viewport_index );
        break;
    }
}

public  void  install_slice_events(
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
    }
}

public  void  remove_slice_events(
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
    }
}
