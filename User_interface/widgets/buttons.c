#include  <def_user_interface.h>

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
    button_struct                 *button,
    int                           x,
    int                           y )
{
    button->x = x;
    button->y = y;
    position_button_text( button );
    position_button_rectangle( button );

    set_event_viewport_callback_viewport(
               event_viewports,
               viewport_index, LEFT_MOUSE_DOWN_EVENT,
               button->push_button_function,
               (void *) button,
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
    event_function_type        push_button_function )
{
    widget_struct   *widget;
    button_struct   *button;

    widget = create_widget( BUTTON );
    button = (button_struct *) get_widget_pointer( widget );

    button->x = 0;
    button->y = 0;
    button->x_size = x_size;
    button->y_size = y_size;
    button->push_button_function = push_button_function;

    add_event_viewport_callback( &ui_info->graphics_window.event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 push_button_function,
                                 (void *) button );

    create_button_graphics( ui_info, viewport_index, button, label,
                            button_colour, text_colour, text_font,
                            font_size );

    position_button( &ui_info->graphics_window.event_viewports,
                     viewport_index, button, x, y );

    add_widget_to_list( &ui_info->widget_list, widget );
}
