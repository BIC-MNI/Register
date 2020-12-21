/**
 * \file Functionality/slices/updates.c
 * \brief Handle redraw control for the slice views.
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

#include <register.h>

  void  set_recreate_3_slices_flags(
    main_struct  *main,
    int          volume_index )
{
    int  view;

    for_less( view, 0, N_VIEWS )
        set_recreate_slice_flag( main, volume_index, view );
}

  void  set_recreate_slice_flag(
    main_struct  *main,
    int          volume,
    int          view )
{
    if( volume == MERGED_VOLUME_INDEX )
    {
        main->merged.slices[view].pixels_are_up_to_date = FALSE;
    }
    else
    {
        main->trislice[volume].slices[view].pixels_are_up_to_date = FALSE;
    }
}

  void  set_update_slice_viewport_flag(
    main_struct     *main,
    int             volume,
    int             view,
    Bitplane_types  bitplane )
{
    set_viewport_update_flag( &main->graphics,
                              get_slice_viewport_index(volume,view),
                              bitplane );
}
