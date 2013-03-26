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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/widgets/buttons.c,v 1.19 1998-06-29 15:02:06 david Exp $";
#endif

#include  <user_interface.h>

private  void  deselect_other_radio_buttons(
    widget_struct           *widget )
{
    widget_struct   *current_widget;
    button_struct   *button;

    button = get_widget_button( widget );

    current_widget = widget;

    while( button->next_radio_button != widget )
    {
        current_widget = button->next_radio_button;
        set_widget_selected( current_widget, OFF );

        button = get_widget_button( current_widget );
    }
}

private  VIO_BOOL  is_radio_button(
    button_struct  *button )
{
    return( button->next_radio_button != (widget_struct *) NULL );
}

public  void  define_radio_buttons(
    int            n_widgets,
    widget_struct  *widgets[] )
{
    int            i;
    button_struct  *button;

    for_less( i, 0, n_widgets )
    {
        button = get_widget_button( widgets[i] );
        button->next_radio_button = widgets[(i+1)%n_widgets];
    }
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( check_unpush_button )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = get_widget_button( widget );

    if( button->update_counter == 0 )
    {
        ++button->update_counter;
    }
    else if( button->update_counter == 1 )
    {
        ++button->update_counter;
        button->time_to_unpush = current_realtime_seconds() +
                                 Interface_highlight_time;
        button->push_callback( widget, button->callback_data );
    }
    else if( current_realtime_seconds() >= button->time_to_unpush )
    {
        /*--- calling this will cause update_button_activity() to be called
              and remove the check_unpush_button_event */

        set_widget_activity( widget, ON );
    }
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( push_button_event_callback )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = get_widget_button( widget );

    set_widget_activity( widget, OFF );

    if( button->toggle_flag )
    {
        set_toggle_button_state( widget, !button->state );
    }
    else if( is_radio_button( button ) )
    {
        set_widget_selected( widget, ON );
        deselect_other_radio_buttons( widget );
    }

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );

    button->update_counter = 0;
    add_global_event_callback( NO_EVENT, check_unpush_button,
                               ANY_MODIFIER, (void *) widget );
}

public  void  update_button_activity(
    widget_struct           *widget )
{
    button_struct   *button;

    button = get_widget_button( widget );

    update_button_colours( widget );

    set_event_viewport_callback_enabled( &widget->graphics->event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       push_button_event_callback, (void *) widget,
                       widget->active_flag );

    if( button->update_counter >= 0 )
    {
        button->update_counter = -1;
        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

public  void  update_button_colours(
    widget_struct   *widget )
{
    VIO_BOOL        colour_map_state;
    VIO_Colour         rectangle_colour;
    button_struct  *button;

    button = get_widget_button( widget );

    if( widget->active_flag )
    {
        if( button->toggle_flag )
        {
            rectangle_colour = button->active_colour;
        }
        else
        {
            if( widget->selected_flag )
                rectangle_colour = button->selected_colour;
            else
                rectangle_colour = button->active_colour;
        }
    }
    else
        rectangle_colour = button->inactive_colour;

    colour_map_state = G_get_colour_map_state(widget->graphics->window);

    if( widget->use_ui_colours )
    {
        button->polygons->colours[0] = get_ui_colour(colour_map_state,
                                            (UI_colours) rectangle_colour);

        button->text->colour = get_ui_colour( colour_map_state,
                                             (UI_colours) button->text_colour );
    }
    else
    {
        button->polygons->colours[0] = rectangle_colour;

        button->text->colour = button->text_colour;
    }
}

public  void  set_button_active_colour(
    widget_struct   *widget,
    VIO_Colour          col )
{
    button_struct  *button;

    button = get_widget_button( widget );

    button->active_colour = col;

    update_widget_colours( widget );
}

public  VIO_Colour  get_button_colour(
    widget_struct   *widget )
{
    button_struct  *button;

    button = get_widget_button( widget );

    return( button->active_colour );
}

public  void  position_button(
    widget_struct                 *widget,
    int                           x,
    int                           y )
{
    button_struct   *button;

    button = get_widget_button( widget );
    position_text_centred( button->text, widget->x, widget->y,
                           widget->x_size, widget->y_size );
    position_rectangle( button->polygons, widget->x, widget->y,
                        widget->x_size, widget->y_size );

    set_event_viewport_callback_viewport(
               &widget->graphics->event_viewports,
               widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
               push_button_event_callback,
               (void *) widget,
               x, x + widget->x_size - 1,
               y, y + widget->y_size - 1 );
}

public  void  set_toggle_button_state(
    widget_struct    *widget,
    VIO_BOOL          state )
{
    button_struct    *button;

    button = get_widget_button( widget );

    button->state = state;
    set_button_text( widget, button->toggle_text[button->state] );
    update_button_colours( widget );
}

public  void  set_button_text(
    widget_struct    *widget,
    VIO_STR           text_string )
{
    button_struct    *button;

    button = get_widget_button( widget );

    replace_string( &button->text->string, create_string(text_string) );
    position_text_centred( button->text, widget->x, widget->y,
                           widget->x_size, widget->y_size );

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );
}

private  void  create_button_graphics(
    widget_struct    *widget,
    VIO_STR           label,
    Font_types       text_font,
    VIO_Real             font_size )
{
    object_struct     *object;
    button_struct     *button;

    button = get_widget_button( widget );

    object = create_rectangle( BLACK );
    button->polygons = get_polygons_ptr( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );

    object = create_text( BLACK, text_font, font_size );

    button->text = get_text_ptr( object );

    if( button->toggle_flag )
        set_button_text( widget, button->toggle_text[button->state] );
    else
        set_button_text( widget, label );

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );
}

/* ARGSUSED */

public  void  delete_button(
    widget_struct  *widget )
{
    button_struct     *button;

    button = get_widget_button( widget );

    delete_string( button->toggle_text[0] );
    delete_string( button->toggle_text[1] );

    if( button->update_counter >= 0 )
    {
        button->update_counter = -1;
        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

private  widget_struct  *create_a_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_BOOL                    toggle_flag,
    VIO_BOOL                    initial_state,
    VIO_STR                     text1,
    VIO_STR                     text2,
    VIO_BOOL                    initial_activity,
    VIO_BOOL                    use_ui_colours,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = create_widget( BUTTON, x, y, x_size, y_size, initial_activity,
                            use_ui_colours, graphics, viewport_index );

    button = get_widget_button( widget );

    button->active_colour = active_colour;
    button->inactive_colour = inactive_colour;
    button->text_colour = text_colour;

    button->next_radio_button = (widget_struct *) 0;

    button->toggle_flag = toggle_flag;
    button->update_counter = -1;

    if( toggle_flag )
    {
        button->state = initial_state;
        button->toggle_text[0] = create_string( text1 );
        button->toggle_text[1] = create_string( text2 );
    }
    else
    {
        button->selected_colour = selected_colour;
        button->toggle_text[0] = create_string( NULL );
        button->toggle_text[1] = create_string( NULL );
    }

    button->push_callback = push_callback;
    button->callback_data = callback_data;

    add_event_viewport_callback( &graphics->event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 push_button_event_callback,
                                 ANY_MODIFIER, (void *) widget );

    create_button_graphics( widget, text1, text_font, font_size );

    update_button_activity( widget );

    update_button_colours( widget );

    position_button( widget, x, y );

    return( widget );
}

public  widget_struct *create_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_STR                     label,
    VIO_BOOL                    initial_activity,
    VIO_BOOL                    use_ui_colours,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     FALSE, FALSE, label, NULL,
                     initial_activity, use_ui_colours, active_colour,
                     selected_colour, inactive_colour,
                     text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}

public  widget_struct  *create_toggle_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_STR                     off_text,
    VIO_STR                     on_text,
    VIO_BOOL                    initial_state,
    VIO_BOOL                    initial_activity,
    VIO_BOOL                    use_ui_colours,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     TRUE, initial_state, off_text, on_text,
                     initial_activity, use_ui_colours, active_colour, BLACK,
                     inactive_colour, text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}
