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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/update/update_window.c,v 1.9 1995-10-02 18:34:45 david Exp $";
#endif

#include  <register.h>

public  BOOLEAN  update_slice_display(
    main_struct     *main,
    int             current_buffer )
{
    BOOLEAN   drawn;
    int       volume, view;

    for_less( volume, 0, N_VOLUMES )
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

public  BOOLEAN  slices_to_be_updated(
    main_struct     *main,
    int             current_buffer )
{
    BOOLEAN   update_required;
    int       volume, view;

    update_required = FALSE;

    for_less( volume, 0, N_VOLUMES )
    {
        for_less( view, 0, N_VIEWS )
        {
            if( (main->trislice[volume].input_flag &&
                 !main->trislice[volume].slices[view].pixels_are_up_to_date) ||
                get_viewport_update_flag( &main->graphics,
                        get_slice_viewport_index(volume,view), NORMAL_PLANES,
                        current_buffer ) )
            {
                update_required = TRUE;
                break;
            }
        }

        if( update_required )
            break;
    }

    if( !update_required )
    {
        for_less( view, 0, N_VIEWS )
        {
            if( (main->merged.active_flag &&
                 !main->merged.slices[view].pixels_are_up_to_date) ||
                get_viewport_update_flag( &main->graphics,
                       get_slice_viewport_index(MERGED_VOLUME_INDEX,view),
                       NORMAL_PLANES, current_buffer ) )
            {
                update_required = TRUE;
                break;
            }
        }
    }

    return( update_required );
}
