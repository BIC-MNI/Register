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
 * Scale the fixed-pixel UI-geometry globals by an integer factor.
 *
 * bicgl lays out all widgets in framebuffer (physical) pixels, and on a
 * HiDPI/Retina display it DPI-scales its bitmap fonts by the same factor
 * (see G_get_window_content_scale).  The pixel-sized widget-geometry globals
 * below are interpreted directly in that framebuffer space, so to stay
 * consistent with the scaled fonts they must be scaled by the same factor —
 * otherwise text overflows still-tiny buttons.  On a normal display the
 * factor is 1 and this is a no-op (so Linux behaviour is unchanged).
 *
 * Only *internal* layout dimensions are scaled here.  Window-creation sizes
 * (Initial_window_*, and the *_popup / *_selection / Quit / Delete_tags
 * window sizes) are logical points passed to G_create_window and already
 * double via the framebuffer, so they are deliberately left alone; likewise
 * fractions, counts, font sizes, colours, formats, and timings.
 */
static void scale_ui_geometry( int scale )
{
    if( scale <= 1 )
        return;

    Default_main_menu_width      *= scale;
    Default_tag_panel_height     *= scale;
    Default_volume_panel_height  *= scale;
    Default_divider_width        *= scale;

    Main_menu_x_offset           *= scale;
    Main_menu_y_offset           *= scale;
    Interface_x_spacing          *= scale;
    Interface_y_spacing          *= scale;
    Volume_x_spacing             *= scale;
    Volume_y_spacing             *= scale;

    Button_width                 *= scale;
    Button_height                *= scale;
    Text_entry_height            *= scale;
    Text_entry_cursor_size       *= scale;

    Tags_filename_x_offset       *= scale;
    Tags_filename_width          *= scale;

    Volume_menu_x_offset         *= scale;
    Volume_menu_y_offset         *= scale;
    Volume_button_width          *= scale;
    Volume_button_height         *= scale;

    Filter_button_width          *= scale;
    Filter_button_height         *= scale;
    Filter_button_spacing        *= scale;
    Filter_y_spacing             *= scale;
    Full_width_label_width       *= scale;
    Full_width_text_width        *= scale;
    Filter_view_label_width      *= scale;

    Load_filename_width          *= scale;
    Load_meter_x_size            *= scale;
    Load_meter_y_size            *= scale;
    Resample_meter_x_size        *= scale;
    Resample_meter_y_size        *= scale;

    Colour_bar_button_width      *= scale;
    Colour_bar_button_spacing    *= scale;
    Colour_bar_slider_width      *= scale;
    Colour_bar_slider_height     *= scale;
    Opacity_slider_width         *= scale;
    Opacity_slider_height        *= scale;

    Position_label_width         *= scale;
    Position_values_width        *= scale;
    Position_values_separation   *= scale;

    Slider_text_entry_x_offset   *= scale;
    Slider_text_entry_y_offset   *= scale;
    Slider_height                *= scale;
    Slider_text_width            *= scale;
    Slider_text_height           *= scale;
    Slider_text_peg_width        *= scale;

    Avg_rms_label_width          *= scale;
    Avg_rms_number_width         *= scale;
    Rms_button_width             *= scale;
    Rms_number_width             *= scale;
    Value_readout_width          *= scale;

    Tag_world_button_width       *= scale;
    Tag_number_button_width      *= scale;
    Tag_position_label_width     *= scale;
    Tag_position_width           *= scale;
    Tag_point_height             *= scale;
    Tag_name_width               *= scale;
    Tag_activity_width           *= scale;
    Advance_tags_button_width    *= scale;
    Tags_x_spacing               *= scale;
    Tags_y_spacing               *= scale;
    Tag_radius_pixels            *= scale;

    Transform_button_width       *= scale;

    Message_x_offset             *= scale;
    Message_y_offset             *= scale;
    Message_text_y_offset        *= scale;
    Message_ok_button_width      *= scale;
    Message_ok_button_height     *= scale;

    Slice_cursor_offset          *= scale;
    Slice_cursor_size            *= scale;
}

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

    /* On a HiDPI/Retina display bicgl works in framebuffer pixels and scales
     * its fonts up; scale the fixed-pixel widget geometry to match, before any
     * layout is computed.  No-op (scale == 1) on normal displays and X11. */
    scale_ui_geometry( G_get_window_content_scale( ui->graphics_window.window ) );

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
