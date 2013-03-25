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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/tags/tag_points.c,v 1.12 1998-06-29 15:01:48 david Exp $";
#endif

#include  <register.h>

private  VIO_BOOL  check_update_transform_and_rms_error(
    main_struct   *main )
{
    if( main->tags.transform_out_of_date )
    {
        recompute_tag_transform( &main->tags );
        main->tags.transform_out_of_date = FALSE;
    }

    return( main->tags.transform_exists );
}

public  void  initialize_tag_points(
    main_struct      *main )
{
    main->tags.transform_type = (Trans_type) Default_transform_type;
    main->tags.n_tag_points = 0;
    main->tags.transform_out_of_date = TRUE;
    main->tags.transform_exists = FALSE;
    main->tags.saved_flag = TRUE;
    main->tags.tags_visible = Initial_tags_visible;
}

public  VIO_BOOL  get_tags_visibility(
    main_struct      *main )
{
    return( main->tags.tags_visible );
}

public  void  set_tags_visibility(
    main_struct      *main,
    VIO_BOOL          visibility )
{
    main->tags.tags_visible = visibility;
    update_all_tag_objects( main );
}

public  void  delete_tag_points(
    main_struct      *main )
{
    int   i;

    for_less( i, 0, main->tags.n_tag_points )
    {
        delete_tag_objects( main, &main->tags.tag_points[i] );
        delete_string( main->tags.tag_points[i].name );
    }

    if( main->tags.n_tag_points > 0 )
        FREE( main->tags.tag_points );

    main->tags.transform_out_of_date = TRUE;
    main->tags.saved_flag = TRUE;
    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );

    if( main->tags.transform_exists )
        delete_general_transform( &main->tags.v2_to_v1_transform );
}

public  void  create_new_tag_point(
    main_struct      *main )
{
    int               volume_index;
    tag_point_struct  tag;

    for_less( volume_index, 0, N_VOLUMES )
    {
        tag.position_exists[volume_index] = FALSE;
        fill_Point( tag.position[volume_index], 0.0, 0.0, 0.0 );
    }

    tag.name = create_string( NULL );
    tag.activity = ON;

    create_tag_objects( main, &tag );

    ADD_ELEMENT_TO_ARRAY( main->tags.tag_points, main->tags.n_tag_points, tag,
                          DEFAULT_CHUNK_SIZE );
}

public  void  delete_tag_point(
    main_struct      *main,
    int              ind )
{
    delete_string( main->tags.tag_points[ind].name );
    delete_tag_objects( main, &main->tags.tag_points[ind] );
    DELETE_ELEMENT_FROM_ARRAY( main->tags.tag_points, main->tags.n_tag_points,
                               ind, DEFAULT_CHUNK_SIZE );
    main->tags.transform_out_of_date = TRUE;
    main->tags.saved_flag = FALSE;
    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

public  int  get_n_tag_points(
    main_struct      *main )
{
    return( main->tags.n_tag_points );
}

public  VIO_BOOL  get_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    Real             position[] )
{
    VIO_BOOL  exists;
    int      dim;

    if( ind < main->tags.n_tag_points &&
        main->tags.tag_points[ind].position_exists[volume_index] )
    {
        for_less( dim, 0, N_DIMENSIONS )
        {
            position[dim] = (Real) Point_coord(main->tags.tag_points[ind]
                                               .position[volume_index],dim);
        }
        exists = TRUE;
    }
    else
    {
        for_less( dim, 0, N_DIMENSIONS )
            position[dim] = 0.0;
        exists = FALSE;
    }

    return( exists );
}

public  void  set_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    Real             position[] )
{
    if( ind < main->tags.n_tag_points )
    {
        main->tags.tag_points[ind].position_exists[volume_index] = TRUE;
        
        fill_Point( main->tags.tag_points[ind].position[volume_index],
                    position[X], position[Y], position[Z] );

        update_tag_objects( main, &main->tags.tag_points[ind] );

        main->tags.transform_out_of_date = TRUE;
        main->tags.saved_flag = FALSE;
        set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
    }
}

public  STRING  get_tag_point_name(
    main_struct      *main,
    int              ind )
{
    STRING   name;

    if( ind < main->tags.n_tag_points )
        name = main->tags.tag_points[ind].name;
    else
        name = NULL;

    return( name );
}

public  void  set_tag_point_name(
    main_struct      *main,
    int              ind,
    STRING           name )
{
    if( ind < main->tags.n_tag_points )
    {
        replace_string( &main->tags.tag_points[ind].name, create_string(name) );
        main->tags.saved_flag = FALSE;
    }
}

public  VIO_BOOL  get_tag_point_avg_rms_error(
    main_struct      *main,
    Real             *error )
{
    VIO_BOOL  exists;

    if( check_update_transform_and_rms_error( main ) )
    {
        *error = main->tags.avg_rms_error;
        exists = TRUE;
    }
    else
    {
        *error = 0.0;
        exists = FALSE;
    }

    return( exists );
}

public  VIO_BOOL  get_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    Real             *error )
{
    VIO_BOOL  exists;

    if( ind < main->tags.n_tag_points &&
        check_update_transform_and_rms_error( main ) )
    {
        *error = main->tags.tag_points[ind].rms_error;
        exists = TRUE;
    }
    else
    {
        *error = 0.0;
        exists = FALSE;
    }

    return( exists );
}

public  void  set_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    Real             rms_error )
{
    if( ind < main->tags.n_tag_points )
        main->tags.tag_points[ind].rms_error = rms_error;
}

public  void  set_tag_point_activity(
    main_struct      *main,
    int              ind,
    VIO_BOOL          activity )
{
    if( ind < main->tags.n_tag_points )
    {
        main->tags.tag_points[ind].activity = activity;
        main->tags.transform_out_of_date = TRUE;
        main->tags.saved_flag = FALSE;
        update_tag_objects( main, &main->tags.tag_points[ind] );
        set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
    }
}

public  VIO_BOOL  get_tag_point_activity(
    main_struct      *main,
    int              ind )
{
    VIO_BOOL  activity;

    if( ind < main->tags.n_tag_points )
        activity = main->tags.tag_points[ind].activity;
    else
        activity = OFF;

    return( activity );
}

public  VIO_BOOL  get_tag_point_transform(
    main_struct           *main,
    VIO_General_transform     **transform )
{
    VIO_BOOL  exists;

    if( check_update_transform_and_rms_error( main ) )
    {
        if( transform != (VIO_General_transform **) NULL )
            *transform = &main->tags.v2_to_v1_transform;
        exists = TRUE;
    }
    else
    {
        if( transform != (VIO_General_transform **) NULL )
            *transform = (VIO_General_transform *) NULL;
        exists = FALSE;
    }

    return( exists );
}

public  VIO_BOOL  get_tag_points_saved(
    main_struct   *main )
{
    return( main->tags.saved_flag );
}
