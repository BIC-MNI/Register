#include  <def_user_interface.h>

private  void  delete_quit_confirm(
    popup_struct          *popup )
{
    delete_popup_window( popup );

    FREE( popup );

    set_quit_button_activity( get_ui_struct(), ON );
}

private  DEFINE_WIDGET_CALLBACK( quit_button_callback )   /* ARGSUSED */
{
    set_quit_program_flag();

    delete_quit_confirm( (popup_struct *) callback_data );
}

private  DEFINE_WIDGET_CALLBACK( dont_quit_button_callback )   /* ARGSUSED */
{
    delete_quit_confirm( (popup_struct *) callback_data );
}

private  DEFINE_EVENT_FUNCTION( kill_window_callback )   /* ARGSUSED */
{
    delete_quit_confirm( (popup_struct *) callback_data );
}

public  void  popup_quit_confirm(
    UI_struct   *ui )
{
    int               x, y;
    Point             point;
    object_struct     *object;
    text_struct       *text;
    widget_struct     *widget;
    popup_struct      *popup;

    set_quit_button_activity( ui, OFF );

    G_get_mouse_screen_position( &x, &y );

    ALLOC( popup, 1 );

    create_popup_window( popup, "Quit Dialog", x, y, Quit_x_size, Quit_y_size,
                         kill_window_callback, (void *) popup );

    x = Interface_x_spacing;
    y = Quit_y_size - 1 - Interface_y_spacing;

    if( !IF_tag_points_have_been_saved() )
    {
        object = create_object( TEXT );
        text = get_text_ptr( object );
        fill_Point( point, x, y - Message_font_size, 0.0);
        initialize_text( text, &point, Message_text_colour,
                         Message_font, Message_font_size );

        (void) strcpy( text->string, "The tag points have not been saved." );

        add_object_to_viewport( &popup->graphics.graphics, 0, NORMAL_PLANES,
                                object );

        y -= Message_font_size + Interface_y_spacing;
    }

    object = create_object( TEXT );
    text = get_text_ptr( object );
    fill_Point( point, x, y - Message_font_size, 0.0);
    initialize_text( text, &point, Message_text_colour,
                     Message_font, Message_font_size );

    (void) strcpy( text->string, "Do you want to quit?" );

    add_object_to_viewport( &popup->graphics.graphics, 0, NORMAL_PLANES,
                            object );

    y -= Message_font_size + Interface_y_spacing;

    initialize_widget_list( &popup->widgets );

    y -= Button_height;

    widget = create_button( &popup->graphics, 0,
                            x, y, Button_width, Button_height,
                            "Quit", ON, TRUE,
                            BUTTON_ACTIVE_COLOUR,
                            BUTTON_SELECTED_COLOUR,
                            BUTTON_INACTIVE_COLOUR,
                            BUTTON_PUSHED_COLOUR,
                            BUTTON_TEXT_COLOUR,
                            Button_text_font, Button_text_font_size,
                            quit_button_callback, (void *) popup );
    (void) add_widget_to_list( &popup->widgets, widget );

    x += Button_width + Interface_x_spacing;

    widget = create_button( &popup->graphics, 0,
                            x, y, Button_width, Button_height,
                            "Don't Quit", ON, TRUE,
                            BUTTON_ACTIVE_COLOUR,
                            BUTTON_SELECTED_COLOUR,
                            BUTTON_INACTIVE_COLOUR,
                            BUTTON_PUSHED_COLOUR,
                            BUTTON_TEXT_COLOUR,
                            Button_text_font, Button_text_font_size,
                            dont_quit_button_callback, (void *) popup );
    (void) add_widget_to_list( &popup->widgets, widget );
}
