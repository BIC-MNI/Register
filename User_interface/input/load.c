/**
 * \file User_interface/input/load.c
 * \brief Start and finish loading a volume.
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

static  void  volume_has_been_loaded( UI_struct  *, load_struct * );

#define  TIME_SLICE   0.1    /* input volume for at least tenth of second */

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( more_input )
{
    VIO_BOOL       done_loading;
    load_struct   *data;
    VIO_Real          fraction_done;

    data = (load_struct *) callback_data;

    done_loading = IF_load_more_of_volume( data->volume_index, TIME_SLICE,
                                           &fraction_done );

    if( done_loading )
    {
        volume_has_been_loaded( get_ui_struct(), data );
    }
    else
        set_load_popup_meter( data, fraction_done );
}

static  void  delete_popup_interaction(
    load_struct    *data )
{
    set_load_activity( get_ui_struct(), data->volume_index, TRUE );

    remove_global_event_callback( NO_EVENT, more_input, (void *) data );

    delete_load_popup( data );

    delete_string( data->filename );

    FREE( data );
}

  VIO_Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    VIO_STR     filename,
    VIO_BOOL    this_is_resampled_volume )
{
    VIO_Status        status;
    load_struct   *data;
    int           x_window, y_window;
    int           x_min, y_min, x_max, y_max;

    ALLOC( data, 1 );

    if (volume >= ui_info->n_volumes_loaded)
      ui_info->n_volumes_loaded = volume + 1;

    data->volume_index = volume;
    data->filename = create_string( filename );
    data->this_is_resampled_volume = this_is_resampled_volume;

    status = IF_start_loading_volume( volume, filename );

    if( status == VIO_OK )
    {
        set_load_activity( ui_info, volume, FALSE );

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

static  void  volume_has_been_loaded(
    UI_struct      *ui_info,
    load_struct    *data )
{
    VIO_Real   min_value, max_value;
    int i;

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

    set_volume_widgets_activity( ui_info, data->volume_index, TRUE,
                                 IF_is_an_rgb_volume(data->volume_index),
                                 IF_volume_has_time_axis(data->volume_index));

    set_resampled_label_activity( ui_info, IF_is_resampled_volume_loaded() );

    update_position_counters( ui_info, data->volume_index );

    for (i = 0; i < ui_info->n_volumes_loaded; i++) 
    {
      if ( !IF_volume_is_loaded( i ) )
      {
        break;
      }
    }

    if ( i == ui_info->n_volumes_loaded && i >= MIN_MERGED_VOLUMES )
      set_merged_activity( ui_info, TRUE );

    for (i = 0; i < ui_info->n_volumes_loaded; i++)
    {
        if ( i == data->volume_index )
            continue;
        if( IF_volume_is_loaded( i ) )
        {
            update_other_volume_positions( ui_info, i );
        }
    }

    if( data->volume_index < ui_info->n_volumes_loaded )
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

  void  cancel_loading(
    load_struct    *data )
{
    IF_cancel_loading_volume( data->volume_index );

    delete_popup_interaction( data );
}
