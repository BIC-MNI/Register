#include  <def_user_interface.h>

private  void  delete_popup_interaction(
    resample_volume_struct    *data );

private  void  done_resampling(
    UI_struct               *ui,
    resample_volume_struct  *data )
{
    Status  status;

    status = output_volume( get_ui_struct()->resampled_filename,
                            &data->resampled_volume );

    if( status == OK )
    {
        (void) initialize_loading_volume( get_ui_struct(),
                                          RESAMPLED_VOLUME_INDEX,
                                          get_ui_struct()->resampled_filename,
                                          TRUE );
    }

    delete_popup_interaction( data );
}

private  DEFINE_EVENT_FUNCTION( more_resampling )   /* ARGSUSED */
{
    resample_volume_struct   *data;
    Real                     fraction_done;

    data = (resample_volume_struct *) callback_data;

    if( do_more_resampling( &data->resample, Seconds_per_resample,
                            &fraction_done ) )
    {
        set_resample_popup_meter( data, fraction_done );
    }
    else
    {
        done_resampling( get_ui_struct(), data );
    }
}

private  void  delete_popup_interaction(
    resample_volume_struct    *data )
{
    set_resample_button_activity( get_ui_struct(), ON );

    remove_global_event_callback( NO_EVENT, more_resampling, (void *) data );

    delete_resample_popup( data );

    delete_volume( &data->resampled_volume );

    FREE( data );
}

public  void  cancel_resampling(
    resample_volume_struct *data )
{
    delete_popup_interaction( data );
}

public  void  start_resampling(
    UI_struct   *ui,
    Transform   *resampling_transform,
    char        resampled_filename[] )
{
    int                     x_min, x_max, y_min, y_max, x_window, y_window;
    resample_volume_struct  *data;
    String                  output_filename;

    ALLOC( data, 1 );

    (void) strcpy( ui->original_filename_volume_2,
                   IF_get_volume_filename(RESAMPLED_VOLUME_INDEX) );

    expand_filename( resampled_filename, output_filename );
    (void) strcpy( ui->resampled_filename, output_filename );

    ui->resampling_transform = *resampling_transform;

    data->resampled_volume = *IF_get_volume( 1 - RESAMPLED_VOLUME_INDEX );

    alloc_volume( &data->resampled_volume );

    initialize_resample_volume( &data->resample,
                                IF_get_volume( RESAMPLED_VOLUME_INDEX ),
                                resampling_transform, &data->resampled_volume);

    set_resample_button_activity( ui, FALSE );

    add_global_event_callback( NO_EVENT, more_resampling, ANY_MODIFIER,
                               (void *) data );

    get_graphics_viewport( &ui->graphics_window.graphics,
                           get_volume_menu_viewport_index(
                                      RESAMPLED_VOLUME_INDEX),
                           &x_min, &x_max, &y_min, &y_max );

    G_get_window_position( ui->graphics_window.window,
                           &x_window, &y_window );

    initialize_resample_popup( data, x_window + x_min, y_window + y_min,
                               resampled_filename );
}
