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

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/widgets/utilities.c,v 1.7 1995-10-02 18:35:03 david Exp $";
#endif

#include  <user_interface.h>

public  object_struct  *create_rectangle(
    Colour       colour )
{
    Point             point;
    object_struct     *object;
    polygons_struct   *polygons;

    object = create_object( POLYGONS );
    polygons = get_polygons_ptr( object );

    initialize_polygons( polygons, colour, (Surfprop *) 0 );

    fill_Point( point, 0.0, 0.0, 0.0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );

    return( object );
}

public  void  position_rectangle(
    polygons_struct    *polygons,
    int                x,
    int                y,
    int                x_size,
    int                y_size )
{
    Point_x(polygons->points[0]) = x;
    Point_y(polygons->points[0]) = y;

    Point_x(polygons->points[1]) = x + x_size - 1;
    Point_y(polygons->points[1]) = y;

    Point_x(polygons->points[2]) = x + x_size - 1;
    Point_y(polygons->points[2]) = y + y_size - 1;

    Point_x(polygons->points[3]) = x;
    Point_y(polygons->points[3]) = y + y_size - 1;
}

public  object_struct  *create_text(
    Colour           colour,
    Font_types       text_font,
    Real             font_size )
{
    Point             point;
    object_struct     *object;
    text_struct       *text;

    object = create_object( TEXT );
    text = get_text_ptr( object );

    fill_Point( point, 0.0, 0.0, 0.0 );

    initialize_text( text, &point, colour,
                     text_font, font_size );

    return( object );
}

public  void  position_text(
    text_struct   *text,
    int           x,
    int           y,
    int           y_size )
{
    Real            height;

    height = G_get_text_height( text->font, text->size );

    x = x;
    y = y + (y_size - height) / 2.0;

    Point_x(text->origin) = x;
    Point_y(text->origin) = y;
}

public  void  position_text_centred(
    text_struct    *text,
    int            x,
    int            y,
    int            x_size,
    int            y_size )
{
    Real            width, height;

    width = G_get_text_length( text->string, text->font, text->size );
    height = G_get_text_height( text->font, text->size );

    x = x + (x_size - width) / 2.0;
    y = y + (y_size - height) / 2.0;

    Point_x(text->origin) = x;
    Point_y(text->origin) = y;
}

public  BOOLEAN  get_toggle_button_state(
    widget_struct  *widget )
{
    BOOLEAN        state;
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

public  BOOLEAN  get_text_entry_real_value(
    widget_struct  *widget,
    Real           *value )
{
    BOOLEAN            found;
    double             double_value;

    found = FALSE;

    if( sscanf( get_text_entry_string(widget), "%lf", &double_value ) == 1 )
    {
        *value = (Real) double_value;
        found = TRUE;
    }
    else
    {
        restore_text_entry_string( widget );
    }

    return( found );
}

public  void  set_text_entry_real_value(
    widget_struct     *widget,
    STRING            format,
    Real              value )
{
    char          buffer[EXTREMELY_LARGE_STRING_SIZE];

    (void) sprintf( buffer, format, value );

    set_text_entry_string( widget, buffer );
}

public  void  set_button_int_value(
    widget_struct     *widget,
    STRING            format,
    int               value )
{
    char          buffer[EXTREMELY_LARGE_STRING_SIZE];

    (void) sprintf( buffer, format, value );

    set_button_text( widget, buffer );
}
