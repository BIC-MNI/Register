/**
 * \file Functionality/update/update_window.c
 * \brief Control the updating of the main window.
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

  VIO_BOOL  update_slice_display(
    main_struct     *main,
    int             current_buffer )
{
    VIO_BOOL   drawn;
    int       volume, view;

    for_less( volume, 0, main->n_volumes_displayed - 1 )
    {
        if( main->trislice[volume].input_flag )
        {
            for_less( view, 0, N_VIEWS )
            {
                if( !main->trislice[volume].slices[view].pixels_are_up_to_date )
                {
                    create_slice_pixels( main, volume, view );
                    main->trislice[volume].slices[view].pixels_are_up_to_date =
                                                                TRUE;

                    set_viewport_update_flag( &main->graphics,
                               get_slice_viewport_index(volume,view),
                               NORMAL_PLANES );
                }
            }
        }
    }

    if( main->merged.active_flag )
    {
        for_less( view, 0, N_VIEWS )
        {
            if( !main->merged.slices[view].pixels_are_up_to_date )
            {
                create_merged_pixels( main, view );
                main->merged.slices[view].pixels_are_up_to_date = TRUE;

                set_viewport_update_flag( &main->graphics,
                       get_slice_viewport_index(MERGED_VOLUME_INDEX,view),
                       NORMAL_PLANES );
            }
        }
    }

#ifndef DEBUG
    drawn = redraw_out_of_date_viewports( &main->graphics, main->window,
                                          current_buffer );
#else
    drawn = FALSE;
#endif

    return( drawn );
}
