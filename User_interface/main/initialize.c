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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/main/initialize.c,v 1.17 1996-12-09 20:21:54 david Exp $";
#endif

#include  <user_interface.h>

public  Status   initialize_user_interface(
    UI_struct  *ui,
    STRING     executable_name )
{
    Status      status;

    ui->volumes_synced = Initial_volumes_synced;
    ui->original_filename_volume_2 = create_string( NULL );
    ui->resampled_filename = create_string( NULL );

    create_linear_transform( &ui->resampling_transform, (Transform *) NULL );

    initialize_ui_colours();

    initialize_print_popup();

    status = G_create_window( Main_window_name, -1, -1,
                              Initial_window_x_size,
                              Initial_window_y_size,
                              !Initial_rgb_state,
                              Initial_double_buffer_state,
                              FALSE, 2, &ui->graphics_window.window );

    if( status != OK ||
        (!Initial_rgb_state &&
         G_get_n_colour_map_entries(ui->graphics_window.window) <
         Min_colour_map_size) )
    {
        print_error( "Not enough colours, reopening in RGB mode.\n" );

        (void) G_delete_window( ui->graphics_window.window );

        status = G_create_window( Main_window_name, -1, -1,
                                  Initial_window_x_size,
                                  Initial_window_y_size,
                                  FALSE,
                                  Initial_double_buffer_state,
                                  FALSE, 2, &ui->graphics_window.window );
    }

    IF_initialize_register( ui->graphics_window.window, executable_name );

    IF_set_interpolation_flag( Initial_interpolation_state );

    G_set_zbuffer_state( ui->graphics_window.window, OFF );
    G_set_lighting_state( ui->graphics_window.window, OFF );
    G_set_transparency_state( ui->graphics_window.window, OFF );

    set_start_colour_table( ui );

    G_set_automatic_clear_state( ui->graphics_window.window, OFF );

    initialize_graphics_struct( &ui->graphics_window.graphics );
    initialize_event_viewports( &ui->graphics_window.event_viewports );
    ui->graphics_window.current_buffer = 0;

    record_graphics_window( &ui->graphics_window );

    initialize_layout( ui );

    set_clear_and_update_flags( ui );

    install_window_events( ui );

    initialize_UI_widgets( ui );

    IF_set_volume_colour_coding_type( 0,
                  (Colour_coding_types) Volume_1_default_colour_coding );
    IF_set_volume_colour_coding_type( 1,
                  (Colour_coding_types) Volume_2_default_colour_coding );
    IF_set_volume_colour_coding_type( MERGED_VOLUME_INDEX,
                  (Colour_coding_types) Volume_1_default_colour_coding );
    IF_set_volume_colour_coding_type( MERGED_VOLUME_INDEX+1,
                  (Colour_coding_types) Volume_2_default_colour_coding );

    colour_map_state_has_changed( ui );

    install_tag_events( ui );

    return( status );
}

public  void   terminate_user_interface(
    UI_struct  *ui )
{
    int   volume;

    delete_string( ui->original_filename_volume_2 );
    delete_string( ui->resampled_filename );

    for_less( volume, 0, N_VOLUMES )
    {
        if( IF_volume_is_loaded(volume) )
            IF_delete_volume( volume );
    }

    IF_terminate_register();

    delete_UI_widgets( ui );

    delete_all_graphics_windows();

    delete_global_events();

    delete_general_transform( &ui->resampling_transform );

    G_terminate();
}
