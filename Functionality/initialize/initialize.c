#include  <def_register.h>

public  void  record_window( graphics_window_struct  *graphics_window )
{
    add_graphics_window( graphics_window );
    initialize_event_rectangles( &graphics_window->rectangles_actions );
}

#define   REGISTER_GLOBALS_FILENAME   "register.globals"

public  Status   initialize_register( main_struct  *main )
{
    Status     status;

    if( file_exists( REGISTER_GLOBALS_FILENAME ) )
    {
        status = input_globals_file( REGISTER_GLOBALS_FILENAME );
    }

    main->rgb_mode = Initial_rgb_state;
    main->double_buffer_mode = Initial_double_buffer_state;
    main->current_buffer = 0;
    main->interpolation_flag = Initial_interpolation_state;

    status = G_create_window( Main_window_name, -1, -1, 900, 900,
                              &main->graphics_window.window );

    record_window( &main->graphics_window );

    G_set_colour_map_state( main->graphics_window.window, !main->rgb_mode );

    if( main->rgb_mode )
    {
        G_set_background_colour( main->graphics_window.window,
                                 Default_background_colour );
    }

    G_set_double_buffer_state( main->graphics_window.window,
                               main->double_buffer_mode );

    G_set_automatic_clear_state( main->graphics_window.window, FALSE );

    initialize_global_events();
    install_window_events();

    initialize_layout( main );

    initialize_slices( main );

    install_quit_events( &main->graphics_window.rectangles_actions );

    set_all_update_flags( main );

    return( status );
}
