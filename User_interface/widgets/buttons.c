#include  <def_user_interface.h>

private  DEFINE_EVENT_FUNCTION( check_unpush_button )   /* ARGSUSED */
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = get_widget_button( widget );

    if( current_realtime_seconds() >= button->time_to_unpush )
    {
        set_widget_activity( widget, ON );

        button->time_to_unpush = -1.0;
        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

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

private  Boolean  is_radio_button(
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

private  DEFINE_EVENT_FUNCTION( push_button_event_callback )   /* ARGSUSED */
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
                              widget->viewport_index, NORMAL_PLANES );

    button->time_to_unpush = current_realtime_seconds() +
                             Interface_highlight_time;

    add_global_event_callback( NO_EVENT, check_unpush_button,
                               (void *) widget );

    button->push_callback( widget, button->callback_data );
}

public  void  update_button_colours(
    widget_struct   *widget )
{
    Boolean        colour_map_state;
    UI_colours     rectangle_colour;
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

    button->polygons->colours[0] = get_ui_colour(colour_map_state,
                                                 rectangle_colour);

    button->text->colour = get_ui_colour(colour_map_state,button->text_colour);
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
    Boolean          state )
{
    button_struct    *button;

    button = get_widget_button( widget );

    button->state = state;
    set_button_text( widget, button->toggle_text[button->state] );
    update_button_colours( widget );
}

public  void  set_button_text(
    widget_struct    *widget,
    char             text_string[] )
{
    button_struct    *button;

    button = get_widget_button( widget );

    (void) strcpy( button->text->string, text_string );
    position_text_centred( button->text, widget->x, widget->y,
                           widget->x_size, widget->y_size );

    set_viewport_update_flag( &widget->graphics->graphics,
                              widget->viewport_index, NORMAL_PLANES );
}

public  void  update_button_activity(
    widget_struct           *widget )
{
    update_button_colours( widget );

    set_event_viewport_callback_enabled( &widget->graphics->event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       push_button_event_callback, (void *) widget,
                       widget->active_flag );
}

private  void  create_button_graphics(
    widget_struct    *widget,
    char             label[],
    Font_types       text_font,
    Real             font_size )
{
    object_struct     *object;
    button_struct     *button;

    button = get_widget_button( widget );

    object = create_rectangle( get_ui_colour(button->active_colour,FALSE) );
    button->polygons = (polygons_struct *) get_object_pointer( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            widget->viewport_index, NORMAL_PLANES, object );

    object = create_text( get_ui_colour(button->text_colour,FALSE),
                          text_font, font_size );

    button->text = (text_struct *) get_object_pointer( object );

    if( button->toggle_flag )
        set_button_text( widget, button->toggle_text[button->state] );
    else
        set_button_text( widget, label );

    add_object_to_viewport( &widget->graphics->graphics,
                            widget->viewport_index, NORMAL_PLANES, object );
}

public  void  delete_button(
    widget_struct  *widget )      /* ARGSUSED */
{
    button_struct     *button;

    button = get_widget_button( widget );

    if( button->time_to_unpush >= 0.0 )
    {
        button->time_to_unpush = -1.0;
        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

private  widget_struct  *create_a_button(
    graphics_window_struct     *graphics,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Boolean                    toggle_flag,
    Boolean                    initial_state,
    char                       text1[],
    char                       text2[],
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 selected_colour,
    UI_colours                 inactive_colour,
    UI_colours                 pushed_colour,
    UI_colours                 text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = create_widget( BUTTON, x, y, x_size, y_size, initial_activity,
                            graphics, viewport_index );

    button = get_widget_button( widget );

    button->active_colour = active_colour;
    button->inactive_colour = inactive_colour;
    button->pushed_colour = pushed_colour;
    button->text_colour = text_colour;

    button->next_radio_button = (widget_struct *) 0;

    button->toggle_flag = toggle_flag;
    button->time_to_unpush = -1.0;

    if( toggle_flag )
    {
        button->state = initial_state;
        (void) strcpy( button->toggle_text[0], text1 );
        (void) strcpy( button->toggle_text[1], text2 );
    }
    else
    {
        button->selected_colour = selected_colour;
    }

    button->push_callback = push_callback;
    button->callback_data = callback_data;

    add_event_viewport_callback( &graphics->event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 push_button_event_callback,
                                 (void *) widget );

    create_button_graphics( widget, text1, text_font, font_size );

    update_button_activity( widget );

    update_button_colours( widget );

    position_button( widget, x, y );

    return( widget );
}

public  widget_struct *create_button(
    graphics_window_struct     *graphics,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    char                       label[],
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 selected_colour,
    UI_colours                 inactive_colour,
    UI_colours                 pushed_colour,
    UI_colours                 text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     FALSE, FALSE, label, (char *) 0,
                     initial_activity, active_colour,
                     selected_colour, inactive_colour,
                     pushed_colour, text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}

public  widget_struct  *create_toggle_button(
    graphics_window_struct     *graphics,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    char                       off_text[],
    char                       on_text[],
    Boolean                    initial_state,
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 inactive_colour,
    UI_colours                 pushed_colour,
    UI_colours                 text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data )
{
    return( create_a_button( graphics, viewport_index,
                     x, y, x_size, y_size,
                     TRUE, initial_state, off_text, on_text,
                     initial_activity, active_colour, BLACK,
                     inactive_colour, pushed_colour, text_colour,
                     text_font, font_size, push_callback, callback_data ) );
}
