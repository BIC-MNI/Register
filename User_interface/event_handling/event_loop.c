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

#include  <user_interface.h>

  void  set_quit_program_flag( void )
{
    exit( 0 );
}

static  void  timer_function(
    void  *data )
{
    handle_event( NO_EVENT, NULL, 0 );
    make_windows_up_to_date();
    G_add_timer_function( Timer_interval, timer_function, NULL );
}

  void  event_loop( void )
{
    G_add_timer_function( Timer_interval, timer_function, NULL );

    G_main_loop();
}

  void   handle_event(
    Event_types       event_type,
    Gwindow           event_window,
    int               key_pressed )
{
    int                     shift_state;
    int                     x_mouse, y_mouse;
    event_viewports_struct  *event_viewports;

    shift_state = G_get_shift_key_state() || G_get_ctrl_key_state() ||
                  G_get_alt_key_state();

    x_mouse = 0;
    y_mouse = 0;

    if( !execute_global_event_callbacks( shift_state, event_type,
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
