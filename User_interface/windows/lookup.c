/**
 * \file User_interface/windows/lookup.c
 * \brief Functions for managing multiple windows.
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

static  int                        n_windows = 0;
static  graphics_window_struct     **windows;

  void  record_graphics_window(
    graphics_window_struct   *graphics_window )
{
    ADD_ELEMENT_TO_ARRAY( windows, n_windows,
                          graphics_window, DEFAULT_CHUNK_SIZE );
}

static  int  get_window_index( window_struct  *window )
{
    static   int  current_index = -1;
    int      i;

    if( current_index >= 0 && current_index < n_windows &&
        windows[current_index]->window == window )
    {
        return( current_index );
    }
   
    for_less( i, 0, n_windows )
    {
        if( windows[i]->window == window )
        {
            current_index = i;
            break;
        }
    }

    if( i >= n_windows )
        i = -1;

    return( i );
}

  void  unrecord_graphics_window(
    graphics_window_struct   *graphics_window )
{
    int      i;

    i = get_window_index( graphics_window->window );

    if( i >= 0 )
    {
        DELETE_ELEMENT_FROM_ARRAY( windows, n_windows, i, DEFAULT_CHUNK_SIZE );
    }
}

  VIO_BOOL   lookup_event_viewports(
    window_struct           *window,
    event_viewports_struct  **event_viewports )
{
    int   i;

    i = get_window_index( window );

    if( i >= 0 )
    {
        *event_viewports = &windows[i]->event_viewports;
    }

    return( i >= 0 );
}

  void  update_window(
    graphics_window_struct   *window )
{
    if( make_window_up_to_date( window->window, &window->graphics,
                                window->current_buffer ) )
    {
        if( G_get_double_buffer_state( window->window ) )
            window->current_buffer = 1 - window->current_buffer;
    }
}

  void  make_windows_up_to_date( void )
{
    int   i;

    for_less( i, 0, n_windows )
        update_window( windows[i] );
}

  void  delete_all_graphics_windows( void )
{
    int   i;

    for_less( i, 0, n_windows )
    {
        delete_graphics_struct( &windows[i]->graphics );

        delete_event_viewports( &windows[i]->event_viewports );

        /* don't actually call G_delete_window here, as it
         * will trigger an error since glut has already been
         * deinitialized.
         */
    }
    if( n_windows > 0 )
        FREE( windows );
}
