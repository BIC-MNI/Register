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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/event_handling/event_loop.c,v 1.6 1995-07-31 19:54:23 david Exp $";
#endif

#include  <user_interface.h>

private  void   handle_event(
    Event_types        event_type,
    window_struct      *event_window,
    int                key_pressed );

private  BOOLEAN   quit_flag = FALSE;

public  void  set_quit_program_flag()
{
    quit_flag = TRUE;
}

public  void  event_loop()
{
    int                     key_pressed;
    Event_types             event_type;
    window_struct           *event_window;

    do
    {
        event_type = G_get_event( &event_window, &key_pressed );

        handle_event( event_type, event_window, key_pressed );

        if( event_type == NO_EVENT )
            make_windows_up_to_date();
    }
    while( !quit_flag );
}

public  void  force_update_all_windows()
{
    handle_event( NO_EVENT, (window_struct *) NULL, 0 );
    make_windows_up_to_date();
}

private  void   handle_event(
    Event_types              event_type,
    window_struct            *event_window,
    int                      key_pressed )
{
    static                  shift_state = OFF;
    int                     x_mouse, y_mouse;
    event_viewports_struct  *event_viewports;

    if( (event_type == KEY_DOWN_EVENT || event_type == KEY_UP_EVENT) &&
        (key_pressed == LEFT_SHIFT_KEY ||
         key_pressed == RIGHT_SHIFT_KEY ||
         key_pressed == LEFT_CTRL_KEY ||
         key_pressed == RIGHT_CTRL_KEY ||
         key_pressed == LEFT_ALT_KEY ||
         key_pressed == RIGHT_ALT_KEY) )
    {
        shift_state = (event_type == KEY_DOWN_EVENT);
    }
    else if( event_type == WINDOW_LEAVE_EVENT )
        shift_state = FALSE;
    else if( !execute_global_event_callbacks( shift_state, event_type,
                                              key_pressed ) &&
        event_is_allowable( event_type ) &&
        (!mouse_must_be_in_window(event_type) ||
         G_get_mouse_position( event_window, &x_mouse, &y_mouse )) &&
        lookup_event_viewports( event_window, &event_viewports ) )
    {
        execute_event_viewport_events( shift_state, event_viewports,
                                       x_mouse, y_mouse,
                                       event_type, key_pressed );
    }
}
