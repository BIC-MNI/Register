/**
 * \file Functionality/tags/tag_transform.c
 * \brief Manipulate the transform derived from the tags.
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

  Trans_type  get_tag_transform_type(
    main_struct  *main )
{
    return( main->tags.transform_type );
}

  void  set_tag_transform_type(
    main_struct  *main,
    Trans_type   type )
{
    main->tags.transform_type = type;
    main->tags.transform_out_of_date = TRUE;
    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

static  void  recompute_tag_rms_errors(
    tag_list_struct   *tags )
{
    int    i, n_active;
    VIO_Real   dist, rms_error, x, y, z;
    VIO_Point  transformed;

    rms_error = 0.0;
    n_active = 0;

    for_less( i, 0, tags->n_tag_points )
    {
        general_transform_point( &tags->v2_to_v1_transform,
                               (VIO_Real) Point_x(tags->tag_points[i].position[1]),
                               (VIO_Real) Point_y(tags->tag_points[i].position[1]),
                               (VIO_Real) Point_z(tags->tag_points[i].position[1]),
                               &x, &y, &z );

        fill_Point( transformed, x, y, z );

        dist = distance_between_points( &transformed,
                                             &tags->tag_points[i].position[0] );

        tags->tag_points[i].rms_error = dist;

        if( tags->tag_points[i].activity )
        {
            ++n_active;
            rms_error += dist * dist;
        }
    }

    if( n_active == 0 )
        tags->avg_rms_error = 0.0;
    else
        tags->avg_rms_error = sqrt( rms_error / (VIO_Real) n_active );
}

  void  recompute_tag_transform(
    tag_list_struct   *tags )
{
    int        i, c, n_valid;
    VIO_Real       **Apoints, **Bpoints;

    if( tags->transform_exists )
        delete_general_transform( &tags->v2_to_v1_transform );

    if( tags->n_tag_points < 4 )
    {
        tags->transform_exists = FALSE;
        return;
    }

    VIO_ALLOC2D( Apoints, tags->n_tag_points, VIO_N_DIMENSIONS );
    VIO_ALLOC2D( Bpoints, tags->n_tag_points, VIO_N_DIMENSIONS );

    n_valid = 0;

    for_less( i, 0, tags->n_tag_points )
    {
        if( tags->tag_points[i].activity &&
            tags->tag_points[i].position_exists[0] &&
            tags->tag_points[i].position_exists[1] )
        {
            for_less( c, 0, VIO_N_DIMENSIONS )
            {
                Apoints[n_valid][c] = (VIO_Real) Point_coord(tags->tag_points[i]
                                                         .position[0],c);
                Bpoints[n_valid][c] = (VIO_Real) Point_coord(tags->tag_points[i]
                                                         .position[1],c);
            }
            ++n_valid;
        }
    }

    if( n_valid >= 4 )
    {
        /* Don't use safe_compute_transform_from_tags(), it isn't
         * safe. It actually triggers a crash in GLUT/X (bert).
         */
        compute_transform_from_tags( n_valid, Apoints, Bpoints,
                                     tags->transform_type,
                                     &tags->v2_to_v1_transform );
        tags->transform_exists = TRUE;
        recompute_tag_rms_errors( tags );
    }
    else
        tags->transform_exists = FALSE;

    VIO_FREE2D( Apoints );
    VIO_FREE2D( Bpoints );
}
