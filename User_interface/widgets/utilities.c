#include  <def_user_interface.h>

public  object_struct  *create_rectangle(
    Colour           colour )
{
    Point             point;
    object_struct     *object;
    polygons_struct   *polygons;

    object = create_object( POLYGONS );
    polygons = (polygons_struct *) get_object_pointer( object );

    initialize_polygons( polygons, colour, (Surfprop *) 0 );

    fill_Point( point, 0.0, 0.0, 0.0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );
    add_point_to_polygon( polygons, &point, (Vector *) 0 );

    return( object );
}

public  object_struct  *create_text(
    Colour           colour,
    Font_types       font,
    Real             font_size )
{
    Point             point;
    object_struct     *object;
    text_struct       *text;

    object = create_object( TEXT );
    text = (text_struct *) get_object_pointer( object );

    fill_Point( point, 0.0, 0.0, 0.0 );

    initialize_text( text, &point, colour, font, font_size );

    return( object );
}
