#include  <def_user_interface.h>

typedef  struct
{
    graphics_window_struct  graphics;
    widget_struct           *widget;
    Real                    expiration_time;
} message_struct;

static  char     *output;
static  int      n_chars = 0;
static  Boolean  create_new_window = TRUE;

private  void  output_chars( char [] );
private  void  create_message_popup( char [] );
private  DEFINE_EVENT_FUNCTION( check_to_expire_popup );

public  void  initialize_print_popup()
{
    set_print_function( output_chars );
}

private  DEFINE_EVENT_FUNCTION( create_the_window )    /* ARGSUSED */
{
    create_new_window = TRUE;
    remove_global_event_callback( NO_EVENT, create_the_window,
                                  (void *) NULL );

    create_message_popup( output );

    SET_ARRAY_SIZE( output, n_chars, 0, DEFAULT_CHUNK_SIZE );
    n_chars = 0;
}

private  void  output_chars(
    char   str[] )
{
    int   new_size;

    if( create_new_window )
    {
        create_new_window = FALSE;
        add_global_event_callback( NO_EVENT, create_the_window,
                                   ANY_MODIFIER, (void *) NULL );
    }

    new_size = strlen( str ) + n_chars + 1;

    SET_ARRAY_SIZE( output, n_chars, new_size, DEFAULT_CHUNK_SIZE );

    (void) strcpy( &output[n_chars], str );

    n_chars = new_size - 1;
}

private  void  delete_message_popup(
    message_struct          *popup )
{
    delete_widget( popup->widget );

    remove_global_event_callback( NO_EVENT, check_to_expire_popup,
                                  (void *) popup );

    delete_popup_window( &popup->graphics );

    FREE( popup );
}

private  DEFINE_WIDGET_CALLBACK( acknowledge_callback )   /* ARGSUSED */
{
    message_struct   *popup;

    popup = (message_struct *) callback_data;
    delete_message_popup( popup );
}

private  DEFINE_EVENT_FUNCTION( check_to_expire_popup )   /* ARGSUSED */
{
    message_struct   *popup;

    popup = (message_struct *) callback_data;

    if( current_realtime_seconds() > popup->expiration_time )
    {
        delete_message_popup( popup );
    }
}

private  void  create_message_popup(
    char   string[] )
{
    int                     i, n_lines, x, y, x_size, y_size;
    Real                    max_length, graphics_length;
    Point                   point;
    char                    *ptr;
    object_struct           *object;
    object_struct           **text_objects;
    text_struct             *text;
    message_struct          *popup;   

    ALLOC( popup, 1 );

    fill_Point( point, Message_x_offset, 0.0, 0.0 );

    n_lines = 0;
    ptr = string;
    max_length = 0.0;

    while( *ptr != (char) 0 )
    {
        object = create_object( TEXT );
        text = (text_struct *) get_object_pointer( object );
        initialize_text( text, &point, Message_text_colour,
                         Message_font, Message_font_size );
        ADD_ELEMENT_TO_ARRAY( text_objects, n_lines, object,
                              DEFAULT_CHUNK_SIZE );

        i = 0;
        while( *ptr != (char) 0 && *ptr != '\n' && i < MAX_STRING_LENGTH )
        {
            text->string[i] = *ptr;
            ++i;
            ++ptr;
        }
        if( *ptr == '\n' )
            ++ptr;
        text->string[i] = (char) 0;

        graphics_length = G_get_text_length( text->string, text->font,
                                             text->size );
        if( graphics_length > max_length )
            max_length = graphics_length;
    }

    G_get_mouse_screen_position( &x, &y );

    x_size = graphics_length + 2 * Message_x_offset;
    y_size = 2 * Message_y_offset + n_lines * Message_text_y_offset +
             Message_ok_button_height;

    create_popup_window( &popup->graphics, "Register Message",
                         x, y, x_size, y_size );

    popup->expiration_time = current_realtime_seconds() +
                             Message_expiration_time;

    for_less( i, 0, n_lines )
    {
        text = (text_struct *) get_object_pointer( text_objects[i] );
        Point_y( text->origin ) = y_size-1-Message_y_offset -
                                    i * Message_text_y_offset;
        
        add_object_to_viewport( &popup->graphics.graphics, 0,
                                NORMAL_PLANES, text_objects[i] );
    }

    if( n_lines > 0 )
    {
        FREE( text_objects );
    }

    popup->widget = create_button( &popup->graphics, 0,
                            Interface_x_spacing, Interface_y_spacing,
                            Button_width, Button_height,
                            "Acknowledge",
                            ON, TRUE, BUTTON_ACTIVE_COLOUR,
                            BUTTON_SELECTED_COLOUR,
                            BUTTON_INACTIVE_COLOUR,
                            BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                            Button_text_font, Button_text_font_size,
                            acknowledge_callback, (void *) popup );

    add_global_event_callback( NO_EVENT, check_to_expire_popup,
                               ANY_MODIFIER, (void *) popup );
}
