/**
 * \file Functionality/initialize/initialize.c
 * \brief Initialize functionality components...
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

#include  <register.h>

static   main_struct      main_info;

  main_struct  *get_main_struct( void )
{
    return( &main_info );
}

  VIO_Status   initialize_register(
    Gwindow   window,
    VIO_STR    executable_name,
    int n_volumes )
{
    int             volume, view;
    Bitplane_types  bitplane;

    main_info.window = window;
    main_info.n_volumes_displayed = n_volumes + 1;
    if (main_info.n_volumes_displayed < 3)
      main_info.n_volumes_displayed = 3;

    initialize_graphics_struct( &main_info.graphics );

    initialize_slices( &main_info );

    initialize_tag_points( &main_info );

    main_info.original_volume_filename = create_string( NULL );

    for_less( volume, 0, main_info.n_volumes_displayed - 1 )
        main_info.trislice[volume].filename = create_string( NULL );

    for_less( volume, 0, main_info.n_volumes_displayed )
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
    main_info.cursor_visibility = TRUE;
    main_info.degrees_continuity = -1;
    create_linear_transform( &main_info.resampling_transform,
                             (VIO_Transform *) NULL );

    main_info.render_storage = initialize_render_storage();

    return( VIO_OK );
}

  void   terminate_register( void )
{
    int   volume;

    delete_string( main_info.original_volume_filename );

    for_less( volume, 0, main_info.n_volumes_displayed - 1 )
        delete_string( main_info.trislice[volume].filename );

    terminate_slices( &main_info );

    /* --- since delete_tag_points() makes calls to remove objects from the
           graphics struct, it must be called before delete_graphics_struct() */

    delete_tag_points( &main_info );
    delete_graphics_struct( &main_info.graphics );

    delete_render_storage( main_info.render_storage );

    delete_general_transform( &main_info.resampling_transform );
}

