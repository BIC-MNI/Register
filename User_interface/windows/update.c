/**
 * \file User_interface/windows/update.c
 * \brief Basic window redraw management.
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

#include  <user_interface.h>

  VIO_BOOL  make_window_up_to_date(
    window_struct    *window,
    graphics_struct  *graphics,
    int              current_buffer )
{
    VIO_BOOL          something_was_drawn;

    something_was_drawn = redraw_out_of_date_viewports( graphics, window,
                                                        current_buffer );

    if( window == get_ui_struct()->graphics_window.window &&
        IF_redraw_slices( current_buffer ) )
        something_was_drawn = TRUE;

    if( something_was_drawn )
    {
        G_set_bitplanes( window, NORMAL_PLANES );
        G_update_window( window );
    }

    return( something_was_drawn );
}

  void  set_clear_and_update_flags( UI_struct  *ui_struct )
{
    int             volume, view;
    Bitplane_types  bitplane;
    Viewport_types  viewport;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &ui_struct->graphics_window.graphics,
                                  bitplane );

        for_enum( viewport, N_UI_viewports, Viewport_types )
        {
            set_viewport_update_flag( &ui_struct->graphics_window.graphics,
                                      (int) viewport, bitplane );
        }
    }

    for_less( volume, 0, ui_struct->n_volumes_displayed )
    {
        for_less( view, 0, N_VIEWS )
        {
            for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
                IF_set_update_slice_viewport_flag( volume, view, bitplane );
        }
    }

    ui_struct->graphics_window.current_buffer = 0;
}

  void  set_recreate_all_slice_flags( void )
{
    int             volume, view;
    UI_struct  *ui_struct = get_ui_struct();

    for_less( volume, 0, ui_struct->n_volumes_displayed )
    {
        for_less( view, 0, N_VIEWS )
        {
            IF_set_recreate_slice_flag( volume, view );
        }
    }
}
