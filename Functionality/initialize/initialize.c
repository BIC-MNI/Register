#include  <register.h>

#define  GLOBALS_LOOKUP_NAME  functional_globals
#include  <globals.h>

#define   REGISTER_GLOBALS_FILENAME   "register.globals"

private   main_struct      main_info;

public  main_struct  *get_main_struct()
{
    return( &main_info );
}

public  Status   initialize_register( window_struct  *window )
{
    Status          status;
    int             volume, view;
    Bitplane_types  bitplane;
    STRING          home_filename;

    (void) sprintf( home_filename, "$HOME/%s", REGISTER_GLOBALS_FILENAME );

    if( file_exists( home_filename ) )
    {
        status = input_globals_file( SIZEOF_STATIC_ARRAY(functional_globals),
                                     functional_globals, home_filename );
    }

    if( file_exists( REGISTER_GLOBALS_FILENAME ) )
    {
        status = input_globals_file( SIZEOF_STATIC_ARRAY(functional_globals),
                          functional_globals, REGISTER_GLOBALS_FILENAME );
    }

    if( Disable_alloc_checking )
        set_alloc_checking( OFF );

/*
    set_alloc_debug( Alloc_debugging );
*/

    G_set_overlay_colour_map( window, 1, Overlay_colour_1 );
    G_set_overlay_colour_map( window, 2, Overlay_colour_2 );
    G_set_overlay_colour_map( window, 3, Overlay_colour_3 );

    main_info.window = window;

    initialize_graphics_struct( &main_info.graphics );

    initialize_slices( &main_info );

    initialize_tag_points( &main_info );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
            {
                set_update_slice_viewport_flag( &main_info, volume,
                                                view, bitplane );
            }
        }
    }

    main_info.resampled_file_loaded = FALSE;
    create_linear_transform( &main_info.resampling_transform,
                             (Transform *) NULL );

    return( status );
}

public  void   terminate_register()
{
    terminate_slices( &main_info );

    /* --- since delete_tag_points() makes calls to remove objects from the
           graphics struct, it must be called before delete_graphics_struct() */

    delete_tag_points( &main_info );
    delete_graphics_struct( &main_info.graphics );
}
