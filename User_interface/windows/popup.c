#include  <def_user_interface.h>

private  DEFINE_EVENT_FUNCTION( redraw_window_callback )   /* ARGSUSED */
{
    popup_struct    *popup;
    Bitplane_types  bitplane;

    popup = (popup_struct *) callback_data;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics.graphics, bitplane );
        set_viewport_update_flag( &popup->graphics.graphics, 0, bitplane );
    }
}

private  DEFINE_EVENT_FUNCTION( resize_window_callback )   /* ARGSUSED */
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

public  void   create_popup_window(
    popup_struct      *popup,
    char              title[],
    int               x_position,
    int               y_position,
    int               x_size,
    int               y_size )
{
    Bitplane_types    bitplane;

    (void) G_create_window( title, x_position, y_position,
                            x_size, y_size, &popup->graphics.window );

    initialize_graphics_struct( &popup->graphics.graphics );
    initialize_event_viewports( &popup->graphics.event_viewports );
    initialize_widget_list( &popup->widgets );

    popup->graphics.current_buffer = 0;

    G_set_zbuffer_state( popup->graphics.window, OFF );
    G_set_colour_map_state( popup->graphics.window, FALSE );
    G_set_double_buffer_state( popup->graphics.window, ON );

    G_set_background_colour( popup->graphics.window, Popup_background_colour );

    G_set_automatic_clear_state( popup->graphics.window, OFF );

    record_graphics_window( &popup->graphics );

    G_get_window_size( popup->graphics.window, &x_size, &y_size );

    set_graphics_viewport( &popup->graphics.graphics,
                           0, 0, x_size-1, 0, y_size-1 );

    set_graphics_viewport_background( &popup->graphics.graphics, 0,
                                      Popup_background_colour, 0 );

    set_event_viewport( &popup->graphics.event_viewports, 0, -1, -1, -1, -1 );

    add_event_viewport_callback( &popup->graphics.event_viewports,
                                 0, WINDOW_REDRAW_EVENT, -1, -1, -1, -1,
                                 redraw_window_callback, ANY_MODIFIER,
                                 (void *) popup );

    add_event_viewport_callback( &popup->graphics.event_viewports,
                                 0, WINDOW_RESIZE_EVENT, -1, -1, -1, -1,
                                 resize_window_callback, ANY_MODIFIER,
                                 (void *) popup );

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics.graphics, bitplane );
        set_viewport_update_flag( &popup->graphics.graphics, 0, bitplane );
    }
}

public  void  delete_popup_window(
    popup_struct   *popup )
{
    (void) G_delete_window( popup->graphics.window );

    delete_graphics_struct( &popup->graphics.graphics );
    delete_event_viewports( &popup->graphics.event_viewports );
    delete_widget_list( &popup->widgets );

    unrecord_graphics_window( &popup->graphics );
}
