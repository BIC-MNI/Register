#include  <def_user_interface.h>

typedef  struct
{
    int                     volume;
    int                     over_or_under;
    graphics_window_struct  graphics;
    widgets_struct          widgets;
} colour_selection_struct;

private  void  delete_colour_selection(
    colour_selection_struct          *popup )
{
    delete_widget_list( &popup->widgets );

    delete_popup_window( &popup->graphics );

    set_over_under_colour_activity( get_ui_struct(), popup->volume,
                                    popup->over_or_under, ON );

    FREE( popup );
}

private  DEFINE_WIDGET_CALLBACK( colour_button_callback )   /* ARGSUSED */
{
    colour_selection_struct   *popup;

    popup = (colour_selection_struct *) callback_data;

    set_over_under_colour( get_ui_struct(), popup->volume, popup->over_or_under,
                           get_button_colour(widget) );

    delete_colour_selection( popup );
}

private  DEFINE_EVENT_FUNCTION( cancel_callback )   /* ARGSUSED */
{
    colour_selection_struct   *popup;

    popup = (colour_selection_struct *) callback_data;

    delete_colour_selection( popup );
}

public  void  popup_colour_selection(
    UI_struct   *ui,
    int         volume,
    int         over_or_under )
{
    int                       i, x, y;
    widget_struct             *widget;
    colour_selection_struct   *popup;
    static  char              *over_under_names[] = { "Under", "Over" };
    String                    window_name;
    String                    colour_name;
    static  Colour            colours[] = { BLACK, WHITE, RED, GREEN, BLUE,
                                            CYAN, MAGENTA, YELLOW };

    set_over_under_colour_activity( ui, volume, over_or_under, OFF );

    ALLOC( popup, 1 );

    popup->volume = volume;
    popup->over_or_under = over_or_under;

    G_get_mouse_screen_position( &x, &y );

    (void) sprintf( window_name, "Volume %d %s Colour Selection", volume + 1,
                    over_under_names[over_or_under] );

    create_popup_window( &popup->graphics, window_name, x, y,
                         Colour_selection_x_size, Colour_selection_y_size );

    initialize_widget_list( &popup->widgets );

    x = Interface_x_spacing;
    y = Colour_selection_y_size - 1 - Interface_y_spacing - Button_height;

    for_less( i, 0, SIZEOF_STATIC_ARRAY( colours ) )
    {
        convert_colour_to_string( colours[i], colour_name );

        widget = create_button( &popup->graphics, 0,
                                x, y, Button_width, Button_height,
                                colour_name, ON, FALSE, colours[i],
                                get_ui_rgb_colour(BUTTON_SELECTED_COLOUR),
                                get_ui_rgb_colour(BUTTON_INACTIVE_COLOUR),
                                get_ui_rgb_colour(BUTTON_PUSHED_COLOUR),
                                get_ui_rgb_colour(BUTTON_TEXT_COLOUR),
                                Button_text_font, Button_text_font_size,
                                colour_button_callback, (void *) popup );

        (void) add_widget_to_list( &popup->widgets, widget );

        x += Button_width + Interface_x_spacing;

        if( ((i + 1) % Colour_selection_n_across == 0) ||
            i == SIZEOF_STATIC_ARRAY(colours)-1 )
        {
            x = Interface_x_spacing;
            y -= Button_height + Interface_y_spacing;
        }
    }

    widget = create_button( &popup->graphics, 0,
                            x, y, Button_width, Button_height,
                            "Cancel", ON, FALSE,
                            get_ui_rgb_colour(BUTTON_ACTIVE_COLOUR),
                            get_ui_rgb_colour(BUTTON_SELECTED_COLOUR),
                            get_ui_rgb_colour(BUTTON_INACTIVE_COLOUR),
                            get_ui_rgb_colour(BUTTON_PUSHED_COLOUR),
                            get_ui_rgb_colour(BUTTON_TEXT_COLOUR),
                            Button_text_font, Button_text_font_size,
                            cancel_callback, (void *) popup );

    (void) add_widget_to_list( &popup->widgets, widget );
}
