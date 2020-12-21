/**
 * \file User_interface/windows/popup.c
 * \brief Create and manage popup (e.g. dialog) windows.
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

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( redraw_window_callback )
{
    popup_struct    *popup;
    Bitplane_types  bitplane;

    popup = (popup_struct *) callback_data;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics.graphics, bitplane );
        set_viewport_update_flag( &popup->graphics.graphics, 0, bitplane );
    }

    update_window( &popup->graphics );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( resize_window_callback )
{
    popup_struct     *popup;
    Bitplane_types   bitplane;

    popup = (popup_struct *) callback_data;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics.graphics, bitplane );
        set_viewport_update_flag( &popup->graphics.graphics, 0, bitplane );
    }
}

  void   create_popup_window(
    popup_struct          *popup,
    VIO_STR                title,
    int                   x_position,
    int                   y_position,
    int                   x_size,
    int                   y_size,
    event_function_type   quit_popup_callback,
    void                  *quit_callback_data )
{
    Bitplane_types    bitplane;

    if( G_create_window( title, x_position, y_position,
                         x_size, y_size, FALSE, TRUE,
                         FALSE, 0, &popup->graphics.window ) != VIO_OK )
    {
        (void) fprintf( stderr, "Cannot create popup window\n" );
        return;
    }

    set_window_event_callbacks( &popup->graphics );

    initialize_graphics_struct( &popup->graphics.graphics );
    initialize_event_viewports( &popup->graphics.event_viewports );
    initialize_widget_list( &popup->widgets );

    popup->graphics.current_buffer = 0;

    G_set_background_colour( popup->graphics.window, Popup_background_colour );

    G_set_automatic_clear_state( popup->graphics.window, FALSE );

    record_graphics_window( &popup->graphics );

    G_get_window_size( popup->graphics.window, &x_size, &y_size );

    set_graphics_viewport( &popup->graphics.graphics,
                           0, 0, x_size-1, 0, y_size-1 );

    set_graphics_viewport_background( &popup->graphics.graphics, 0,
                                      Popup_background_colour, 0 );

    set_event_viewport( &popup->graphics.event_viewports, Main_menu_viewport,
                        -1, -1, -1, -1 );

    add_event_viewport_callback( &popup->graphics.event_viewports,
                                 Main_menu_viewport,
                                 WINDOW_REDRAW_EVENT, -1, -1, -1, -1,
                                 redraw_window_callback, ANY_MODIFIER,
                                 (void *) popup );

    add_event_viewport_callback( &popup->graphics.event_viewports,
                                 Main_menu_viewport,
                                 WINDOW_RESIZE_EVENT, -1, -1, -1, -1,
                                 resize_window_callback, ANY_MODIFIER,
                                 (void *) popup );

    add_event_viewport_callback( &popup->graphics.event_viewports,
                                 Main_menu_viewport,
                                 WINDOW_QUIT_EVENT, -1, -1, -1, -1,
                                 quit_popup_callback, ANY_MODIFIER,
                                 quit_callback_data );

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics.graphics, bitplane );
        set_viewport_update_flag( &popup->graphics.graphics, 0, bitplane );
    }
}

  void  delete_popup_window(
    popup_struct   *popup )
{
    (void) G_delete_window( popup->graphics.window );

    delete_graphics_struct( &popup->graphics.graphics );
    delete_event_viewports( &popup->graphics.event_viewports );
    delete_widget_list( &popup->widgets );

    unrecord_graphics_window( &popup->graphics );
}
