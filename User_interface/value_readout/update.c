#include  <def_user_interface.h>

private  const  NO_VOLUME = -1;

private  void  set_readout_activity(
    UI_struct  *ui,
    int        which_volume,
    Boolean    activity )
{
    if( which_volume < N_VOLUMES )
    {
        set_widget_activity( get_volume_readout_widget(ui,which_volume),
                             activity );
    }
    else
    {
        set_widget_activity( get_merged_readout_widget(ui,0), activity );
        set_widget_activity( get_merged_readout_widget(ui,1), activity );
    }
}

private  void  modify_readout(
    UI_struct  *ui,
    int        x_mouse,
    int        y_mouse,
    int        *volume_index )
{
    int    viewport_index;
    Real   value;
    Real   voxel_position[N_DIMENSIONS];

    if( find_viewport_containing_mouse( &ui->graphics_window.event_viewports,
                                        x_mouse, y_mouse, &viewport_index ) &&
        get_voxel_under_mouse( ui, viewport_index,
                               volume_index, voxel_position ) )
    {
        if( *volume_index < N_VOLUMES )
        {
            value = IF_get_voxel_value( *volume_index,
                                        voxel_position[X],
                                        voxel_position[Y],
                                        voxel_position[Z] );
            set_text_entry_real_value(
                     get_volume_readout_widget(ui,*volume_index),
                     Readout_values_format, value );
        }
        else
        {
            value = IF_get_voxel_value( 0,
                                        voxel_position[X],
                                        voxel_position[Y],
                                        voxel_position[Z] );
            set_text_entry_real_value( get_merged_readout_widget(ui,0),
                                       Readout_values_format, value );

            value = IF_get_voxel_value( 1,
                                        voxel_position[X],
                                        voxel_position[Y],
                                        voxel_position[Z] );
            set_text_entry_real_value( get_merged_readout_widget(ui,1),
                                       Readout_values_format, value );
        }
    }
    else
        *volume_index = NO_VOLUME;
}

private  void  update_readout(
    UI_struct  *ui,
    int        x_mouse,
    int        y_mouse )
{
    int       prev_volume;

    prev_volume = ui->which_volume_readout;

    modify_readout( ui, x_mouse, y_mouse, &ui->which_volume_readout );

    if( prev_volume != NO_VOLUME && prev_volume != ui->which_volume_readout )
        set_readout_activity( ui, prev_volume, OFF );

    if( ui->which_volume_readout != NO_VOLUME &&
        ui->which_volume_readout != prev_volume )
        set_readout_activity( ui, ui->which_volume_readout, ON );
}

private  void  check_readout(
    UI_struct  *ui )
{
    int   x_mouse, y_mouse;

    if( !G_get_mouse_position( ui->graphics_window.window, &x_mouse, &y_mouse ))
    {
        x_mouse = -1;
        y_mouse = -1;
    }

    if( IF_slices_to_be_updated( ui->graphics_window.current_buffer ) ||
        x_mouse != ui->x_mouse_readout || y_mouse != ui->y_mouse_readout )
    {
        update_readout( ui, x_mouse, y_mouse );
        ui->x_mouse_readout = x_mouse;
        ui->y_mouse_readout = y_mouse;
    }
}

private  DEFINE_EVENT_FUNCTION( check_update_readout )  /* ARGSUSED */
{
    check_readout( get_ui_struct() );
}

public  void  install_readout_update_event(
    UI_struct   *ui )
{
    add_global_event_callback( NO_EVENT, check_update_readout,
                               ANY_MODIFIER, (void *) NULL );

    ui->x_mouse_readout = -1;
    ui->y_mouse_readout = -1;
    ui->which_volume_readout = NO_VOLUME;
}
