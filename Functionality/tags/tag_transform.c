#include  <register.h>

public  Trans_type  get_tag_transform_type(
    main_struct  *main )
{
    return( main->tags.transform_type );
}

public  void  set_tag_transform_type(
    main_struct  *main,
    Trans_type   type )
{
    main->tags.transform_type = type;
    main->tags.transform_out_of_date = TRUE;
    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

private  void  recompute_tag_rms_errors(
    tag_list_struct   *tags )
{
    int    i, n_active;
    Real   dist, rms_error, x, y, z;
    Point  transformed;

    rms_error = 0.0;
    n_active = 0;

    for_less( i, 0, tags->n_tag_points )
    {
        general_transform_point( &tags->v2_to_v1_transform,
                                 Point_x(tags->tag_points[i].position[1]),
                                 Point_y(tags->tag_points[i].position[1]),
                                 Point_z(tags->tag_points[i].position[1]),
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
        tags->avg_rms_error = sqrt( rms_error / (Real) n_active );
}

public  void  recompute_tag_transform(
    tag_list_struct   *tags )
{
    int        i, c, n_valid;
    Real       **Apoints, **Bpoints;

    if( tags->transform_exists )
        delete_general_transform( &tags->v2_to_v1_transform );

    if( tags->n_tag_points < 4 )
    {
        tags->transform_exists = FALSE;
        return;
    }
    
    ALLOC2D( Apoints, tags->n_tag_points, N_DIMENSIONS );
    ALLOC2D( Bpoints, tags->n_tag_points, N_DIMENSIONS );

    n_valid = 0;

    for_less( i, 0, tags->n_tag_points )
    {
        if( tags->tag_points[i].activity &&
            tags->tag_points[i].position_exists[0] &&
            tags->tag_points[i].position_exists[1] )
        {
            for_less( c, 0, N_DIMENSIONS )
            {
                Apoints[n_valid][c] =
                    Point_coord(tags->tag_points[i].position[0],c);
                Bpoints[n_valid][c] =
                    Point_coord(tags->tag_points[i].position[1],c);
            }
            ++n_valid;
        }
    }

    if( n_valid >= 4 )
    {
        safe_compute_transform_from_tags( n_valid, Apoints, Bpoints,
                                          tags->transform_type,
                                          &tags->v2_to_v1_transform );
        tags->transform_exists = TRUE;
        recompute_tag_rms_errors( tags );
    }
    else
        tags->transform_exists = FALSE;

    FREE2D( Apoints );
    FREE2D( Bpoints );
}
