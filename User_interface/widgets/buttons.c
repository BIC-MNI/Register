#include  <def_user_interface.h>

private  void  set_button_activity_colour( widget_struct * );

private  DEFINE_EVENT_FUNCTION( check_unpush_button )   /* ARGSUSED */
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = get_widget_button( widget );

    if( current_realtime_seconds() >= button->time_to_unpush )
    {
        if( !button->toggle_flag )
        {
            button->state = FALSE;
            update_button_colours( widget,
                                   G_get_colour_map_state(get_ui_struct()->
                                        graphics_window.window) );
        }

        set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                                  widget->viewport_index, NORMAL_PLANES );

        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

private  DEFINE_EVENT_FUNCTION( push_button_event_callback )   /* ARGSUSED */
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = get_widget_button( widget );

    if( button->toggle_flag )
    {
        button->state = !button->state;
        set_button_text( widget, button->toggle_text[button->state] );
    }
    else
        button->state = TRUE;

    update_button_colours( widget, G_get_colour_map_state(get_ui_struct()->
                                               graphics_window.window) );

    set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );

    button->time_to_unpush = current_realtime_seconds() +
                             Interface_highlight_time;
    add_global_event_callback( NO_EVENT, check_unpush_button,
                               (void *) widget );

    button->push_callback( widget );
}

public  void  update_button_colours(
    widget_struct   *widget,
    Boolean         colour_map_state )
{
    UI_colours     rectangle_colour;
    button_struct  *button;

    button = get_widget_button( widget );

    if( widget->active_flag )
    {
        if( button->toggle_flag )
            rectangle_colour = button->active_colour;
        else if( button->state )
            rectangle_colour = button->pushed_colour;
        else
            rectangle_colour = button->active_colour;
    }
    else
        rectangle_colour = button->inactive_colour;

    button->polygons->colours[0] = get_ui_colour(colour_map_state,
                                                 rectangle_colour);

    button->text->colour = get_ui_colour(colour_map_state,button->text_colour);
}

public  void  position_button(
    widget_struct                 *widget,
    event_viewports_struct        *event_viewports,
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
               event_viewports,
               widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
               push_button_event_callback,
               (void *) widget,
               x, x + widget->x_size - 1,
               y, y + widget->y_size - 1 );
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
}

public  void  update_button_activity(
    widget_struct           *widget,
    event_viewports_struct  *event_viewports,
    Boolean                 colour_map_state )
{
    update_button_colours( widget, colour_map_state );

    set_event_viewport_callback_enabled( event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       push_button_event_callback, (void *) widget,
                       widget->active_flag );
}

private  void  create_button_graphics(
    UI_struct        *ui_info,
    int              viewport_index,
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

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );

    object = create_text( get_ui_colour(button->text_colour,FALSE),
                          text_font, font_size );

    button->text = (text_struct *) get_object_pointer( object );

    if( button->toggle_flag )
        set_button_text( widget, button->toggle_text[button->state] );
    else
        set_button_text( widget, label );

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );
}

private  int  create_a_button(
    UI_struct                  *ui_info,
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
    UI_colours                 inactive_colour,
    UI_colours                 pushed_colour,
    UI_colours                 text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback )
{
    int             widget_index;
    widget_struct   *widget;
    button_struct   *button;

    widget = create_widget( BUTTON, x, y, x_size, y_size, initial_activity,
                            viewport_index );

    button = get_widget_button( widget );

    button->active_colour = active_colour;
    button->inactive_colour = inactive_colour;
    button->pushed_colour = pushed_colour;
    button->text_colour = text_colour;

    button->toggle_flag = toggle_flag;

    if( toggle_flag )
    {
        button->state = initial_state;
        (void) strcpy( button->toggle_text[0], text1 );
        (void) strcpy( button->toggle_text[1], text2 );
    }
    else
        button->state = FALSE;

    button->push_callback = push_callback;

    add_event_viewport_callback( &ui_info->graphics_window.event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 push_button_event_callback,
                                 (void *) widget );

    create_button_graphics( ui_info, viewport_index, widget, text1,
                            text_font, font_size );

    update_button_activity( widget, &ui_info->graphics_window.event_viewports,
                            G_get_colour_map_state(
                                 ui_info->graphics_window.window) );

    update_button_colours( widget,
                           G_get_colour_map_state(
                                 ui_info->graphics_window.window) );

    position_button( widget, &ui_info->graphics_window.event_viewports,
                     x, y );

    widget_index = add_widget_to_list( &ui_info->widget_list[viewport_index],
                                       widget );

    return( widget_index );
}

public  int  create_button(
    UI_struct                  *ui_info,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    char                       label[],
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 inactive_colour,
    UI_colours                 pushed_colour,
    UI_colours                 text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback )
{
    return( create_a_button( ui_info, viewport_index,
                     x, y, x_size, y_size,
                     FALSE, FALSE, label, (char *) 0,
                     initial_activity, active_colour, inactive_colour,
                     pushed_colour, text_colour,
                     text_font, font_size, push_callback ) );
}

public  int  create_toggle_button(
    UI_struct                  *ui_info,
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
    widget_callback_type       push_callback )
{
    return( create_a_button( ui_info, viewport_index,
                     x, y, x_size, y_size,
                     TRUE, initial_state, off_text, on_text,
                     initial_activity, active_colour, inactive_colour,
                     pushed_colour, text_colour,
                     text_font, font_size, push_callback ) );
}
