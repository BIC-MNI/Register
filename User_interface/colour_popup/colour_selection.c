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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/colour_popup/colour_selection.c,v 1.11 1998-06-29 15:01:53 david Exp $";
#endif

#include  <user_interface.h>

typedef  struct
{
    int                     volume;
    int                     over_or_under;
    popup_struct            popup_window;
} colour_selection_struct;

private  void  delete_colour_selection(
    colour_selection_struct          *popup )
{
    delete_popup_window( &popup->popup_window );

    set_over_under_colour_activity( get_ui_struct(), popup->volume,
                                    popup->over_or_under, ON );

    FREE( popup );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( colour_button_callback )
{
    colour_selection_struct   *popup;

    popup = (colour_selection_struct *) callback_data;

    set_over_under_colour( get_ui_struct(), popup->volume, popup->over_or_under,
                           get_button_colour(widget) );

    delete_colour_selection( popup );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( cancel_callback )
{
    delete_colour_selection( (colour_selection_struct *) callback_data );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    delete_colour_selection( (colour_selection_struct *) callback_data );
}

public  void  popup_colour_selection(
    UI_struct   *ui,
    int         volume,
    int         over_or_under )
{
    int                       i, x, y;
    widget_struct             *widget;
    colour_selection_struct   *popup;
    static  STRING            over_under_names[] = { "Under", "Over" };
    char                      window_name[EXTREMELY_LARGE_STRING_SIZE];
    Colour                    colour;
    static  STRING            colours[] = { "BLACK", "WHITE",
                                            "RED", "GREEN", "BLUE",
                                            "CYAN", "MAGENTA", "YELLOW",
                                            "ORANGE" };

    set_over_under_colour_activity( ui, volume, over_or_under, OFF );

    ALLOC( popup, 1 );

    popup->volume = volume;
    popup->over_or_under = over_or_under;

    G_get_mouse_screen_position( &x, &y );

    (void) sprintf( window_name, "Volume %d %s Colour Selection", volume + 1,
                    over_under_names[over_or_under] );

    create_popup_window( &popup->popup_window, window_name, x, y,
                         Colour_selection_x_size, Colour_selection_y_size,
                         quit_window_callback, (void *) popup );

    initialize_widget_list( &popup->popup_window.widgets );

    x = Interface_x_spacing;
    y = Colour_selection_y_size - 1 - Interface_y_spacing - Button_height;

    for_less( i, 0, SIZEOF_STATIC_ARRAY( colours ) )
    {
        colour = convert_string_to_colour( colours[i] );

        widget = create_button( &popup->popup_window.graphics,
                                Main_menu_viewport,
                                x, y, Button_width, Button_height,
                                colours[i], ON, FALSE, colour,
                                get_ui_rgb_colour(BUTTON_SELECTED_COLOUR),
                                get_ui_rgb_colour(BUTTON_INACTIVE_COLOUR),
                                get_ui_rgb_colour(BUTTON_TEXT_COLOUR),
                                (Font_types) Button_text_font,
                                Button_text_font_size,
                                colour_button_callback, (void *) popup );

        (void) add_widget_to_list( &popup->popup_window.widgets, widget );

        x += Button_width + Interface_x_spacing;

        if( ((i + 1) % Colour_selection_n_across == 0) ||
            i == SIZEOF_STATIC_ARRAY(colours)-1 )
        {
            x = Interface_x_spacing;
            y -= Button_height + Interface_y_spacing;
        }
    }

    widget = create_button( &popup->popup_window.graphics, Main_menu_viewport,
                            x, y, Button_width, Button_height,
                            "Cancel", ON, FALSE,
                            get_ui_rgb_colour(BUTTON_ACTIVE_COLOUR),
                            get_ui_rgb_colour(BUTTON_SELECTED_COLOUR),
                            get_ui_rgb_colour(BUTTON_INACTIVE_COLOUR),
                            get_ui_rgb_colour(BUTTON_TEXT_COLOUR),
                            (Font_types) Button_text_font,
                            Button_text_font_size,
                            cancel_callback, (void *) popup );

    (void) add_widget_to_list( &popup->popup_window.widgets, widget );
}
