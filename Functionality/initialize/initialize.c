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
    int            volume, view;
    object_struct  *object;
    Status         status;

    if( file_exists( REGISTER_GLOBALS_FILENAME ) )
    {
        status = input_globals_file( SIZEOF_STATIC_ARRAY(functional_globals),
                          functional_globals, REGISTER_GLOBALS_FILENAME );
    }

    G_set_overlay_colour_map( window, 1, Overlay_colour_1 );
    G_set_overlay_colour_map( window, 2, Overlay_colour_2 );
    G_set_overlay_colour_map( window, 3, Overlay_colour_3 );

    main_info.window = window;

    initialize_slices( &main_info );

    initialize_graphics_struct( &main_info.graphics );

    for_less( volume, 0, N_VOLUMES )
    {
        initialize_colour_coding( &main_info.trislice[volume].colour_coding,
                                  GRAY_SCALE,
                                  -0.5, (Real) N_VOXEL_VALUES - 0.5 );

        for_less( view, 0, N_VIEWS )
        {
            set_graphics_viewport_background( &main_info.graphics,
                                get_slice_viewport_index(volume,view),
                                Slice_background_colour, 0 );

            /* create pixels */

            object = create_object( PIXELS );

            add_object_to_viewport( &main_info.graphics,
                                    get_slice_viewport_index(volume,view),
                                    NORMAL_PLANES, object );

            main_info.trislice[volume].slices[view].pixels =
                             (pixels_struct *) get_object_pointer( object );

            /* create cursor */

            main_info.trislice[volume].slices[view].cursor_lines =
                          create_cursor( &main_info, volume, view );

            set_viewport_objects_visibility( &main_info.graphics,
                                    get_slice_viewport_index(volume,view),
                                    OFF );
        }
    }

    return( status );
}

public  void   terminate_register()
{
    terminate_slices( &main_info );
    delete_graphics_struct( &main_info.graphics );
}
