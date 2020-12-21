/**
 * \file User_interface/widgets/buttons.c
 * \brief Implement button widgets - on/off, command, or radio buttons.
 *
 * \copyright Copyright 1993,1994,1995 David MacDonald,
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

static  void  deselect_other_radio_buttons(
    widget_struct           *widget )
{
    widget_struct   *current_widget;
    button_struct   *button;

    button = get_widget_button( widget );

    current_widget = widget;

    while( button->next_radio_button != widget )
    {
        current_widget = button->next_radio_button;
        set_widget_selected( current_widget, FALSE );

        button = get_widget_button( current_widget );
    }
}

static  VIO_BOOL  is_radio_button(
    button_struct  *button )
{
    return( button->next_radio_button != (widget_struct *) NULL );
}

  void  define_radio_buttons(
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

static  DEFINE_EVENT_FUNCTION( check_unpush_button )
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

        set_widget_activity( widget, TRUE );
    }
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( push_button_event_callback )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = get_widget_button( widget );

    set_widget_activity( widget, FALSE );

    if( button->toggle_count > 0 )
    {
        int new_state = button->state + 1;
        if ( new_state >= button->toggle_count )
        {
            new_state = 0;
        }
        set_toggle_button_state( widget, new_state );
    }
    else if( is_radio_button( button ) )
    {
        set_widget_selected( widget, TRUE );
        deselect_other_radio_buttons( widget );
    }

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );

    button->update_counter = 0;
    add_global_event_callback( NO_EVENT, check_unpush_button,
                               ANY_MODIFIER, (void *) widget );
}

  void  update_button_activity(
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

/** 
 * Given a colour, return whether to use a black or white text with it
 * such that the contrast is maximized.
 */
static VIO_Colour get_text_colour(VIO_Colour bkgd_colour)
{
    int r = get_Colour_r(bkgd_colour);
    int g = get_Colour_g(bkgd_colour);
    int b = get_Colour_b(bkgd_colour);
    int yiq = ((r * 299) + (g * 587) + (b * 114)) / 1000;
    return (yiq >= 128) ? BLACK : WHITE;
}

void  update_button_colours(
    widget_struct   *widget )
{
    VIO_Colour     rectangle_colour;
    button_struct  *button;

    button = get_widget_button( widget );

    if( widget->active_flag )
    {
        if( button->toggle_count > 0 )
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

    if( widget->use_ui_colours )
    {
        button->polygons->colours[0] = get_ui_colour(FALSE,
                                            (UI_colours) rectangle_colour);

        button->text->colour = get_text_colour( button->polygons->colours[0] );
    }
    else
    {
        button->polygons->colours[0] = rectangle_colour;

        button->text->colour = get_text_colour( rectangle_colour );
    }
}

  void  set_button_active_colour(
    widget_struct   *widget,
    VIO_Colour          col )
{
    button_struct  *button;

    button = get_widget_button( widget );

    button->active_colour = col;

    update_widget_colours( widget );
}

  VIO_Colour  get_button_colour(
    widget_struct   *widget )
{
    button_struct  *button;

    button = get_widget_button( widget );

    return( button->active_colour );
}

  void  position_button(
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

  void  set_toggle_button_state(
    widget_struct    *widget,
    int              state )
{
    button_struct    *button;

    button = get_widget_button( widget );

    button->state = state;
    set_button_text( widget, button->toggle_text[button->state] );
    update_button_colours( widget );
}

  void  set_button_text(
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

static  void  create_button_graphics(
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

    if( button->toggle_count > 0 )
        set_button_text( widget, button->toggle_text[button->state] );
    else
        set_button_text( widget, label );

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );
}

/* ARGSUSED */

  void  delete_button(
    widget_struct  *widget )
{
    button_struct     *button;
    int               i;

    button = get_widget_button( widget );

    if ( button->toggle_count > 0 )
    {
        for (i = 0; i < button->toggle_count; i++)
        {
            delete_string( button->toggle_text[i] );
        }
        FREE( button->toggle_text );
    }

    if( button->update_counter >= 0 )
    {
        button->update_counter = -1;
        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

static  widget_struct  *create_a_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    int                        toggle_count,
    int                        initial_state,
    VIO_STR                     *text,
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
    int             i;

    widget = create_widget( BUTTON, x, y, x_size, y_size, initial_activity,
                            use_ui_colours, graphics, viewport_index );

    button = get_widget_button( widget );

    button->active_colour = active_colour;
    button->inactive_colour = inactive_colour;
    button->text_colour = text_colour;

    button->next_radio_button = (widget_struct *) 0;

    button->toggle_count = toggle_count;
    button->update_counter = -1;

    if( toggle_count > 0 )
    {
        button->state = initial_state;
        ALLOC( button->toggle_text, toggle_count );
        for (i = 0; i < toggle_count; i++)
        {
            button->toggle_text[i] = create_string( text[i] );
        }
    }
    else
    {
        button->selected_colour = selected_colour;
        button->toggle_text = NULL;
    }

    button->push_callback = push_callback;
    button->callback_data = callback_data;

    add_event_viewport_callback( &graphics->event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 push_button_event_callback,
                                 ANY_MODIFIER, (void *) widget );

    create_button_graphics( widget, text[0], text_font, font_size );

    update_button_activity( widget );

    update_button_colours( widget );

    position_button( widget, x, y );

    return( widget );
}

  widget_struct *create_button(
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
    VIO_STR text[1];
    text[0] = label;
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     0, 0, text,
                     initial_activity, use_ui_colours, active_colour,
                     selected_colour, inactive_colour,
                     text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}

  widget_struct  *create_toggle_button(
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
    VIO_STR text[2];
    text[0] = off_text;
    text[1] = on_text;
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     2, initial_state, text,
                     initial_activity, use_ui_colours, active_colour, BLACK,
                     inactive_colour, text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}


widget_struct  *create_multi_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    int                        n_states,
    VIO_STR                    state_text[],
    VIO_BOOL                   initial_state,
    VIO_BOOL                   initial_activity,
    VIO_BOOL                   use_ui_colours,
    VIO_Colour                 active_colour,
    VIO_Colour                 inactive_colour,
    VIO_Colour                 text_colour,
    Font_types                 text_font,
    VIO_Real                   font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     n_states, initial_state, state_text,
                     initial_activity, use_ui_colours, active_colour, BLACK,
                     inactive_colour, text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}
