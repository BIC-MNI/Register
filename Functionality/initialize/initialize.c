#include  <def_register.h>

#define  GLOBALS_LOOKUP_NAME  functional_globals
#include  <def_globals.h>

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

    if( file_exists( REGISTER_GLOBALS_FILENAME ) )
    {
        status = input_globals_file( SIZEOF_STATIC_ARRAY(functional_globals),
                          functional_globals, REGISTER_GLOBALS_FILENAME );
    }

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

    return( status );
}

public  void   terminate_register()
{
    terminate_slices( &main_info );
    delete_graphics_struct( &main_info.graphics );
    delete_tag_points( &main_info );
}
