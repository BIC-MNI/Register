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

#include  <user_interface.h>

  object_struct  *create_rectangle(
    VIO_Colour       colour )
{
    VIO_Point             point;
    object_struct     *object;
    polygons_struct   *polygons;

    object = create_object( POLYGONS );
    polygons = get_polygons_ptr( object );

    initialize_polygons( polygons, colour, (VIO_Surfprop *) 0 );

    fill_Point( point, 0.0, 0.0, 0.0 );
    add_point_to_polygon( polygons, &point, (VIO_Vector *) 0 );
    add_point_to_polygon( polygons, &point, (VIO_Vector *) 0 );
    add_point_to_polygon( polygons, &point, (VIO_Vector *) 0 );
    add_point_to_polygon( polygons, &point, (VIO_Vector *) 0 );

    return( object );
}

  void  position_rectangle(
    polygons_struct    *polygons,
    int                x,
    int                y,
    int                x_size,
    int                y_size )
{
    fill_Point( polygons->points[0], x, y, 0.0 );
    fill_Point( polygons->points[1], x + x_size - 1, y, 0.0 );
    fill_Point( polygons->points[2], x + x_size - 1, y + y_size - 1, 0.0 );
    fill_Point( polygons->points[3], x, y + y_size - 1, 0.0 );
}

  object_struct  *create_text(
    VIO_Colour           colour,
    Font_types       text_font,
    VIO_Real             font_size )
{
    VIO_Point             point;
    object_struct     *object;
    text_struct       *text;

    object = create_object( TEXT );
    text = get_text_ptr( object );

    fill_Point( point, 0.0, 0.0, 0.0 );

    initialize_text( text, &point, colour,
                     text_font, font_size );

    return( object );
}

  void  position_text(
    text_struct   *text,
    int           x,
    int           y,
    int           y_size )
{
    VIO_Real            height;

    height = G_get_text_height( text->font, text->size );

    x = x;
    y = y + VIO_ROUND( ((VIO_Real) y_size - height) / 2.0 );

    fill_Point( text->origin, x, y, 0.0 );
}

  void  position_text_centred(
    text_struct    *text,
    int            x,
    int            y,
    int            x_size,
    int            y_size )
{
    VIO_Real            width, height;

    width = G_get_text_length( text->string, text->font, text->size );
    height = G_get_text_height( text->font, text->size );

    fill_Point( text->origin, (VIO_Real) x + ((VIO_Real) x_size - width) / 2.0,
                              (VIO_Real) y + ((VIO_Real) y_size - height) / 2.0, 0 );
}

  VIO_BOOL  get_toggle_button_state(
    widget_struct  *widget )
{
    VIO_BOOL        state;
    button_struct  *button;

    state = FALSE;

    if( widget->widget_type == BUTTON )
    {
        button = get_widget_button( widget );

        if( button->toggle_flag )
            state = button->state;
        else
        {
            HANDLE_INTERNAL_ERROR( "get_toggle_button_state" );
        }
    }
    else
    {
        HANDLE_INTERNAL_ERROR( "get_toggle_button_state" );
    }

    return( state );
}

  VIO_BOOL  get_text_entry_real_value(
    widget_struct  *widget,
    VIO_Real           *value )
{
    VIO_BOOL            found;
    double             double_value;

    found = FALSE;

    if( sscanf( get_text_entry_string(widget), "%lf", &double_value ) == 1 )
    {
        *value = (VIO_Real) double_value;
        found = TRUE;
    }
    else
    {
        restore_text_entry_string( widget );
    }

    return( found );
}

  void  set_text_entry_real_value(
    widget_struct     *widget,
    VIO_STR            format,
    VIO_Real              value )
{
    char          buffer[VIO_EXTREMELY_LARGE_STRING_SIZE];

    (void) sprintf( buffer, format, value );

    set_text_entry_string( widget, buffer );
}

  void  set_button_int_value(
    widget_struct     *widget,
    VIO_STR            format,
    int               value )
{
    char          buffer[VIO_EXTREMELY_LARGE_STRING_SIZE];

    (void) sprintf( buffer, format, value );

    set_button_text( widget, buffer );
}
