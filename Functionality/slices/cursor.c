/**
 * \file Functionality/slices/cursor.c
 * \brief Draw the cursor.
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

#include  <register.h>

static  const  int  N_LINES = 12;

static  void  set_cursor_colours(
    main_struct    *main,
    lines_struct   *lines );

static  void  create_cursor_graphics(
    lines_struct   *lines )
{
    int   i;

    initialize_lines( lines, BLACK );

    REALLOC( lines->colours, N_LINES );
    lines->colour_flag = PER_ITEM_COLOURS;

    lines->colours[0] = 0;
    lines->colours[1] = 0;
    lines->colours[2] = 0;
    lines->colours[3] = 0;

    lines->colours[4] = 0;
    lines->colours[5] = 0;
    lines->colours[6] = 0;
    lines->colours[7] = 0;
    lines->colours[8] = 0;
    lines->colours[9] = 0;
    lines->colours[10] = 0;
    lines->colours[11] = 0;

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

  object_struct  *create_cursor(
    main_struct  *main,
    int          volume_index,
    int          view_index )
{
    object_struct  *object;
    lines_struct   *lines;
    
    object = create_object( LINES );

    lines = get_lines_ptr( object );

    create_cursor_graphics( lines );

    set_cursor_colours( main, lines );

    add_object_to_viewport( &main->graphics,
                            get_slice_viewport_index(volume_index,view_index),
                            NORMAL_PLANES, object );

    return( object );
}

  void  position_cursor(
    lines_struct   *lines,
    int            x,
    int            y,
    int            offset,
    int            length )
{
    fill_Point( lines->points[0], x, y + offset, 0.0 );
    fill_Point( lines->points[1], x, y + offset + length, 0.0 );

    fill_Point( lines->points[2], x, y - offset, 0.0 );
    fill_Point( lines->points[3], x, y - offset - length, 0.0 );

    fill_Point( lines->points[4], x + offset, y, 0.0 );
    fill_Point( lines->points[5], x + offset + length, y, 0.0 );

    fill_Point( lines->points[6], x - offset, y, 0.0 );
    fill_Point( lines->points[7], x - offset - length, y, 0.0 );

    fill_Point( lines->points[8], (VIO_Real) Point_x(lines->points[0]) - 1.0,
                                         Point_y(lines->points[0]), 0.0 );
    fill_Point( lines->points[9], (VIO_Real) Point_x(lines->points[1]) - 1.0,
                                         Point_y(lines->points[1]), 0.0 );

    fill_Point( lines->points[10], (VIO_Real) Point_x(lines->points[0]) + 1.0,
                                          Point_y(lines->points[0]), 0.0 );
    fill_Point( lines->points[11], (VIO_Real) Point_x(lines->points[1]) + 1.0,
                                          Point_y(lines->points[1]), 0.0 );

    fill_Point( lines->points[12], (VIO_Real) Point_x(lines->points[2]) - 1.0,
                                          Point_y(lines->points[2]), 0.0 );
    fill_Point( lines->points[13], (VIO_Real) Point_x(lines->points[3]) - 1.0,
                                          Point_y(lines->points[3]), 0.0 );

    fill_Point( lines->points[14], (VIO_Real) Point_x(lines->points[2]) + 1.0,
                                          Point_y(lines->points[2]), 0.0 );
    fill_Point( lines->points[15], (VIO_Real) Point_x(lines->points[3]) + 1.0,
                                          Point_y(lines->points[3]), 0.0 );

    fill_Point( lines->points[16],        Point_x(lines->points[4]),
                                (VIO_Real) Point_y(lines->points[4]) - 1.0, 0.0 );
    fill_Point( lines->points[17],        Point_x(lines->points[5]),
                                (VIO_Real) Point_y(lines->points[5]) - 1.0, 0.0 );

    fill_Point( lines->points[18],        Point_x(lines->points[4]),
                                (VIO_Real) Point_y(lines->points[4]) + 1.0, 0.0 );
    fill_Point( lines->points[19],        Point_x(lines->points[5]),
                                (VIO_Real) Point_y(lines->points[5]) + 1.0, 0.0 );

    fill_Point( lines->points[20],        Point_x(lines->points[6]),
                                (VIO_Real) Point_y(lines->points[6]) - 1.0, 0.0 );
    fill_Point( lines->points[21],        Point_x(lines->points[7]),
                                (VIO_Real) Point_y(lines->points[7]) - 1.0, 0.0 );

    fill_Point( lines->points[22],        Point_x(lines->points[6]),
                                (VIO_Real) Point_y(lines->points[6]) + 1.0, 0.0 );
    fill_Point( lines->points[23],        Point_x(lines->points[7]),
                                (VIO_Real) Point_y(lines->points[7]) + 1.0, 0.0 );
}

static  object_struct  *get_cursor_lines(
    main_struct    *main,
    int            volume,
    int            view )
{
    object_struct  *lines;

    if( volume == MERGED_VOLUME_INDEX )
        lines = main->merged.slices[view].cursor_lines;
    else
        lines = main->trislice[volume].slices[view].cursor_lines;

    return( lines );
}

static  void  set_cursor_colours(
    main_struct    *main,
    lines_struct   *lines )
{
    VIO_Colour         inside, outside;

    inside = Cursor_inside_colour;
    outside = Cursor_outside_colour;

    lines->colours[0] = inside;
    lines->colours[1] = inside;
    lines->colours[2] = inside;
    lines->colours[3] = inside;

    lines->colours[4] = outside;
    lines->colours[5] = outside;
    lines->colours[6] = outside;
    lines->colours[7] = outside;
    lines->colours[8] = outside;
    lines->colours[9] = outside;
    lines->colours[10] = outside;
    lines->colours[11] = outside;
}

  void  update_cursor_colours(
    main_struct    *main,
    int            volume,
    int            view )
{
    lines_struct   *lines;

    lines = get_lines_ptr( get_cursor_lines( main, volume, view ) );

    set_cursor_colours( main, lines );
}

  void  update_volume_cursor(
    main_struct   *main,
    int           volume_index,
    int           view_index )
{
    VIO_BOOL       activity;
    VIO_Real       x_pixel, y_pixel;
    VIO_Real       position[VIO_MAX_DIMENSIONS];
    object_struct  *cursor;

    cursor = get_cursor_lines( main, volume_index, view_index );

    activity = main->cursor_visibility && is_volume_active(main,volume_index);

    set_object_visibility( cursor, activity );

    if( activity )
    {
        get_volume_voxel_position( main, volume_index, position );

        convert_voxel_to_pixel( main, volume_index, view_index, position,
                                &x_pixel, &y_pixel );

        position_cursor( get_lines_ptr(cursor), VIO_ROUND(x_pixel), VIO_ROUND(y_pixel),
                         Slice_cursor_offset, Slice_cursor_size );
    }

    set_update_slice_viewport_flag( main, volume_index, view_index,
                                    NORMAL_PLANES );
}

  void  update_volume_cursors(
    main_struct   *main,
    int           volume_index )
{
    int    view_index;

    for_less( view_index, 0, N_VIEWS )
        update_volume_cursor( main, volume_index, view_index );
}

  VIO_BOOL  get_cursor_visibility(
    main_struct  *main )
{
    return( main->cursor_visibility );
}

  void  set_cursor_visibility(
    main_struct  *main,
    VIO_BOOL      state )
{
    int   volume;

    main->cursor_visibility = state;

    for_less( volume, 0, main->n_volumes_displayed )
        update_volume_cursors( main, volume );
}
