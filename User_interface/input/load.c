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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/input/load.c,v 1.22 1996-12-09 20:21:51 david Exp $";
#endif

#include  <user_interface.h>

private  void  volume_has_been_loaded( UI_struct  *, load_struct * );

#define  TIME_SLICE   0.1    /* input volume for at least tenth of second */

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( more_input )
{
    BOOLEAN       done_loading;
    load_struct   *data;
    Real          fraction_done;

    data = (load_struct *) callback_data;

    done_loading = IF_load_more_of_volume( data->volume_index, TIME_SLICE,
                                           &fraction_done );

    if( done_loading )
    {
#ifdef  DISABLED_TO_SEE_IF_IT_IS_STILL_NEEDED
        /*  --- the following printf line seems to fix a memory leak
            error that occurs when reading an rgb MINC file */
            
        (void) printf( "" );
#endif

        volume_has_been_loaded( get_ui_struct(), data );
    }
    else
        set_load_popup_meter( data, fraction_done );
}

private  void  delete_popup_interaction(
    load_struct    *data )
{
    set_load_activity( get_ui_struct(), data->volume_index, ON );

    remove_global_event_callback( NO_EVENT, more_input, (void *) data );

    delete_load_popup( data );

    delete_string( data->filename );

    FREE( data );
}

public  Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    STRING     filename,
    BOOLEAN    this_is_resampled_volume )
{
    Status        status;
    load_struct   *data;
    int           x_window, y_window;
    int           x_min, y_min, x_max, y_max;

    ALLOC( data, 1 );

    data->volume_index = volume;
    data->filename = create_string( filename );
    data->this_is_resampled_volume = this_is_resampled_volume;

    status = IF_start_loading_volume( volume, filename );

    if( status == OK )
    {
        set_load_activity( ui_info, volume, OFF );

        add_global_event_callback( NO_EVENT, more_input, ANY_MODIFIER,
                                   (void *) data );

        get_graphics_viewport( &ui_info->graphics_window.graphics, 
                               (int) get_volume_menu_viewport_index(volume),
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

    if( !IF_volume_is_loaded( data->volume_index ) )
    {
        install_slice_events( &ui_info->graphics_window.event_viewports,
                              data->volume_index );
    }

    if( data->this_is_resampled_volume )
    {
        IF_set_resampled_volume( data->volume_index, data->filename,
                                 ui_info->original_filename_volume_2,
                                 &ui_info->resampling_transform );
    }
    else
    {
        IF_set_volume( data->volume_index, data->filename );
    }

    set_volume_widgets_activity( ui_info, data->volume_index, ON,
                                 IF_is_an_rgb_volume(data->volume_index) );

    update_colour_map_toggle_activity( ui_info );

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

    update_colour_map_toggle_activity( ui_info );

    delete_popup_interaction( data );
}

public  void  cancel_loading(
    load_struct    *data )
{
    IF_cancel_loading_volume( data->volume_index );

    delete_popup_interaction( data );
}
