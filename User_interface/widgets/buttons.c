#include  <def_user_interface.h>

private  DEFINE_EVENT_FUNCTION( check_unpush_button )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = (button_struct  *) get_widget_pointer(widget);

    if( current_realtime_seconds() >= button->time_to_unpush )
    {
        button->polygons->colours[0] = Button_colour;
        set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                                  widget->viewport_index, NORMAL_PLANES );

        remove_global_event_callback( NO_EVENT, check_unpush_button,
                                      (void *) widget );
    }
}

private  DEFINE_EVENT_FUNCTION( push_button_event_callback )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = (widget_struct *) callback_data;
    button = (button_struct  *) get_widget_pointer(widget);

    button->polygons->colours[0] = Button_pushed_colour;
    set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );

    button->time_to_unpush = current_realtime_seconds() +
                             Interface_highlight_time;
    add_global_event_callback( NO_EVENT, check_unpush_button,
                               (void *) widget );

    button->push_callback();
}

private  void  position_button_text(
    button_struct    *button )
{
    Real   x, y, width, height;

    width = G_get_text_length( button->text->string,
                               button->text->font,
                               button->text->size );
    height = G_get_text_height( button->text->font, button->text->size );

    x = button->x + (button->x_size - width) / 2.0;
    y = button->y + (button->y_size - height) / 2.0;

    Point_x(button->text->origin) = x;
    Point_y(button->text->origin) = y;
}

private  void  position_button_rectangle(
    button_struct    *button )
{
    Point_x(button->polygons->points[0]) = button->x;
    Point_y(button->polygons->points[0]) = button->y;

    Point_x(button->polygons->points[1]) = button->x + button->x_size - 1;
    Point_y(button->polygons->points[1]) = button->y;

    Point_x(button->polygons->points[2]) = button->x + button->x_size - 1;
    Point_y(button->polygons->points[2]) = button->y + button->y_size - 1;

    Point_x(button->polygons->points[3]) = button->x;
    Point_y(button->polygons->points[3]) = button->y + button->y_size - 1;
}

public  void  position_button(
    event_viewports_struct        *event_viewports,
    int                           viewport_index,
    widget_struct                 *widget,
    int                           x,
    int                           y )
{
    button_struct   *button;

    button = (button_struct *) get_widget_pointer( widget );
    button->x = x;
    button->y = y;
    position_button_text( button );
    position_button_rectangle( button );

    set_event_viewport_callback_viewport(
               event_viewports,
               viewport_index, LEFT_MOUSE_DOWN_EVENT,
               push_button_event_callback,
               (void *) widget,
               button->x, button->x + button->x_size - 1,
               button->y, button->y + button->y_size - 1 );
}

public  void  set_button_text(
    button_struct    *button,
    char             text_string[] )
{
    (void) strcpy( button->text->string, text_string );
    position_button_text( button );
}

private  void  create_button_graphics(
    UI_struct        *ui_info,
    int              viewport_index,
    button_struct    *button,
    char             label[],
    Colour           button_colour,
    Colour           text_colour,
    Font_types       text_font,
    Real             font_size )
{
    object_struct     *object;

    object = create_rectangle( button_colour );
    button->polygons = (polygons_struct *) get_object_pointer( object );

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );

    object = create_text( text_colour, text_font, font_size );

    button->text = (text_struct *) get_object_pointer( object );

    set_button_text( button, label );

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );
}

public  void  create_button(
    UI_struct                  *ui_info,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    char                       label[],
    Colour                     button_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = create_widget( BUTTON, viewport_index );
    button = (button_struct *) get_widget_pointer( widget );

    button->x = 0;
    button->y = 0;
    button->x_size = x_size;
    button->y_size = y_size;
    button->push_callback = push_callback;

    add_event_viewport_callback( &ui_info->graphics_window.event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 push_button_event_callback,
                                 (void *) widget );

    create_button_graphics( ui_info, viewport_index, button, label,
                            button_colour, text_colour, text_font,
                            font_size );

    position_button( &ui_info->graphics_window.event_viewports,
                     viewport_index, widget, x, y );

    add_widget_to_list( &ui_info->widget_list, widget );
}
