#include  <def_user_interface.h>

private  DEFINE_EVENT_FUNCTION( redraw_window_callback )   /* ARGSUSED */
{
    graphics_window_struct   *popup;
    Bitplane_types           bitplane;

    popup = (graphics_window_struct *) callback_data;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics, bitplane );
        set_viewport_update_flag( &popup->graphics, 0, bitplane );
    }
}

private  DEFINE_EVENT_FUNCTION( resize_window_callback )   /* ARGSUSED */
{
    graphics_window_struct   *popup;
    Bitplane_types           bitplane;

    popup = (graphics_window_struct *) callback_data;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics, bitplane );
        set_viewport_update_flag( &popup->graphics, 0, bitplane );
    }
}

public  void   create_popup_window(
    graphics_window_struct   *popup,
    char                     title[],
    int                      x_position,
    int                      y_position,
    int                      x_size,
    int                      y_size )
{
    Bitplane_types          bitplane;

    (void) G_create_window( title, x_position, y_position,
                            x_size, y_size,
                            &popup->window );

    initialize_graphics_struct( &popup->graphics );
    initialize_event_viewports( &popup->event_viewports );

    popup->current_buffer = 0;

    G_set_zbuffer_state( popup->window, OFF );
    G_set_colour_map_state( popup->window, FALSE );
    G_set_double_buffer_state( popup->window, ON );

    G_set_background_colour( popup->window, Popup_background_colour );

    G_set_automatic_clear_state( popup->window, OFF );

    record_graphics_window( popup );

    G_get_window_size( popup->window, &x_size, &y_size );

    set_graphics_viewport( &popup->graphics, 0, 0, x_size-1, 0, y_size-1 );

    set_graphics_viewport_background( &popup->graphics, 0,
                                      Popup_background_colour, 0 );

    set_event_viewport( &popup->event_viewports, 0, -1, -1, -1, -1 );

    add_event_viewport_callback( &popup->event_viewports,
                                 0, WINDOW_REDRAW_EVENT, -1, -1, -1, -1,
                                 redraw_window_callback, ANY_MODIFIER,
                                 (void *) popup );

    add_event_viewport_callback( &popup->event_viewports,
                                 0, WINDOW_RESIZE_EVENT, -1, -1, -1, -1,
                                 resize_window_callback, ANY_MODIFIER,
                                 (void *) popup );

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics, bitplane );
        set_viewport_update_flag( &popup->graphics, 0, bitplane );
    }
}

public  void  delete_popup_window(
    graphics_window_struct   *popup )
{
    (void) G_delete_window( popup->window );

    delete_graphics_struct( &popup->graphics );
    delete_event_viewports( &popup->event_viewports );

    unrecord_graphics_window( popup );
}
