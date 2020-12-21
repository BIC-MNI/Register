/**
 * \file User_interface/event_handling/window_events.c
 * \brief Handle events associated with a specific window.
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

static  void  update_callback(
    Gwindow   window,
    void      *data )
{
    handle_event( WINDOW_REDRAW_EVENT, window, 0 );
}

static  void  resize_callback(
    Gwindow   window,
    int       x,
    int       y,
    int       x_size,
    int       y_size,
    void      *data )
{
    handle_event( WINDOW_RESIZE_EVENT, window, 0 );
}

static  void  key_down_callback(
    Gwindow   window,
    int       key,
    void      *data )
{
    handle_event( KEY_DOWN_EVENT, window, key );
}

static  void  mouse_movement_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( MOUSE_MOVEMENT_EVENT, window, 0 );
}

static  void  left_down_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( LEFT_MOUSE_DOWN_EVENT, window, 0 );
}

static  void  left_up_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( LEFT_MOUSE_UP_EVENT, window, 0 );
}

static  void  middle_down_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( MIDDLE_MOUSE_DOWN_EVENT, window, 0 );
}

static  void  middle_up_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( MIDDLE_MOUSE_UP_EVENT, window, 0 );
}

static  void  right_down_callback( Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( RIGHT_MOUSE_DOWN_EVENT, window, 0 );
}

static  void  right_up_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( RIGHT_MOUSE_UP_EVENT, window, 0 );
}

static  void  iconify_callback(
    Gwindow   window,
    void      *data )
{
    handle_event( WINDOW_ICONIZED_EVENT, window, 0 );
}

static  void  deiconify_callback(
    Gwindow   window,
    void      *data )
{
    handle_event( WINDOW_DEICONIZED_EVENT, window, 0 );
}

static  void  enter_callback(
    Gwindow   window,
    void      *data )
{
    handle_event( WINDOW_ENTER_EVENT, window, 0 );
}

static  void  leave_callback(
    Gwindow   window,
    void      *data )
{
    handle_event( WINDOW_LEAVE_EVENT, window, 0 );
}

static  void  quit_callback(
    Gwindow   window,
    void      *data )
{
    handle_event( WINDOW_QUIT_EVENT, window, 0 );
}

static  void  scroll_up_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( SCROLL_UP_EVENT, window, 0 );
}

static  void  scroll_down_callback(
    Gwindow   window,
    int       x,
    int       y,
    void      *data )
{
    handle_event( SCROLL_DOWN_EVENT, window, 0 );
}

  void  set_window_event_callbacks(
    graphics_window_struct    *window )
{
    Gwindow  gwindow;

    gwindow = window->window;

    G_set_update_function( gwindow, update_callback, NULL );
    G_set_resize_function( gwindow, resize_callback, NULL);
    G_set_key_down_function( gwindow, key_down_callback, NULL);
    G_set_mouse_movement_function( gwindow, mouse_movement_callback, NULL);
    G_set_left_mouse_down_function( gwindow, left_down_callback, NULL);
    G_set_left_mouse_up_function( gwindow, left_up_callback, NULL);
    G_set_middle_mouse_down_function( gwindow, middle_down_callback, NULL);
    G_set_middle_mouse_up_function( gwindow, middle_up_callback, NULL);
    G_set_right_mouse_down_function( gwindow, right_down_callback, NULL);
    G_set_right_mouse_up_function( gwindow, right_up_callback, NULL);
    G_set_iconify_function( gwindow, iconify_callback, NULL);
    G_set_deiconify_function( gwindow, deiconify_callback, NULL);
    G_set_window_enter_function( gwindow, enter_callback, NULL);
    G_set_window_leave_function( gwindow, leave_callback, NULL);
    G_set_window_quit_function( gwindow, quit_callback, NULL);
    G_set_scroll_up_function( gwindow, scroll_up_callback, NULL);
    G_set_scroll_down_function( gwindow, scroll_down_callback, NULL);
}
