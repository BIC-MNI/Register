/**
 * \file text_entry.c
 * \brief Implement the text entry (edit) widget.
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

#define  CONTROL_A    1
#define  CONTROL_B    2
#define  CONTROL_E    5
#define  CONTROL_F    6
#define  CONTROL_U    21

static  VIO_BOOL  fit_text_within_widget(
    VIO_STR       string,
    VIO_Real         x_size,
    int          left_index,
    int          cursor_index,
    text_struct  *text,
    VIO_Real         *cursor_position )
{
    int      i, len;
    VIO_Real     width;

    i = 0;

    replace_string( &text->string, create_string(NULL) );

    len = string_length( &string[left_index] );

    *cursor_position = 0.0;
    width = 0.0;

    while( i < len && width <= x_size )
    {
        concat_char_to_string( &text->string, string[left_index+i] );

        width = G_get_text_length( text->string, text->font, text->size );

        if( left_index + i + 1 == cursor_index )
            *cursor_position = width;

        ++i;
    }

    if( width > x_size && i > 0 )
    {
        --i;
        text->string[i] = VIO_END_OF_STRING;
    }

    return( left_index + i >= cursor_index );
}
    

static  void      recreate_text_entry_text(
    widget_struct   *widget )
{
    text_entry_struct   *text_entry;
    VIO_Real                cursor_pos;

    text_entry = get_widget_text_entry( widget );

    if( text_entry->label_only_flag )
    {
        (void) fit_text_within_widget( text_entry->string,
                                       (VIO_Real) widget->x_size,
                                       0, 0, text_entry->text, &cursor_pos );
    }
    else
    {
        if( text_entry->string_index < text_entry->left_index )
            text_entry->left_index = text_entry->string_index;

#ifdef OLD
        while( text_entry->left_index >= 0 &&
               fit_text_within_widget( text_entry->string,
                                    (VIO_Real) widget->x_size,
                                    text_entry->left_index,
                                    text_entry->string_index, text_entry->text,
                                    &cursor_pos ) )
        {
            --text_entry->left_index;
        }

        ++text_entry->left_index;
#endif

        while( !fit_text_within_widget( text_entry->string,
                                    (VIO_Real) widget->x_size,
                                    text_entry->left_index,
                                    text_entry->string_index, text_entry->text,
                                    &cursor_pos ) )
        {
            ++text_entry->left_index;
        }

        position_rectangle( get_polygons_ptr( text_entry->cursor),
                            widget->x + (int) cursor_pos, widget->y,
                            (int) Text_entry_cursor_size, widget->y_size );
    }
}

static  void  insert_character_in_text_entry(
    text_entry_struct  *text_entry,
    int                key )
{
    int     i, len;
    VIO_STR  inserted;
    
    len = string_length( text_entry->string );

    inserted = alloc_string( string_length(text_entry->string ) + 1 );

    for_less( i, 0, text_entry->string_index )
        inserted[i] = text_entry->string[i];

    for_down( i, len, text_entry->string_index )
        inserted[i+1] = text_entry->string[i];

    inserted[text_entry->string_index] = (char) key;

    replace_string( &text_entry->string, inserted );
    ++text_entry->string_index;
}

static  void  delete_character_in_text_entry(
    text_entry_struct  *text_entry )
{
    int  i, len;

    if( text_entry->string_index > 0 )
    {
        len = string_length( text_entry->string );

        if( text_entry->string_index <= len )
        {
            for_less( i, text_entry->string_index-1, len )
                text_entry->string[i] = text_entry->string[i+1];

            --text_entry->string_index;
        }
    }
}

static  void  add_character_to_text_entry(
    text_entry_struct  *text_entry,
    int                key )
{
    switch( key )
    {
    case CONTROL_A:
        if( text_entry->string_index > 0 )
            text_entry->string_index = 0;
        break;

    case CONTROL_E:
        text_entry->string_index = string_length( text_entry->string );
        break;

    case CONTROL_U:
        text_entry->string_index = 0;
        replace_string( &text_entry->string, create_string(NULL) );
        break;

    case CONTROL_B:
    case LEFT_ARROW_KEY:
        if( text_entry->string_index > 0 )
            --text_entry->string_index;
        break;

    case CONTROL_F:
    case RIGHT_ARROW_KEY:
        if( text_entry->string_index < string_length( text_entry->string ) )
            ++text_entry->string_index;
        break;

    case DELETE_KEY:
    case BACKSPACE_KEY:
        delete_character_in_text_entry( text_entry );
        break;

    default:
        if( key >= ' ' && key < 127 )
            insert_character_in_text_entry( text_entry, key );
        break;
    }
}

static  DEFINE_EVENT_FUNCTION( select_text_entry_event_callback );

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( timer_event )
{
    widget_struct       *widget;
    text_entry_struct   *text_entry;

    widget = (widget_struct *) callback_data;
    text_entry = get_widget_text_entry( widget );
    VIO_Real time_now = current_realtime_seconds() ;
    if (time_now - text_entry->blink_time >= Text_entry_blink_time)
    {
        VIO_BOOL is_visible = get_object_visibility(text_entry->cursor);
        set_object_visibility(text_entry->cursor, !is_visible);
        text_entry->blink_time = time_now;
        set_viewport_update_flag( &widget->graphics->graphics,
                                  (int) widget->viewport_index, NORMAL_PLANES );
    }
}

static  DEFINE_EVENT_FUNCTION( key_hit_event )
{
    widget_struct       *widget;
    text_entry_struct   *text_entry;

    widget = (widget_struct *) callback_data;
    text_entry = get_widget_text_entry( widget );

    if( keyboard_character == RETURN_KEY || keyboard_character == '' )
    {
        text_entry->in_edit_mode = FALSE;
        update_text_entry_colours( widget );
        set_object_visibility( text_entry->cursor, FALSE );

        set_event_viewport_callback_enabled(
                           &widget->graphics->event_viewports,
                           widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                           select_text_entry_event_callback, (void *) widget,
                           TRUE );

        remove_global_event_callback( KEY_DOWN_EVENT, key_hit_event,
                                      (void *) widget );

        remove_global_event_callback( NO_EVENT, timer_event,
                                      (void *) widget );

        set_interaction_in_progress( FALSE );

        if( keyboard_character == RETURN_KEY )
            text_entry->hit_return_callback( widget, text_entry->callback_data);
        else
            restore_text_entry_string( widget );
    }
    else
    {
        add_character_to_text_entry( text_entry, keyboard_character );
        recreate_text_entry_text( widget );
    }

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( select_text_entry_event_callback )
{
    widget_struct   *widget;
    text_entry_struct   *text_entry;

    widget = (widget_struct *) callback_data;
    text_entry = get_widget_text_entry( widget );

    text_entry->in_edit_mode = TRUE;

    replace_string( &text_entry->saved_string,
                    create_string(text_entry->string) );

    if( text_entry->clear_text_when_selected )
        replace_string( &text_entry->string, create_string(NULL) );

    text_entry->string_index = string_length( text_entry->string );

    set_object_visibility( text_entry->cursor, TRUE );

    recreate_text_entry_text( widget );

    update_text_entry_colours( widget );

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );

    set_event_viewport_callback_enabled(
                       &widget->graphics->event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       select_text_entry_event_callback, (void *) widget,
                       FALSE );

    set_interaction_in_progress( TRUE );

    text_entry->blink_time = current_realtime_seconds();

    add_global_event_callback( KEY_DOWN_EVENT, key_hit_event, ANY_MODIFIER,
                               (void *) widget );

    add_global_event_callback( NO_EVENT, timer_event, ANY_MODIFIER,
                               (void *) widget );
}

  void  position_text_entry(
    widget_struct                 *widget,
    int                           x,
    int                           y )
{
    int                 x1, x2, y1, y2;
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );

    position_text( text_entry->text, widget->x+1, widget->y, widget->y_size );
    position_rectangle( text_entry->polygons, widget->x, widget->y,
                        widget->x_size, widget->y_size );

    if( !text_entry->label_only_flag )
    {
        x1 = MAX( x, 0 );
        x2 = MAX( x + widget->x_size - 1, 0 );
        y1 = MAX( y, 0 );
        y2 = MAX( y + widget->y_size - 1, 0 );
        set_event_viewport_callback_viewport(
                   &widget->graphics->event_viewports,
                   widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                   select_text_entry_event_callback,
                   (void *) widget,
                   x1, x2, y1, y2 );
    }
}

  VIO_STR  get_text_entry_string(
    widget_struct  *widget )
{
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );
    return( text_entry->string );
}

  void  set_text_entry_string(
    widget_struct  *widget,
    VIO_STR         string )
{
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );

    replace_string( &text_entry->string, create_string(string) );

    text_entry->left_index = 0;
    text_entry->string_index = 0;

    recreate_text_entry_text( widget );

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );
}

  void  restore_text_entry_string(
    widget_struct  *widget )
{
    text_entry_struct   *text_entry;

    text_entry = get_widget_text_entry( widget );
    set_text_entry_string( widget, text_entry->saved_string );
}

  void  update_text_entry_colours(
    widget_struct  *widget )
{
    polygons_struct    *cursor_polygons;
    text_entry_struct  *text_entry;
    VIO_Colour             rectangle_colour, text_colour;

    text_entry = get_widget_text_entry( widget );

    if( widget->active_flag )
    {
        if( text_entry->in_edit_mode )
        {
            rectangle_colour = text_entry->edit_colour;
            text_colour = text_entry->text_edit_colour;
        }
        else
        {
            if( widget->selected_flag )
                rectangle_colour = text_entry->selected_colour;
            else
                rectangle_colour = text_entry->active_colour;
            text_colour = text_entry->text_colour;
        }
    }
    else
    {
        rectangle_colour = text_entry->inactive_colour;
        text_colour = text_entry->text_colour;
    }

    cursor_polygons = get_polygons_ptr( text_entry->cursor );

    if( widget->use_ui_colours )
    {
        text_entry->polygons->colours[0] = get_ui_colour( FALSE,
                                              (UI_colours) rectangle_colour );
        text_entry->text->colour = get_ui_colour( FALSE,
                                                  (UI_colours) text_colour );
        cursor_polygons->colours[0] = get_ui_colour( FALSE,
                                   (UI_colours) text_entry->cursor_colour );
    }
    else
    {
        text_entry->polygons->colours[0] = rectangle_colour;
        text_entry->text->colour = text_colour;
        cursor_polygons->colours[0] = text_entry->cursor_colour;
    }
}

  void  update_text_entry_activity(
    widget_struct           *widget )
{
    update_text_entry_colours( widget );

    if( !get_widget_text_entry(widget)->label_only_flag )
    {
        set_event_viewport_callback_enabled( &widget->graphics->event_viewports,
                           widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                           select_text_entry_event_callback, (void *) widget,
                           widget->active_flag );
    }
}

static  void  create_text_entry_graphics(
    widget_struct        *widget,
    Font_types           text_font,
    VIO_Real                 font_size )
{
    text_entry_struct    *text_entry;
    object_struct        *object;

    text_entry = get_widget_text_entry( widget );

    /*  create background rectangle */

    object = create_rectangle( BLACK );
    text_entry->polygons = get_polygons_ptr( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );

    /*  create cursor rectangle */

    object = create_rectangle( BLACK );
    set_object_visibility( object, FALSE );
    text_entry->cursor = object;

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );

    /*  create text object */

    object = create_text( BLACK, text_font, font_size );

    text_entry->text = get_text_ptr( object );

    recreate_text_entry_text( widget );

    update_text_entry_colours( widget ); 

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );
}

/* ARGSUSED */

  void  delete_text_entry(
    widget_struct  *widget )
{
    text_entry_struct     *text_entry;

    text_entry = get_widget_text_entry( widget );

    delete_string( text_entry->string );
    delete_string( text_entry->saved_string );
}

static  widget_struct  *create_a_text_entry(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_BOOL                    label_only_flag,
    VIO_BOOL                    clear_text_when_selected,
    VIO_STR                     initial_text,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    VIO_Colour                     edit_colour,
    VIO_Colour                     text_edit_colour,
    VIO_Colour                     cursor_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       hit_return_callback,
    void                       *callback_data )
{
    widget_struct       *widget;
    text_entry_struct   *text_entry;

    widget = create_widget( TEXT_ENTRY, x, y, x_size, y_size,
                            initial_activity, TRUE, graphics, viewport_index );

    text_entry = get_widget_text_entry( widget );

    text_entry->in_edit_mode = FALSE;

    text_entry->active_colour = active_colour;
    text_entry->selected_colour = selected_colour;
    text_entry->inactive_colour = inactive_colour;
    text_entry->text_colour = text_colour;
    text_entry->edit_colour = edit_colour;
    text_entry->text_edit_colour = text_edit_colour;
    text_entry->cursor_colour = cursor_colour;

    text_entry->label_only_flag = label_only_flag;
    text_entry->clear_text_when_selected = clear_text_when_selected;
    text_entry->string_index = 0;
    text_entry->left_index = 0;

    text_entry->string = create_string( initial_text );
    text_entry->saved_string = create_string( NULL );

    text_entry->blink_time = 0;

    if( !text_entry->label_only_flag )
    {
        text_entry->hit_return_callback = hit_return_callback;
        text_entry->callback_data = callback_data;

        add_event_viewport_callback( &graphics->event_viewports,
                                     viewport_index,
                                     LEFT_MOUSE_DOWN_EVENT,
                                     x, x + x_size - 1, y, y + y_size - 1,
                                     select_text_entry_event_callback,
                                     ANY_MODIFIER, (void *) widget );
    }

    create_text_entry_graphics( widget, text_font, font_size );

    update_text_entry_activity( widget );

    update_text_entry_colours( widget );

    position_text_entry( widget, x, y );

    return( widget );
}

  widget_struct  *create_text_entry(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_BOOL                    clear_text_when_selected,
    VIO_STR                     initial_text,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    VIO_Colour                     edit_colour,
    VIO_Colour                     text_edit_colour,
    VIO_Colour                     cursor_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       hit_return_callback,
    void                       *callback_data )
{
    return( create_a_text_entry( graphics, viewport_index, x, y, x_size, y_size,
                         FALSE, clear_text_when_selected,
                         initial_text, initial_activity, active_colour,
                         selected_colour, inactive_colour,
                         text_colour, edit_colour,
                         text_edit_colour, cursor_colour,
                         text_font, font_size,
                         hit_return_callback, callback_data ) );
}

  widget_struct  *create_label(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_STR                     initial_text,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size )
{
    return( create_a_text_entry( graphics, viewport_index, x, y, x_size, y_size,
                         TRUE, FALSE,
                         initial_text, initial_activity, active_colour,
                         selected_colour, inactive_colour, text_colour,
                         0, 0, 0,
                         text_font, font_size,
                         (widget_callback_type) 0, (void *) NULL ) );
}
