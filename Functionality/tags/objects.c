/**
 * \file Functionality/tags/objects.c
 * \brief Create and manipulate tag objects.
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

  void  create_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag )
{
    lines_struct    *lines;
    int             i, volume, view;

    for_less( volume, 0, main->n_volumes_displayed )
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

  void  update_slice_tag_colours(
    main_struct        *main,
    int                volume,
    int                view,
    tag_point_struct   *tag )
{
    lines_struct    *lines;
    VIO_Colour          inside_colour, outside_colour;

    lines = get_lines_ptr( tag->objects[volume][view] );

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

    lines->colours[0] = inside_colour;
    lines->colours[1] = outside_colour;
}

   void  update_tag_colours(
    main_struct        *main,
    tag_point_struct   *tag )
{
    int             volume, view;

    for_less( volume, 0, main->n_volumes_displayed )
    {
        for_less( view, 0, N_VIEWS )
        {
            update_slice_tag_colours( main, volume, view, tag );
        }
    }
}

   void  update_all_tag_colours(
    main_struct        *main )
{
    int  i;

    for_less( i, 0, main->tags.n_tag_points )
        update_tag_colours( main, &main->tags.tag_points[i] );
}

static  VIO_BOOL  convert_tag_to_pixel(
    main_struct       *main,
    int               volume,
    int               view,
    tag_point_struct  *tag,
    VIO_Real          *x,
    VIO_Real          *y,
    VIO_Real          *radius )
{
    VIO_BOOL visible;
    int      which_volume, axis;
    VIO_Real x_trans, y_trans, x_scale, y_scale;
    VIO_Real *cursor_ptr, separations[VIO_MAX_DIMENSIONS];
    VIO_Real diff, voxel_position[VIO_MAX_DIMENSIONS];

    visible = FALSE;

    if( volume == MERGED_VOLUME_INDEX )
        which_volume = 0;
    else
        which_volume = volume;

    if( tag->position_exists[which_volume] )
    {
        convert_original_world_to_voxel( main, which_volume,
                                 (VIO_Real) Point_x(tag->position[which_volume]),
                                 (VIO_Real) Point_y(tag->position[which_volume]),
                                 (VIO_Real) Point_z(tag->position[which_volume]),
                                 &voxel_position[VIO_X],
                                 &voxel_position[VIO_Y],
                                 &voxel_position[VIO_Z] );

        get_volume_separations( get_slice_volume(main,which_volume),
                                separations );
        get_slice_transform( main, volume, view, &x_trans, &y_trans,
                             &x_scale, &y_scale );
        axis = get_slice_axis( view );

        cursor_ptr = get_volume_cursor( main, volume );

        diff = VIO_FABS( voxel_position[axis] - cursor_ptr[axis] ) *
               x_scale * VIO_FABS( separations[axis] );

        if( diff < (VIO_Real) Tag_radius_pixels )
        {
            convert_voxel_to_pixel( main, volume, view, voxel_position, x, y );
            *radius = sqrt( (VIO_Real) Tag_radius_pixels * (VIO_Real) Tag_radius_pixels-
                            diff * diff );
            visible = TRUE;
        }
    }

    return( visible );
}

static  void  fill_in_circle_points(
    VIO_Real    x,
    VIO_Real    y,
    VIO_Real    radius,
    int     n_points,
    VIO_Point   points[] )
{
    int    i;
    VIO_Real   angle;

    for_less( i, 0, n_points )
    {
        angle = 2.0 * M_PI * (VIO_Real) i / (VIO_Real) n_points + M_PI / 4.0;

        fill_Point( points[i], x + radius * cos( (double) angle ),
                               y + radius * sin( (double) angle ),
                               0.0 );
    }
}

static  void  position_tag_circle(
    lines_struct   *lines,
    VIO_Real           x,
    VIO_Real           y,
    VIO_Real           radius )
{
    fill_in_circle_points( x + 0.5, y + 0.5, radius,
                           lines->n_points / 2, lines->points );
    fill_in_circle_points( x + 0.5, y + 0.5, radius + 1.0,
                           lines->n_points / 2,
                           &lines->points[lines->n_points / 2] );
}

  void  update_tag_object(
    main_struct       *main,
    int               volume,
    int               view,
    tag_point_struct  *tag )
{
    VIO_Real            x, y;
    VIO_Real            radius;
    VIO_BOOL         visibility;
    lines_struct    *lines;

    if( main->tags.tags_visible &&
        is_volume_active( main, volume ) &&
        convert_tag_to_pixel( main, volume, view, tag, &x, &y, &radius ) )
    {
        lines = get_lines_ptr( tag->objects[volume][view] );
        position_tag_circle( lines, x, y, radius );
        update_slice_tag_colours( main, volume, view, tag );
        visibility = TRUE;
    }
    else
    {
        visibility = FALSE;
    }

    set_object_visibility( tag->objects[volume][view], visibility );
    set_update_slice_viewport_flag( main, volume, view, NORMAL_PLANES );
}

  void  update_tag_objects(
    main_struct       *main,
    tag_point_struct  *tag )
{
    int             volume, view;

    for_less( volume, 0, main->n_volumes_displayed )
    {
        for_less( view, 0, N_VIEWS )
        {
            update_tag_object( main, volume, view, tag );
        }
    }
}

  void  update_slice_tag_objects(
    main_struct       *main,
    int               volume,
    int               view )
{
    int  i;

    for_less( i, 0, main->tags.n_tag_points )
        update_tag_object( main, volume, view, &main->tags.tag_points[i] );
}

   void  update_volume_tag_objects(
    main_struct        *main,
    int                volume )
{
    int  view;

    for_less( view, 0, N_VIEWS )
    {
        update_slice_tag_objects( main, volume, view );
    }
}

   void  update_all_tag_objects(
    main_struct        *main )
{
    int             volume;

    for_less( volume, 0, main->n_volumes_displayed )
    {
        update_volume_tag_objects( main, volume );
    }
}

  void  delete_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag )
{
    int             volume, view;

    for_less( volume, 0, main->n_volumes_displayed )
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
