#include  <def_user_interface.h>

typedef  struct
{
    int                       volume_index;
    volume_input_struct       input;
    volume_struct             volume;
    load_meter_popup_struct   popup;
}
load_struct;

private  void  volume_has_been_loaded( UI_struct  *, load_struct * );

private  DEFINE_EVENT_FUNCTION( more_input )
{
    load_struct   *data;
    Real          fraction_done;

    data = (load_struct *) callback_data;

    if( input_more_of_volume( &data->volume, &data->input, &fraction_done ) )
    {
        set_load_popup_meter( &data->popup, fraction_done );
    }
    else
    {
        volume_has_been_loaded( get_ui_struct(), data );
        remove_global_event_callback( NO_EVENT, more_input, callback_data );
    }
}

public  Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    char       filename[] )
{
    Status        status;
    load_struct   *data;
    int           x_window, y_window;
    int           x_min, y_min, x_max, y_max;

    ALLOC( data, 1 );

    data->volume_index = volume;

    status = start_volume_input( filename, &data->volume, &data->input );

    if( status == OK )
    {
        set_load_activity( ui_info, volume, OFF );

        if( status == OK )
        {
            add_global_event_callback( NO_EVENT, more_input, (void *) data );
        }

        get_graphics_viewport( &ui_info->graphics_window.graphics, 
                               Volume_1_menu_viewport + volume,
                               &x_min, &x_max, &y_min, &y_max );

        G_get_window_position( ui_info->graphics_window.window,
                               &x_window, &y_window );

        initialize_load_popup( &data->popup, x_window + x_min, y_window + y_min,
                               filename );
    }
    else
        FREE( data );

    return( status );
}

private  void  volume_has_been_loaded(
    UI_struct      *ui_info,
    load_struct    *data )
{
    set_volume_widgets_activity( ui_info, data->volume_index, ON );

    if( !IF_volume_is_loaded( data->volume_index ) )
    {
        install_slice_events( &ui_info->graphics_window.event_viewports,
                              data->volume_index );
    }

    IF_set_volume( data->volume_index, &data->volume );

    delete_load_popup( &data->popup );

    update_position_counters( ui_info, data->volume_index );

    FREE( data );
}
