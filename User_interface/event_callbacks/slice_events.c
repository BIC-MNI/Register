#include  <def_register.h>

private  void  update_move_voxel_cursor(
     main_struct *main )
{
    int    volume_index;
    Real   voxel_position[N_DIMENSIONS];

    if( get_voxel_under_mouse( main, main->interaction_viewport_index,
                               &volume_index, voxel_position ) )
    {
        set_volume_position( main, volume_index, voxel_position );
    }
}

private  DEFINE_EVENT_FUNCTION( update_voxel_cursor_callback )  /* ARGSUSED */
{
    update_move_voxel_cursor( main );
}

private  DEFINE_EVENT_FUNCTION( left_mouse_up_callback )  /* ARGSUSED */
{
    update_move_voxel_cursor( main );

    terminate_interaction( LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                           update_voxel_cursor_callback );
}

private  DEFINE_EVENT_FUNCTION( left_mouse_down_callback )   /* ARGSUSED */
{
    start_interaction( main, viewport_index,
                       LEFT_MOUSE_UP_EVENT, left_mouse_up_callback,
                       update_voxel_cursor_callback );
    update_move_voxel_cursor( main );
}

/* ------------------------- middle mouse --------------------------- */

private  void  update_pan_slice(
     main_struct *main )
{
    int   volume, view, x_mouse, y_mouse;

    if( get_volume_view_index( main->interaction_viewport_index,
                               &volume, &view ) )
    {
        get_mouse_viewport_position( main, volume, view, &x_mouse, &y_mouse );

        if( x_mouse != main->x_mouse_start ||
            y_mouse != main->y_mouse_start )
        {
            main->trislice[volume].slices[view].x_translation +=
                                          x_mouse - main->x_mouse_start;
            main->trislice[volume].slices[view].y_translation +=
                                          y_mouse - main->y_mouse_start;
            set_recreate_slice_pixels_flag( main, volume, view );
            main->x_mouse_start = x_mouse;
            main->y_mouse_start = y_mouse;
        }
    }
}

private  DEFINE_EVENT_FUNCTION( pan_slice_callback )  /* ARGSUSED */
{
    update_pan_slice( main );
}

private  DEFINE_EVENT_FUNCTION( middle_mouse_up_callback )  /* ARGSUSED */
{
    update_pan_slice( main );

    terminate_interaction( MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                           pan_slice_callback );
}

private  DEFINE_EVENT_FUNCTION( middle_mouse_down_callback )   /* ARGSUSED */
{
    start_interaction( main, viewport_index,
                       MIDDLE_MOUSE_UP_EVENT, middle_mouse_up_callback,
                       pan_slice_callback );
}

private  DEFINE_EVENT_FUNCTION( right_mouse_down_callback )   /* ARGSUSED */
{
    print( "Right mouse down\n" );
}

public  void  install_slice_events(
    rectangles_action_struct   *action_table,
    int                        volume )
{
    int  view;

    for_less( view, 0, N_VIEWS )
    {
        add_rectangle_callback( action_table,
                                get_slice_viewport_index(volume,view),
                                LEFT_MOUSE_DOWN_EVENT,
                                left_mouse_down_callback );
        add_rectangle_callback( action_table,
                                get_slice_viewport_index(volume,view),
                                MIDDLE_MOUSE_DOWN_EVENT,
                                middle_mouse_down_callback );
    }
}
