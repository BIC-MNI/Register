#include  <def_user_interface.h>

private  int                        n_windows = 0;
private  graphics_window_struct     **windows;

public  void  record_graphics_window(
    graphics_window_struct   *graphics_window )
{
    ADD_ELEMENT_TO_ARRAY( n_windows, windows,
                          graphics_window, DEFAULT_CHUNK_SIZE );
}

private  int  get_window_index( window_struct  *window )
{
    static   current_index = -1;
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

public  void  unrecord_graphics_window(
    graphics_window_struct   *graphics_window )
{
    int      i;

    i = get_window_index( graphics_window->window );

    if( i >= 0 )
    {
        DELETE_ELEMENT_FROM_ARRAY( n_windows, windows, i, DEFAULT_CHUNK_SIZE );
    }
}

public  Boolean   lookup_event_viewports(
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

public  void  make_windows_up_to_date()
{
    int   i;

    for_less( i, 0, n_windows )
    {
        if( make_window_up_to_date( windows[i]->window,
                                    &windows[i]->graphics,
                                    windows[i]->current_buffer ) )
        {
            if( G_get_double_buffer_state( windows[i]->window ) )
                windows[i]->current_buffer = 1 - windows[i]->current_buffer;
        }
    }
}
