#include  <user_interface.h>

private  void  volume_has_been_loaded( UI_struct  *, load_struct * );

private  DEFINE_EVENT_FUNCTION( more_input )
{
    load_struct   *data;
    Real          fraction_done;

    data = (load_struct *) callback_data;

    if( input_more_of_volume( data->volume, &data->input, &fraction_done ) )
    {
        set_load_popup_meter( data, fraction_done );
    }
    else
    {
        /*  --- the following printf line seems to fix a memory leak
            error that occurs when reading an rgb MINC file */
            
        printf( "" );

        volume_has_been_loaded( get_ui_struct(), data );
    }
}

private  void  delete_popup_interaction(
    load_struct    *data )
{
    set_load_activity( get_ui_struct(), data->volume_index, ON );

    remove_global_event_callback( NO_EVENT, more_input, (void *) data );

    delete_volume_input( &data->input );

    delete_load_popup( data );

    FREE( data );
}

public  Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    char       filename[],
    BOOLEAN    this_is_resampled_volume )
{
    Status        status;
    load_struct   *data;
    int           x_window, y_window;
    int           x_min, y_min, x_max, y_max;

    ALLOC( data, 1 );

    data->volume_index = volume;
    (void) strcpy( data->filename, filename );
    data->this_is_resampled_volume = this_is_resampled_volume;

    status = start_volume_input( filename, 3, XYZ_dimension_names,
                                 NC_BYTE, FALSE, 0.0, 0.0, TRUE,
                                 &data->volume, (minc_input_options *) NULL,
                                 &data->input );

    if( status == OK )
    {
        set_load_activity( ui_info, volume, OFF );

        add_global_event_callback( NO_EVENT, more_input, ANY_MODIFIER,
                                   (void *) data );

        get_graphics_viewport( &ui_info->graphics_window.graphics, 
                               get_volume_menu_viewport_index(volume),
                               &x_min, &x_max, &y_min, &y_max );

        G_get_window_position( ui_info->graphics_window.window,
                               &x_window, &y_window );

        initialize_load_popup( data, x_window + x_min, y_window + y_min,
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
    Real   min_value, max_value;

    set_volume_widgets_activity( ui_info, data->volume_index, ON );

    if( !IF_volume_is_loaded( data->volume_index ) )
    {
        install_slice_events( &ui_info->graphics_window.event_viewports,
                              data->volume_index );
    }

    if( data->this_is_resampled_volume )
    {
        IF_set_resampled_volume( data->volume, data->filename,
                                 ui_info->original_filename_volume_2,
                                 &ui_info->resampling_transform );
    }
    else
    {
        IF_set_volume( data->volume_index, data->volume, data->filename );
    }

    set_resampled_label_activity( ui_info, IF_is_resampled_volume_loaded() );

    update_position_counters( ui_info, data->volume_index );

    if( IF_volume_is_loaded( 0 ) &&
        IF_volume_is_loaded( 1 ) )
    {
        set_merged_activity( ui_info, ON );
    }

    if( IF_volume_is_loaded( 1 - data->volume_index ) )
    {
        update_other_volume_positions( ui_info, 1 - data->volume_index );
    }

    if( data->volume_index < N_VOLUMES )
    {
        IF_get_volume_value_range( data->volume_index, &min_value, &max_value );
        set_colour_bar_limits( ui_info, data->volume_index,
                               min_value, max_value );
        IF_get_colour_coding_limits( data->volume_index, &min_value,
                                     &max_value );
        set_colour_bar_values( ui_info, data->volume_index,
                               min_value, max_value );
    }

    delete_popup_interaction( data );
}

public  void  cancel_loading(
    load_struct    *data )
{
    delete_volume( data->volume );

    delete_popup_interaction( data );
}
