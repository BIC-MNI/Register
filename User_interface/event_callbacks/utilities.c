#include  <user_interface.h>

public  void  start_interaction(
    UI_struct              *ui_info,
    int                    event_viewport_index,
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

public  void  terminate_interaction(
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

public  void  restore_mouse_position(
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

public  void  get_viewport_mouse_position(
    graphics_window_struct    *graphics_window,
    int                       event_viewport_index,
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
