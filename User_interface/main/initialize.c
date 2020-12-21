/**
 * \file User_interface/main/initialize.c
 * \brief Basic user interface initialization and cleanup.
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

int MERGED_VOLUME_INDEX = 0;

/**
 * Perform basic initialization for the user interface.
 *
 * Sets up the global UI_struct, creates the main window.
 * \param ui A pointer to the global UI_struct.
 * \param executable_name The path to the executable, generally from argv[0].
 * \param n_volumes Number of volumes loaded.
 * \returns VIO_OK on successful completion.
 */
  VIO_Status   initialize_user_interface(
    UI_struct     *ui,
    const VIO_STR executable_name,
    int           n_volumes )
{
    VIO_Status      status;

    ui->volumes_synced = Initial_volumes_synced;
    ui->original_filename_volume_2 = create_string( NULL );
    ui->resampled_filename = create_string( NULL );
    /* Enforce a minimum of two loadable volumes. */
    if (n_volumes < 2) {
      n_volumes = 2;
    }
    ui->n_volumes_loaded = n_volumes;
    ui->n_volumes_displayed = n_volumes + 1;
    MERGED_VOLUME_INDEX = ui->n_volumes_displayed - 1;
    ui->prev_divider_x = 0;
    ui->prev_divider_y = 0;
    ui->divider_vp_index = -1;

    create_linear_transform( &ui->resampling_transform, NULL );

    initialize_ui_colours();

    initialize_print_popup();

    if ( Initial_window_x_size <= 0 )
    {
        Initial_window_x_size = 100 + ui->n_volumes_displayed * 300;
    }

    status = G_create_window( Main_window_name, -1, -1,
                              Initial_window_x_size,
                              Initial_window_y_size,
                              Initial_rgb_state,
                              Initial_double_buffer_state,
                              FALSE, 1, &ui->graphics_window.window );

    set_window_event_callbacks( &ui->graphics_window );

    IF_initialize_register( ui->graphics_window.window, executable_name,
                            n_volumes);

    IF_set_interpolation_flag( Initial_interpolation_state );

    G_set_zbuffer_state( ui->graphics_window.window, FALSE );
    G_set_lighting_state( ui->graphics_window.window, FALSE );
    G_set_transparency_state( ui->graphics_window.window, FALSE );

    set_start_colour_table( ui );

    G_set_automatic_clear_state( ui->graphics_window.window, FALSE );

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

    colour_map_state_has_changed( ui );

    install_tag_events( ui );

    return( status );
}

/**
 * Clean up after the user interface.
 *
 * Disposes of memory and other resources.
 * \param ui A pointer to the global UI_struct.
 */
  void   terminate_user_interface(
    UI_struct  *ui )
{
    int   volume;

    delete_string( ui->original_filename_volume_2 );
    delete_string( ui->resampled_filename );

    for_less( volume, 0, ui->n_volumes_loaded )
    {
        if( IF_volume_is_loaded(volume) )
            IF_delete_volume( volume );
    }

    IF_terminate_register();

    delete_UI_widgets( ui );

    delete_all_graphics_windows();

    delete_global_events();

    delete_general_transform( &ui->resampling_transform );

    /* Don't call G_terminate() */
}
