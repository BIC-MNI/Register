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

    main_info.window = window;

    initialize_slices( &main_info );

    initialize_graphics_struct( &main_info.graphics );

    for_less( volume, 0, N_VOLUMES )
    {
        for_less( view, 0, N_VIEWS )
        {
            set_graphics_viewport_background( &main_info.graphics,
                                get_slice_viewport_index(volume,view),
                                Slice_background_colour, 0 );

            object = create_object( PIXELS );

            add_object_to_viewport( &main_info.graphics,
                                    get_slice_viewport_index(volume,view),
                                    NORMAL_PLANES, object );

            main_info.trislice[volume].slices[view].pixels =
                             (pixels_struct *) get_object_pointer( object );

            set_viewport_objects_visibility( &main_info.graphics,
                                    get_slice_viewport_index(volume,view),
                                    OFF );
        }
    }

    return( status );
}
