#include  <register.h>

private  BOOLEAN  check_update_transform_and_rms_error(
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

public  BOOLEAN  get_tags_visibility(
    main_struct      *main )
{
    return( main->tags.tags_visible );
}

public  void  set_tags_visibility(
    main_struct      *main,
    BOOLEAN          visibility )
{
    main->tags.tags_visible = visibility;
    update_all_tag_objects( main );
}

public  void  delete_tag_points(
    main_struct      *main )
{
    int   i;

    for_less( i, 0, main->tags.n_tag_points )
        delete_tag_objects( main, &main->tags.tag_points[i] );

    if( main->tags.n_tag_points > 0 )
        FREE( main->tags.tag_points );

    main->tags.transform_out_of_date = TRUE;
    main->tags.saved_flag = TRUE;
    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
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

    create_tag_objects( main, &tag );

    tag.name[0] = (char) 0;
    tag.activity = ON;

    ADD_ELEMENT_TO_ARRAY( main->tags.tag_points, main->tags.n_tag_points, tag,
                          DEFAULT_CHUNK_SIZE );
}

public  void  delete_tag_point(
    main_struct      *main,
    int              ind )
{
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

public  BOOLEAN  get_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    Real             position[] )
{
    BOOLEAN  exists;
    int      dim;

    if( ind < main->tags.n_tag_points &&
        main->tags.tag_points[ind].position_exists[volume_index] )
    {
        for_less( dim, 0, N_DIMENSIONS )
        {
            position[dim] =
            Point_coord(main->tags.tag_points[ind].position[volume_index],dim);
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

public  char  *get_tag_point_name(
    main_struct      *main,
    int              ind )
{
    char   *name;

    if( ind < main->tags.n_tag_points )
        name = main->tags.tag_points[ind].name;
    else
        name = (char *) NULL;

    return( name );
}

public  void  set_tag_point_name(
    main_struct      *main,
    int              ind,
    char             *name )
{
    if( ind < main->tags.n_tag_points )
    {
        (void) strcpy( main->tags.tag_points[ind].name, name );
        main->tags.saved_flag = FALSE;
    }
}

public  BOOLEAN  get_tag_point_avg_rms_error(
    main_struct      *main,
    Real             *error )
{
    BOOLEAN  exists;

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

public  BOOLEAN  get_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    Real             *error )
{
    BOOLEAN  exists;

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
    BOOLEAN          activity )
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

public  BOOLEAN  get_tag_point_activity(
    main_struct      *main,
    int              ind )
{
    BOOLEAN  activity;

    if( ind < main->tags.n_tag_points )
        activity = main->tags.tag_points[ind].activity;
    else
        activity = OFF;

    return( activity );
}

public  BOOLEAN  get_tag_point_transform(
    main_struct           *main,
    General_transform     **transform )
{
    BOOLEAN  exists;

    if( check_update_transform_and_rms_error( main ) )
    {
        if( transform != (General_transform **) NULL )
            *transform = &main->tags.v2_to_v1_transform;
        exists = TRUE;
    }
    else
    {
        if( transform != (General_transform **) NULL )
            *transform = (General_transform *) NULL;
        exists = FALSE;
    }

    return( exists );
}

public  BOOLEAN  get_tag_points_saved(
    main_struct   *main )
{
    return( main->tags.saved_flag );
}
