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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/tags/objects.c,v 1.7 1995-07-31 19:54:14 david Exp $";
#endif

#include  <register.h>

public  void  create_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag )
{
    lines_struct    *lines;
    int             i, volume, view;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            tag->objects[volume][view] = create_object( LINES );

            lines = get_lines_ptr( tag->objects[volume][view] );

            initialize_lines( lines, BLACK );

            REALLOC( lines->colours, 2 );
            lines->colour_flag = PER_ITEM_COLOURS;

            lines->n_points = 2 * Tag_circle_resolution;
            ALLOC( lines->points, lines->n_points );

            lines->n_items = 2;

            ALLOC( lines->end_indices, lines->n_items );

            lines->end_indices[0] = Tag_circle_resolution + 1;
            lines->end_indices[1] = 2 * (Tag_circle_resolution + 1);

            ALLOC( lines->indices, lines->end_indices[lines->n_items-1] );

            for_less( i, 0, Tag_circle_resolution + 1 )
            {
                lines->indices[i] = i % Tag_circle_resolution;
                lines->indices[i+Tag_circle_resolution+1] =
                         i % Tag_circle_resolution + Tag_circle_resolution;
            }

            add_object_to_viewport( &main->graphics,
                                    get_slice_viewport_index(volume,view),
                                    NORMAL_PLANES, tag->objects[volume][view] );
        }
    }

    update_tag_colours( main, tag );
    update_tag_objects( main, tag );
}

public  void  update_slice_tag_colours(
    main_struct        *main,
    int                volume,
    int                view,
    tag_point_struct   *tag )
{
    lines_struct    *lines;
    Colour          inside_colour, outside_colour;

    lines = get_lines_ptr( tag->objects[volume][view] );

    if( G_get_colour_map_state( main->window ) )
    {
        if( tag->activity )
        {
            inside_colour = main->start_colour_index+TAG_INSIDE_COLOUR;
            outside_colour = main->start_colour_index+TAG_OUTSIDE_COLOUR;
        }
        else
        {
            inside_colour = main->start_colour_index+TAG_INSIDE_INACTIVE_COLOUR;
            outside_colour = main->start_colour_index+
                                           TAG_OUTSIDE_INACTIVE_COLOUR;
        }
    }
    else
    {
        if( tag->activity )
        {
            inside_colour = Tag_inside_colour;
            outside_colour = Tag_outside_colour;
        }
        else
        {
            inside_colour = Tag_inside_inactive_colour;
            outside_colour = Tag_outside_inactive_colour;
        }
    }

    lines->colours[0] = inside_colour;
    lines->colours[1] = outside_colour;
}

public   void  update_tag_colours(
    main_struct        *main,
    tag_point_struct   *tag )
{
    int             volume, view;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            update_slice_tag_colours( main, volume, view, tag );
        }
    }
}

public   void  update_all_tag_colours(
    main_struct        *main )
{
    int  i;

    for_less( i, 0, main->tags.n_tag_points )
        update_tag_colours( main, &main->tags.tag_points[i] );
}

private  BOOLEAN  convert_tag_to_pixel(
    main_struct       *main,
    int               volume,
    int               view,
    tag_point_struct  *tag,
    Real              *x,
    Real              *y,
    Real              *radius )
{
    BOOLEAN  visible;
    int      which_volume, axis;
    Real     x_trans, y_trans, x_scale, y_scale;
    Real     *cursor_ptr, separations[MAX_DIMENSIONS];
    Real     diff, voxel_position[N_DIMENSIONS];

    visible = FALSE;

    if( volume == MERGED_VOLUME_INDEX )
        which_volume = 0;
    else
        which_volume = volume;

    if( tag->position_exists[which_volume] )
    {
        convert_original_world_to_voxel( main, which_volume,
                                         Point_x(tag->position[which_volume]),
                                         Point_y(tag->position[which_volume]),
                                         Point_z(tag->position[which_volume]),
                                         &voxel_position[X],
                                         &voxel_position[Y],
                                         &voxel_position[Z] );

        get_volume_separations( get_slice_volume(main,which_volume),
                                separations );
        get_slice_transform( main, volume, view, &x_trans, &y_trans,
                             &x_scale, &y_scale );
        axis = get_slice_axis( view );

        cursor_ptr = get_volume_cursor( main, volume );

        diff = ABS( voxel_position[axis] - cursor_ptr[axis] ) *
               x_scale * ABS( separations[axis] );

        if( diff < Tag_radius_pixels )
        {
            convert_voxel_to_pixel( main, volume, view, voxel_position, x, y );
            *radius = sqrt( Tag_radius_pixels * Tag_radius_pixels -
                            diff * diff );
            visible = TRUE;
        }
    }

    return( visible );
}

private  void  fill_in_circle_points(
    Real    x,
    Real    y,
    Real    radius,
    int     n_points,
    Point   points[] )
{
    int    i;
    Real   angle;

    for_less( i, 0, n_points )
    {
        angle = 2.0 * PI * (Real) i / (Real) n_points + PI / 4.0;

        fill_Point( points[i], x + radius * cos( (double) angle ),
                               y + radius * sin( (double) angle ),
                               0.0 );
    }
}

private  void  position_tag_circle(
    lines_struct   *lines,
    Real           x,
    Real           y,
    Real           radius )
{
    fill_in_circle_points( x + 0.5, y + 0.5, radius,
                           lines->n_points / 2, lines->points );
    fill_in_circle_points( x + 0.5, y + 0.5, radius + 1.0,
                           lines->n_points / 2,
                           &lines->points[lines->n_points / 2] );
}

public  void  update_tag_object(
    main_struct       *main,
    int               volume,
    int               view,
    tag_point_struct  *tag )
{
    Real            x, y;
    Real            radius;
    BOOLEAN         visibility;
    lines_struct    *lines;

    if( main->tags.tags_visible &&
        is_volume_active( main, volume ) &&
        convert_tag_to_pixel( main, volume, view, tag, &x, &y, &radius ) )
    {
        lines = get_lines_ptr( tag->objects[volume][view] );
        position_tag_circle( lines, x, y, radius );
        update_slice_tag_colours( main, volume, view, tag );
        visibility = ON;
    }
    else
    {
        visibility = OFF;
    }

    set_object_visibility( tag->objects[volume][view], visibility );
    set_update_slice_viewport_flag( main, volume, view, NORMAL_PLANES );
}

public  void  update_tag_objects(
    main_struct       *main,
    tag_point_struct  *tag )
{
    int             volume, view;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            update_tag_object( main, volume, view, tag );
        }
    }
}

public  void  update_slice_tag_objects(
    main_struct       *main,
    int               volume,
    int               view )
{
    int  i;

    for_less( i, 0, main->tags.n_tag_points )
        update_tag_object( main, volume, view, &main->tags.tag_points[i] );
}

public   void  update_volume_tag_objects(
    main_struct        *main,
    int                volume )
{
    int  view;

    for_less( view, 0, N_VIEWS )
    {
        update_slice_tag_objects( main, volume, view );
    }
}

public   void  update_all_tag_objects(
    main_struct        *main )
{
    int             volume;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        update_volume_tag_objects( main, volume );
    }
}

public  void  delete_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag )
{
    int             volume, view;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            remove_object_from_viewport( &main->graphics,
                                         get_slice_viewport_index(volume,view),
                                         NORMAL_PLANES,
                                         tag->objects[volume][view] );

            delete_object( tag->objects[volume][view] );
        }
    }
}
