#include  <def_user_interface.h>

private  Boolean  fit_text_within_widget(
    char         string[],
    Real         x_size,
    int          left_index,
    int          cursor_index,
    text_struct  *text,
    Real         *cursor_position )
{
    int      i, len;
    Real     width;

    i = 0;

    len = strlen( &string[left_index] );

    text->string[0] = (char) 0;
    *cursor_position = 0.0;
    width = 0.0;

    while( i < len && width <= x_size )
    {
        text->string[i] = string[left_index+i];
        text->string[i+1] = (char) 0;
        width = G_get_text_length( text->string, text->font, text->size );

        if( left_index + i + 1 == cursor_index )
            *cursor_position = width;

        ++i;
    }

    if( width > x_size )
    {
        --i;
        text->string[i] = (char) 0;
    }

    return( left_index + i >= cursor_index );
}
    

private  void      recreate_text_entry_text(
    widget_struct   *widget )
{
    text_entry_struct   *text_entry;
    Real                cursor_pos;

    text_entry = get_widget_text_entry( widget );

    if( text_entry->label_only_flag )
    {
        (void) fit_text_within_widget( text_entry->string,
                                       (Real) widget->x_size,
                                       0, 0, text_entry->text, &cursor_pos );
    }
    else
    {
        if( text_entry->string_index < text_entry->left_index )
            text_entry->left_index = text_entry->string_index;

#ifdef OLD
        while( text_entry->left_index >= 0 &&
               fit_text_within_widget( text_entry->string,
                                    (Real) widget->x_size,
                                    text_entry->left_index,
                                    text_entry->string_index, text_entry->text,
                                    &cursor_pos ) )
        {
            --text_entry->left_index;
        }

        ++text_entry->left_index;
#endif

        while( !fit_text_within_widget( text_entry->string,
                                    (Real) widget->x_size,
                                    text_entry->left_index,
                                    text_entry->string_index, text_entry->text,
                                    &cursor_pos ) )
        {
            ++text_entry->left_index;
        }

        if( get_object_visibility( text_entry->cursor ))
        {
            position_rectangle( (polygons_struct *) get_object_pointer(
                                                       text_entry->cursor),
                                widget->x + cursor_pos, widget->y,
                                Text_entry_cursor_size, widget->y_size );
        }
    }
}

private  void  insert_character_in_text_entry(
    text_entry_struct  *text_entry,
    int                key )
{
    int  i, len;

    len = strlen( text_entry->string );

    if( len < MAX_STRING_LENGTH )
    {
        for( i = len+1;  i > text_entry->string_index;  --i )
        {
            text_entry->string[i] = text_entry->string[i-1];
        }

        text_entry->string[text_entry->string_index] = (char) key;
        ++text_entry->string_index;
    }
}

private  void  delete_character_in_text_entry(
    text_entry_struct  *text_entry )
{
    int  i, len;

    if( text_entry->string_index > 0 )
    {
        len = strlen( text_entry->string );

        if( text_entry->string_index <= len )
        {
            for_less( i, text_entry->string_index-1, len )
            {
                text_entry->string[i] = text_entry->string[i+1];
            }

            --text_entry->string_index;
        }
    }
}

private  void  add_character_to_text_entry(
    text_entry_struct  *text_entry,
    int                key )
{
    switch( key )
    {
    case LEFT_ARROW_KEY:
        if( text_entry->string_index > 0 )
            --text_entry->string_index;
        break;

    case RIGHT_ARROW_KEY:
        if( text_entry->string_index < strlen(text_entry->string) )
            ++text_entry->string_index;
        break;

    case DELETE_KEY:
        delete_character_in_text_entry( text_entry );

    default:
        if( key >= ' ' && key < 127 )
            insert_character_in_text_entry( text_entry, key );
        break;
    }
}

private  void  set_text_entry_activity_colour( widget_struct * );
private  DEFINE_EVENT_FUNCTION( select_text_entry_event_callback );

private  DEFINE_EVENT_FUNCTION( key_hit_event )   /* ARGSUSED */
{
    widget_struct       *widget;
    text_entry_struct   *text_entry;

    widget = (widget_struct *) callback_data;
    text_entry = get_widget_text_entry( widget );

    if( keyboard_character == RETURN_KEY )
    {
        set_text_entry_activity_colour( widget );
        set_object_visibility( text_entry->cursor, FALSE );

        set_event_viewport_callback_enabled(
                           &get_ui_struct()->graphics_window.event_viewports,
                           widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                           select_text_entry_event_callback, (void *) widget,
                           TRUE );

        remove_global_event_callback( KEY_DOWN_EVENT, key_hit_event,
                                      (void *) widget );

        set_interaction_in_progress( FALSE );

        text_entry->hit_return_callback( widget );
    }
    else
    {
        add_character_to_text_entry( text_entry, keyboard_character );
        recreate_text_entry_text( widget );
    }

    set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );
}

private  DEFINE_EVENT_FUNCTION( select_text_entry_event_callback )/* ARGSUSED */
{
    widget_struct   *widget;
    text_entry_struct   *text_entry;

    widget = (widget_struct *) callback_data;
    text_entry = get_widget_text_entry( widget );

    (void) strcpy( text_entry->saved_string, text_entry->string );
    text_entry->string_index = strlen( text_entry->string );

    set_object_visibility( text_entry->cursor, TRUE );

    recreate_text_entry_text( widget );

    text_entry->polygons->colours[0] = get_ui_colour_index(
                                            TEXT_ENTRY_EDIT_COLOUR);;
    set_viewport_update_flag( &get_ui_struct()->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );

    set_event_viewport_callback_enabled(
                       &get_ui_struct()->graphics_window.event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       select_text_entry_event_callback, (void *) widget,
                       FALSE );

    set_interaction_in_progress( TRUE );

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

    position_text( text_entry->text, widget->x, widget->y, widget->y_size );
    position_rectangle( text_entry->polygons, widget->x, widget->y,
                        widget->x_size, widget->y_size );

    if( !text_entry->label_only_flag )
    {
        set_event_viewport_callback_viewport(
                   event_viewports,
                   viewport_index, LEFT_MOUSE_DOWN_EVENT,
                   select_text_entry_event_callback,
                   (void *) widget,
                   x, x + widget->x_size - 1,
                   y, y + widget->y_size - 1 );
    }
}

public  char  *get_text_entry_string(
    widget_struct  *widget )
{
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );
    return( text_entry->string );
}

public  void  set_text_entry_string(
    UI_struct      *ui_info,
    widget_struct  *widget,
    char           string[] )
{
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );

    (void) strncpy( text_entry->string, string, MAX_STRING_LENGTH );
    text_entry->string[MAX_STRING_LENGTH] = (char) 0;
    text_entry->left_index = 0;

    recreate_text_entry_text( widget );

    set_viewport_update_flag( &ui_info->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );
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

    if( !get_widget_text_entry(widget)->label_only_flag )
    {
        set_event_viewport_callback_enabled( event_viewports,
                           widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                           select_text_entry_event_callback, (void *) widget,
                           widget->active_flag );
    }
}

private  void  create_text_entry_graphics(
    UI_struct            *ui_info,
    int                  viewport_index,
    widget_struct        *widget,
    Colour               cursor_colour,
    Colour               text_colour,
    Font_types           text_font,
    Real                 font_size )
{
    text_entry_struct    *text_entry;
    object_struct        *object;

    text_entry = get_widget_text_entry( widget );

    /*  create background rectangle */

    object = create_rectangle( BLACK );
    text_entry->polygons = (polygons_struct *) get_object_pointer( object );
    set_text_entry_activity_colour( widget ); 

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );

    /*  create cursor rectangle */

    object = create_rectangle( cursor_colour );
    set_object_visibility( object, OFF );
    text_entry->cursor = object;

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );

    /*  create text object */

    object = create_text( text_colour, text_font, font_size );

    text_entry->text = (text_struct *) get_object_pointer( object );

    recreate_text_entry_text( widget );

    add_object_to_viewport( &ui_info->graphics_window.graphics,
                            viewport_index, NORMAL_PLANES, object );
}

private  int  create_a_text_entry(
    UI_struct                  *ui_info,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Boolean                    label_only_flag,
    char                       initial_text[],
    Boolean                    initial_activity,
    Colour                     active_colour,
    Colour                     inactive_colour,
    Colour                     cursor_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       hit_return_callback )
{
    int                 widget_index;
    widget_struct       *widget;
    text_entry_struct   *text_entry;

    widget = create_widget( TEXT_ENTRY, x, y, x_size, y_size,
                            initial_activity, active_colour, inactive_colour,
                            viewport_index );
    text_entry = get_widget_text_entry( widget );

    text_entry->label_only_flag = label_only_flag;
    text_entry->string_index = 0;
    text_entry->left_index = 0;

    (void) strcpy( text_entry->string, initial_text );

    if( !text_entry->label_only_flag )
    {
        text_entry->hit_return_callback = hit_return_callback;
        add_event_viewport_callback( &ui_info->graphics_window.event_viewports,
                                     viewport_index,
                                     LEFT_MOUSE_DOWN_EVENT,
                                     x, x + x_size - 1, y, y + y_size - 1,
                                     select_text_entry_event_callback,
                                     (void *) widget );
    }

    create_text_entry_graphics( ui_info, viewport_index, widget, cursor_colour,
                                text_colour, text_font, font_size );

    position_text_entry( widget, &ui_info->graphics_window.event_viewports,
                         viewport_index, x, y );

    widget_index = add_widget_to_list( &ui_info->widget_list[viewport_index],
                                       widget );

    return( widget_index );
}

public  int  create_text_entry(
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
    Colour                     cursor_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       hit_return_callback )
{
    return( create_a_text_entry( ui_info, viewport_index, x, y, x_size, y_size,
                         FALSE, initial_text, initial_activity, active_colour,
                         inactive_colour, cursor_colour,
                         text_colour, text_font, font_size,
                         hit_return_callback ) );
}

public  int  create_label(
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
    Real                       font_size )
{
    return( create_a_text_entry( ui_info, viewport_index, x, y, x_size, y_size,
                         TRUE, initial_text, initial_activity, active_colour,
                         inactive_colour, 0,
                         text_colour, text_font, font_size,
                         (widget_callback_type) 0 ) );
}
