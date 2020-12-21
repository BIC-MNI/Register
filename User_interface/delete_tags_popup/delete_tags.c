/**
 * \file delete_tags.c
 * \brief Implement and create the confirmation dialog for tag deletion.
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

static  void  delete_tags_confirm(
    popup_struct          *popup )
{
    delete_popup_window( popup );

    FREE( popup );

    set_delete_tags_button_activity( get_ui_struct(), TRUE );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( delete_button_callback )
{
    delete_all_tag_points( get_ui_struct() );

    delete_tags_confirm( (popup_struct *) callback_data );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( cancel_button_callback )
{
    delete_tags_confirm( (popup_struct *) callback_data );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( kill_window_callback )
{
    delete_tags_confirm( (popup_struct *) callback_data );
}

  void  popup_delete_tags(
    UI_struct   *ui )
{
    int               x, y;
    VIO_Point             point;
    object_struct     *object;
    text_struct       *text;
    widget_struct     *widget;
    popup_struct      *popup;

    set_delete_tags_button_activity( ui, FALSE );

    G_get_mouse_screen_position( &x, &y );

    ALLOC( popup, 1 );

    create_popup_window( popup, "Delete Tags Dialog", x, y,
                         Delete_tags_x_size, Delete_tags_y_size,
                         kill_window_callback, (void *) popup );

    x = Interface_x_spacing;
    y = Delete_tags_y_size - 1 - Interface_y_spacing;

    object = create_object( TEXT );
    text = get_text_ptr( object );
    fill_Point( point, x, (VIO_Real) y - Message_font_size, 0.0 );
    initialize_text( text, &point, Message_text_colour,
                     (Font_types) Message_font, Message_font_size );

    replace_string( &text->string, create_string("Delete all tag points?") );

    add_object_to_viewport( &popup->graphics.graphics, 0, NORMAL_PLANES,
                            object );

    y -= (int) Message_font_size + Interface_y_spacing;

    initialize_widget_list( &popup->widgets );

    y -= Button_height;

    widget = create_button( &popup->graphics, Main_menu_viewport,
                            x, y, Button_width, Button_height,
                            "Delete", TRUE, TRUE,
                            BUTTON_ACTIVE_COLOUR,
                            BUTTON_SELECTED_COLOUR,
                            BUTTON_INACTIVE_COLOUR,
                            BUTTON_TEXT_COLOUR,
                            (Font_types) Button_text_font,
                            Button_text_font_size,
                            delete_button_callback, (void *) popup );
    (void) add_widget_to_list( &popup->widgets, widget );

    x += Button_width + Interface_x_spacing;

    widget = create_button( &popup->graphics, Main_menu_viewport,
                            x, y, Button_width, Button_height,
                            "Cancel", TRUE, TRUE,
                            BUTTON_ACTIVE_COLOUR,
                            BUTTON_SELECTED_COLOUR,
                            BUTTON_INACTIVE_COLOUR,
                            BUTTON_TEXT_COLOUR,
                            (Font_types) Button_text_font,
                            Button_text_font_size,
                            cancel_button_callback, (void *) popup );
    (void) add_widget_to_list( &popup->widgets, widget );
}
