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

#include  <user_interface.h>

  void  update_volume_readout(
    UI_struct  *ui,
    int        volume_index )
{
    VIO_Real   value;
    VIO_Real   voxel[VIO_N_DIMENSIONS];

    if( volume_index < MERGED_VOLUME_INDEX )
    {
        IF_get_volume_voxel_position( volume_index, voxel );
        value = IF_get_voxel_value( volume_index, voxel[VIO_X], voxel[VIO_Y], voxel[VIO_Z]);
        set_text_entry_real_value( get_volume_readout_widget(ui,volume_index),
                                   Readout_values_format, value );
    }
    else
    {
        IF_get_volume_voxel_position( MERGED_VOLUME_INDEX, voxel );
        value = IF_get_voxel_value( 0, voxel[VIO_X], voxel[VIO_Y], voxel[VIO_Z] );
        set_text_entry_real_value( get_merged_readout_widget(ui,0),
                                   Readout_values_format, value );

        IF_get_volume_voxel_position( MERGED_VOLUME_INDEX+1, voxel );
        value = IF_get_voxel_value( 1, voxel[VIO_X], voxel[VIO_Y], voxel[VIO_Z] );
        set_text_entry_real_value( get_merged_readout_widget(ui,1),
                                   Readout_values_format, value );
    }
}

#ifdef NOT_NEEDED
static  const  NO_VOLUME = -1;

static  void  set_readout_activity(
    UI_struct  *ui,
    int        which_volume,
    VIO_BOOL    activity )
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

static  void  modify_readout(
    UI_struct  *ui,
    int        x_mouse,
    int        y_mouse,
    int        *volume_index )
{
    int    viewport_index;
    VIO_Real   value;
    VIO_Real   voxel_position[VIO_N_DIMENSIONS];

    if( find_viewport_containing_mouse( &ui->graphics_window.event_viewports,
                                        x_mouse, y_mouse, &viewport_index ) &&
        get_voxel_under_mouse( ui, viewport_index,
                               volume_index, voxel_position ) )
    {
        set_readout( ui, *volume_index, voxel_position );
    }
    else
        *volume_index = NO_VOLUME;
}

static  void  update_readout(
    UI_struct  *ui,
    int        x_mouse,
    int        y_mouse )
{
    int       prev_volume;

    prev_volume = ui->which_volume_readout;

    modify_readout( ui, x_mouse, y_mouse, &ui->which_volume_readout );

    if( prev_volume != NO_VOLUME && prev_volume != ui->which_volume_readout )
        set_readout_activity( ui, prev_volume, FALSE );

    if( ui->which_volume_readout != NO_VOLUME &&
        ui->which_volume_readout != prev_volume )
        set_readout_activity( ui, ui->which_volume_readout, TRUE );
}

static  void  check_readout(
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

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( check_update_readout )
{
    check_readout( get_ui_struct() );
}

  void  install_readout_update_event(
    UI_struct   *ui )
{
    add_global_event_callback( NO_EVENT, check_update_readout,
                               ANY_MODIFIER, (void *) NULL );

    ui->x_mouse_readout = -1;
    ui->y_mouse_readout = -1;
    ui->which_volume_readout = NO_VOLUME;
}
#endif
