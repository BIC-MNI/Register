#include  <def_user_interface.h>

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

private  DEFINE_EVENT_FUNCTION( update_voxel_cursor_callback )  /* ARGSUSED */
{
    update_move_voxel_cursor( get_ui_struct() );
}

private  DEFINE_EVENT_FUNCTION( left_mouse_up_callback )  /* ARGSUSED */
{
    update_move_voxel_cursor( get_ui_struct() );

    terminate_interaction( LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                           update_voxel_cursor_callback );
}

private  DEFINE_EVENT_FUNCTION( left_mouse_down_callback )   /* ARGSUSED */
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

    get_viewport_mouse_position( ui_info,
                                 ui_info->interaction_viewport_index,
                                 &x_mouse, &y_mouse);

    delta_slice = Move_slice_speed *
                  (Real) (y_mouse - ui_info->y_mouse_start);

    IF_get_volume_voxel_position( volume, position );

    position[view] = ui_info->slice_position_start + delta_slice;

    ui_set_volume_voxel_position( ui_info, volume, position );
}

private  DEFINE_EVENT_FUNCTION( update_move_slice_callback )  /* ARGSUSED */
{
    update_move_slice( get_ui_struct() );
}

private  DEFINE_EVENT_FUNCTION( middle_mouse_up_callback )  /* ARGSUSED */
{
    update_move_slice( get_ui_struct() );

    terminate_interaction( MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                           update_move_slice_callback );
}

private  DEFINE_EVENT_FUNCTION( middle_mouse_down_callback )   /* ARGSUSED */
{
    int   volume, view;
    Real  position[N_DIMENSIONS];

    start_interaction( get_ui_struct(), event_viewport_index,
                       MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                       update_move_slice_callback );

    ui_get_volume_view_index( event_viewport_index, &volume, &view );

    IF_get_volume_voxel_position( volume, position );

    get_ui_struct()->slice_position_start = position[view];
}

/* ------------------------- right mouse --------------------------- */

private  void  update_pan_slice(
     UI_struct *ui_info )
{
    int   volume, view, x_mouse, y_mouse;

    get_viewport_mouse_position( ui_info,
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

private  DEFINE_EVENT_FUNCTION( pan_slice_callback )  /* ARGSUSED */
{
    update_pan_slice( get_ui_struct() );
}

private  DEFINE_EVENT_FUNCTION( right_mouse_up_callback )  /* ARGSUSED */
{
    update_pan_slice( get_ui_struct() );

    terminate_interaction( RIGHT_MOUSE_UP_EVENT, right_mouse_up_callback,
                           pan_slice_callback );
}

private  DEFINE_EVENT_FUNCTION( right_mouse_down_callback )   /* ARGSUSED */
{
    start_interaction( get_ui_struct(), event_viewport_index,
                       RIGHT_MOUSE_UP_EVENT, right_mouse_up_callback,
                       pan_slice_callback );
}

public  void  install_slice_events(
    event_viewports_struct   *event_table,
    int                     volume )
{
    int  view;

    for_less( view, 0, N_SLICE_VIEWS )
    {
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     LEFT_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     left_mouse_down_callback, (void *) 0 );
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     MIDDLE_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     middle_mouse_down_callback, (void *) 0 );
        add_event_viewport_callback( event_table,
                                     ui_get_slice_viewport_index(volume,view),
                                     RIGHT_MOUSE_DOWN_EVENT,
                                     -1, -1, -1, -1,
                                     right_mouse_down_callback, (void *) 0 );
    }
}
