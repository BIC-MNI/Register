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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/initialize/initialize.c,v 1.14 1995-10-02 18:34:39 david Exp $";
#endif

#include  <register.h>

#define  GLOBALS_LOOKUP_NAME  functional_globals
#include  <globals.h>

#define   REGISTER_GLOBALS_FILENAME   "register.globals"

private   main_struct      main_info;

private  void    initialize_global_colours();

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

    initialize_global_colours();

    home_filename = get_absolute_filename( REGISTER_GLOBALS_FILENAME, "$HOME" );

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

    delete_string( home_filename );

    if( Disable_alloc_checking )
        set_alloc_checking( OFF );

/*
    set_alloc_debug( Alloc_debugging );
*/

    if( G_has_overlay_planes() && Use_overlay_planes )
    {
        G_set_overlay_colour_map( window, 1, Overlay_colour_1 );
        G_set_overlay_colour_map( window, 2, Overlay_colour_2 );
        G_set_overlay_colour_map( window, 3, Overlay_colour_3 );
    }

    main_info.window = window;

    initialize_graphics_struct( &main_info.graphics );

    initialize_slices( &main_info );

    initialize_tag_points( &main_info );

    main_info.original_volume_filename = create_string( NULL );

    for_less( volume, 0, N_VOLUMES )
        main_info.trislice[volume].filename = create_string( NULL );

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
    main_info.degrees_continuity = -1;
    create_linear_transform( &main_info.resampling_transform,
                             (Transform *) NULL );

    main_info.render_storage = initialize_render_storage();

    return( status );
}

public  void   terminate_register()
{
    int   volume;

    delete_string( main_info.original_volume_filename );

    for_less( volume, 0, N_VOLUMES )
        delete_string( main_info.trislice[volume].filename );

    terminate_slices( &main_info );

    /* --- since delete_tag_points() makes calls to remove objects from the
           graphics struct, it must be called before delete_graphics_struct() */

    delete_tag_points( &main_info );
    delete_graphics_struct( &main_info.graphics );

    delete_render_storage( main_info.render_storage );

    delete_general_transform( &main_info.resampling_transform );
}

private  void    initialize_global_colours()
{
    Overlay_colour_1 = RED;
    Overlay_colour_2 = GREEN;
    Overlay_colour_3 = BLUE;
    Slice_background_colour = DARK_SLATE_GREY;
    Cursor_inside_colour = RED;
    Cursor_outside_colour = BLUE;
    Tag_inside_colour = CYAN;
    Tag_outside_colour = MAGENTA;
    Tag_inside_inactive_colour = WHITE;
    Tag_outside_inactive_colour = GRAY;
    Initial_under_colour = BLACK;
    Initial_over_colour = WHITE;
}
