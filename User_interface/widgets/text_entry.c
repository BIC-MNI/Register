#include  <def_user_interface.h>

private  void  set_text_entry_activity_colour( widget_struct * );
private  DEFINE_EVENT_FUNCTION( select_text_entry_event_callback );

private  DEFINE_EVENT_FUNCTION( key_hit_event )   /* ARGSUSED */
{
    widget_struct   *widget;
    text_entry_struct   *text_entry;

    print( "Key hit = <%c>\n", keyboard_character );

    if( keyboard_character == RETURN_KEY )
    {
        widget = (widget_struct *) callback_data;
        text_entry = get_widget_text_entry( widget );

        set_text_entry_activity_colour( widget );
        set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                                  widget->viewport_index, NORMAL_PLANES );

        set_event_viewport_callback_enabled(
                           &get_ui_struct()->graphics_window.event_viewports,
                           widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                           select_text_entry_event_callback, (void *) widget,
                           TRUE );

        remove_global_event_callback( KEY_DOWN_EVENT, key_hit_event,
                                      (void *) widget );

        text_entry->hit_return_callback( FALSE );
    }
}

private  DEFINE_EVENT_FUNCTION( select_text_entry_event_callback )/* ARGSUSED */
{
    widget_struct   *widget;
    text_entry_struct   *text_entry;

    widget = (widget_struct *) callback_data;
    text_entry = get_widget_text_entry( widget );

    text_entry->polygons->colours[0] = Text_entry_edit_colour;
    set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );

    set_event_viewport_callback_enabled(
                       &get_ui_struct()->graphics_window.event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       select_text_entry_event_callback, (void *) widget,
                       FALSE );

    add_global_event_callback( KEY_DOWN_EVENT, key_hit_event,
                               (void *) widget );
}

public  void  position_text_entry(
    widget_struct                 *widget,
    event_viewports_struct        *event_viewports,
    int                           viewport_index,
    int                           x,
    int                           y )
{
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );

    position_text( text_entry->text, widget->x, widget->y );
    position_rectangle( text_entry->polygons, widget->x, widget->y,
                        widget->x_size, widget->y_size );

    set_event_viewport_callback_viewport(
               event_viewports,
               viewport_index, LEFT_MOUSE_DOWN_EVENT,
               select_text_entry_event_callback,
               (void *) widget,
               x, x + widget->x_size - 1,
               y, y + widget->y_size - 1 );
}

private  void  set_text_entry_activity_colour(
    widget_struct  *widget )
{
    get_widget_text_entry(widget)->polygons->colours[0] =
                                       get_widget_colour(widget);
}

public  void  update_text_entry_activity(
    widget_struct           *widget,
    event_viewports_struct  *event_viewports )
{
    set_text_entry_activity_colour( widget );

    set_event_viewport_callback_enabled( event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       select_text_entry_event_callback, (void *) widget,
                       widget->active_flag );
}

private  void  create_text_entry_graphics(
    UI_struct            *ui_info,
    int                  viewport_index,
    widget_struct        *widget,
    Colour               text_colour,
    Font_types           text_font,
    Real                 font_size )
{
    text_entry_struct    *text_entry;
    object_struct        *object;

    text_entry = get_widget_text_entry( widget );

    object = create_rectangle( BLACK );
    text_entry->polygons = (polygons_struct *) get_object_pointer( object );
    set_text_entry_activity_colour( widget ); 

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );

    object = create_text( text_colour, text_font, font_size );

    text_entry->text = (text_struct *) get_object_pointer( object );

    (void) strcpy( text_entry->text->string, text_entry->string );

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );
}

public  void  create_text_entry(
    UI_struct                  *ui_info,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    char                       initial_text[],
    Boolean                    initial_activity,
    Colour                     active_colour,
    Colour                     inactive_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       hit_return_callback )
{
    widget_struct       *widget;
    text_entry_struct   *text_entry;

    widget = create_widget( TEXT_ENTRY, x, y, x_size, y_size,
                            initial_activity, active_colour, inactive_colour,
                            viewport_index );
    text_entry = get_widget_text_entry( widget );

    text_entry->hit_return_callback = hit_return_callback;
    (void) strcpy( text_entry->string, initial_text );

    add_event_viewport_callback( &ui_info->graphics_window.event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 select_text_entry_event_callback,
                                 (void *) widget );

    create_text_entry_graphics( ui_info, viewport_index, widget,
                                text_colour, text_font, font_size );

    position_text_entry( widget, &ui_info->graphics_window.event_viewports,
                         viewport_index, x, y );

    add_widget_to_list( &ui_info->widget_list, widget );
}
