#include  <def_register.h>

private  const  int  N_LINES = 12;

private  const  Bitplane_types  CURSOR_BITPLANE = OVERLAY_PLANES;

private  void  create_cursor_graphics(
    lines_struct   *lines,
    Colour         inside_colour,
    Colour         outside_colour )
{
    int   i;

    initialize_lines( lines, BLACK );

    REALLOC( lines->colours, N_LINES );
    lines->colour_flag = PER_ITEM_COLOURS;

    lines->colours[0] = inside_colour;
    lines->colours[1] = inside_colour;
    lines->colours[2] = inside_colour;
    lines->colours[3] = inside_colour;

    lines->colours[4] = outside_colour;
    lines->colours[5] = outside_colour;
    lines->colours[6] = outside_colour;
    lines->colours[7] = outside_colour;
    lines->colours[8] = outside_colour;
    lines->colours[9] = outside_colour;
    lines->colours[10] = outside_colour;
    lines->colours[11] = outside_colour;

    lines->n_points = 2 * N_LINES;

    lines->n_items = N_LINES;

    ALLOC( lines->points, lines->n_points );

    ALLOC( lines->end_indices, lines->n_items );
    ALLOC( lines->indices, lines->n_points );

    for_less( i, 0, lines->n_items )
        lines->end_indices[i] = 2 * (i+1);

    for_less( i, 0, lines->n_points )
    {
        fill_Point( lines->points[i], 0.0, 0.0, 0.0 );
        lines->indices[i] = i;
    }

    position_cursor( lines, 0, 0, 0, 0 );
}

public  lines_struct  *create_cursor(
    main_struct  *main,
    int          volume_index,
    int          view_index )
{
    object_struct  *object;
    lines_struct   *lines;

    
    object = create_object( LINES );

    lines = get_lines_ptr( object );

    create_cursor_graphics( lines, Slice_cursor_inside_colour,
                            Slice_cursor_outside_colour );

    add_object_to_viewport( &main->graphics,
                            get_slice_viewport_index(volume_index,view_index),
                            CURSOR_BITPLANE, object );

    return( lines );
}

public  void  position_cursor(
    lines_struct   *lines,
    int            x,
    int            y,
    int            offset,
    int            length )
{
    Point_x(lines->points[0]) = x;
    Point_y(lines->points[0]) = y + offset;
    Point_x(lines->points[1]) = x;
    Point_y(lines->points[1]) = y + offset + length;

    Point_x(lines->points[2]) = x;
    Point_y(lines->points[2]) = y - offset;
    Point_x(lines->points[3]) = x;
    Point_y(lines->points[3]) = y - offset - length;

    Point_x(lines->points[4]) = x + offset;
    Point_y(lines->points[4]) = y;
    Point_x(lines->points[5]) = x + offset + length;
    Point_y(lines->points[5]) = y;

    Point_x(lines->points[6]) = x - offset;
    Point_y(lines->points[6]) = y;
    Point_x(lines->points[7]) = x - offset - length;
    Point_y(lines->points[7]) = y;

    Point_x(lines->points[8]) = Point_x(lines->points[0]) - 1;
    Point_y(lines->points[8]) = Point_y(lines->points[0]);
    Point_x(lines->points[9]) = Point_x(lines->points[1]) - 1;
    Point_y(lines->points[9]) = Point_y(lines->points[1]);

    Point_x(lines->points[10]) = Point_x(lines->points[0]) + 1;
    Point_y(lines->points[10]) = Point_y(lines->points[0]);
    Point_x(lines->points[11]) = Point_x(lines->points[1]) + 1;
    Point_y(lines->points[11]) = Point_y(lines->points[1]);

    Point_x(lines->points[12]) = Point_x(lines->points[2]) - 1;
    Point_y(lines->points[12]) = Point_y(lines->points[2]);
    Point_x(lines->points[13]) = Point_x(lines->points[3]) - 1;
    Point_y(lines->points[13]) = Point_y(lines->points[3]);

    Point_x(lines->points[14]) = Point_x(lines->points[2]) + 1;
    Point_y(lines->points[14]) = Point_y(lines->points[2]);
    Point_x(lines->points[15]) = Point_x(lines->points[3]) + 1;
    Point_y(lines->points[15]) = Point_y(lines->points[3]);

    Point_x(lines->points[16]) = Point_x(lines->points[4]);
    Point_y(lines->points[16]) = Point_y(lines->points[4]) - 1;
    Point_x(lines->points[17]) = Point_x(lines->points[5]);
    Point_y(lines->points[17]) = Point_y(lines->points[5]) - 1;

    Point_x(lines->points[18]) = Point_x(lines->points[4]);
    Point_y(lines->points[18]) = Point_y(lines->points[4]) + 1;
    Point_x(lines->points[19]) = Point_x(lines->points[5]);
    Point_y(lines->points[19]) = Point_y(lines->points[5]) + 1;

    Point_x(lines->points[20]) = Point_x(lines->points[6]);
    Point_y(lines->points[20]) = Point_y(lines->points[6]) - 1;
    Point_x(lines->points[21]) = Point_x(lines->points[7]);
    Point_y(lines->points[21]) = Point_y(lines->points[7]) - 1;

    Point_x(lines->points[22]) = Point_x(lines->points[6]);
    Point_y(lines->points[22]) = Point_y(lines->points[6]) + 1;
    Point_x(lines->points[23]) = Point_x(lines->points[7]);
    Point_y(lines->points[23]) = Point_y(lines->points[7]) + 1;
}

public  void  update_volume_cursor(
    main_struct   *main,
    int           volume_index,
    int           view_index )
{
    Real          x_pixel, y_pixel;
    Real          position[N_DIMENSIONS];
    lines_struct  *lines;

    get_volume_voxel_position( main, volume_index, position );

    convert_voxel_to_pixel( main, volume_index, view_index, position,
                            &x_pixel, &y_pixel );

    if( volume_index == MERGED_VOLUME_INDEX )
        lines = main->merged.slices[view_index].cursor_lines;
    else
        lines = main->trislice[volume_index].slices[view_index].cursor_lines;

    position_cursor( lines, ROUND(x_pixel), ROUND(y_pixel),
                     Slice_cursor_offset, Slice_cursor_size );

    set_update_slice_viewport_flag( main, volume_index, view_index,
                                    CURSOR_BITPLANE );
}

public  void  update_volume_cursors(
    main_struct   *main,
    int           volume_index )
{
    int    view_index;

    for_less( view_index, 0, N_VIEWS )
        update_volume_cursor( main, volume_index, view_index );
}
