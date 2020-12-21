/**
 * \file User_interface/event_callbacks/utilities.c
 * \brief Generically useful functions for event management.
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

  void  start_interaction(
    UI_struct              *ui_info,
    Viewport_types         event_viewport_index,
    Event_types            terminate_event,
    event_function_type    terminate_callback,
    event_function_type    no_event_callback )
{
    ui_info->interaction_viewport_index = event_viewport_index;

    add_global_event_callback( terminate_event, terminate_callback,
                               ANY_MODIFIER, (void *) NULL );
    add_global_event_callback( NO_EVENT, no_event_callback,
                               ANY_MODIFIER, (void *) NULL );

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 event_viewport_index,
                                 &ui_info->x_mouse_start,
                                 &ui_info->y_mouse_start );

    set_interaction_in_progress( TRUE );
}

  void  terminate_interaction(
    Event_types            terminate_event,
    event_function_type    terminate_callback,
    event_function_type    no_event_callback )
{
    remove_global_event_callback( terminate_event, terminate_callback,
                                  (void *) NULL );
    remove_global_event_callback( NO_EVENT, no_event_callback, 
                                  (void *) NULL );
    set_interaction_in_progress( FALSE );
}

  void  restore_mouse_position(
    UI_struct   *ui )
{
    int   x_min, x_max, y_min, y_max;
    int   x_window, y_window;

    get_event_viewport( &ui->graphics_window.event_viewports,
                        ui->interaction_viewport_index,
                        &x_min, &x_max, &y_min, &y_max );

    G_get_window_position( ui->graphics_window.window,
                           &x_window, &y_window );

    G_set_mouse_position( x_window + x_min + ui->x_mouse_start,
                          y_window + y_min + ui->y_mouse_start );
}

  void  get_viewport_mouse_position(
    graphics_window_struct    *graphics_window,
    Viewport_types            event_viewport_index,
    int                       *x_mouse,
    int                       *y_mouse )
{
    int   x_min, x_max, y_min, y_max;

    (void) G_get_mouse_position( graphics_window->window, x_mouse, y_mouse );

    get_event_viewport( &graphics_window->event_viewports,
                        event_viewport_index, &x_min, &x_max, &y_min, &y_max );

    *x_mouse -= x_min;
    *y_mouse -= y_min;
}
