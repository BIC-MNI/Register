#include  <def_register.h>

private  void   create_tags_array(
    int                n_valid_tags,
    Boolean            valid_tags[],
    int                n_tag_points,
    tag_point_struct   tag_points[],
    int                which_volume,
    double             ***tag_array )
{
    int   i, tag_index, d;

    if( n_valid_tags > 0 )
    {
        ALLOC2D( *tag_array, n_valid_tags, N_DIMENSIONS );

        tag_index = 0;

        for_less( i, 0, n_tag_points )
        {
            if( valid_tags[i] )
            {
                for_less( d, 0, N_DIMENSIONS )
                {
                    (*tag_array)[tag_index][d] = Point_coord
                                    (tag_points[i].position[which_volume], d );
                }

                ++tag_index;
            }
        }
    }
}

private  void  create_comments(
    main_struct   *main,
    char          comments[] )
{
    char    temp[1000];

    comments[0] = (char) 0;

    if( main->trislice[0].input_flag )
    {
        (void) sprintf( comments, "Volume: %s\n",
                        get_volume_filename(main,0) );
    }

    if( main->trislice[1].input_flag )
    {
        (void) sprintf( temp, "Volume: %s\n",
                        get_volume_filename(main,1) );
        (void) strcat( comments, temp );
    }
}

public  Status   save_tag_points(
    main_struct   *main,
    char          filename[] )
{
    int              i, n_valid_tags;
    FILE             *file;
    Status           status;
    int              n_volumes, tag_index;
    double           **tags_volume1, **tags_volume2, ***ptr;
    char             **labels;
    tag_list_struct  *tags;
    Boolean          *tag_is_valid, both_volumes_flag;
    char             comments[1000];

    tags = &main->tags;

    if( tags->n_tag_points > 0 )
        ALLOC( tag_is_valid, tags->n_tag_points );

    both_volumes_flag = (main->trislice[0].input_flag &&
                         main->trislice[1].input_flag);

    if( !main->trislice[0].input_flag && !main->trislice[1].input_flag )
        return( ERROR );

    n_valid_tags = 0;

    for_less( i, 0, tags->n_tag_points )
    {
        if( !get_tag_point_activity( main, i ) )
            tag_is_valid[i] = FALSE;
        else if( both_volumes_flag )
        {
            tag_is_valid[i] = tags->tag_points[i].position_exists[0] &&
                              tags->tag_points[i].position_exists[1];
        }
        else
        {
            tag_is_valid[i] = (main->trislice[0].input_flag &&
                               tags->tag_points[i].position_exists[0] ||
                               main->trislice[1].input_flag &&
                               tags->tag_points[i].position_exists[1]);
        }

        if( tag_is_valid[i] )
            ++n_valid_tags;
    }

    n_volumes = 0;

    if( main->trislice[0].input_flag )
    {
        create_tags_array( n_valid_tags, tag_is_valid,
                           tags->n_tag_points, tags->tag_points,
                           n_volumes, &tags_volume1 );
        ++n_volumes;
    }

    if( main->trislice[1].input_flag )
    {
        if( n_volumes == 0 )
            ptr = &tags_volume1;
        else
            ptr = &tags_volume2;

        create_tags_array( n_valid_tags, tag_is_valid,
                           tags->n_tag_points, tags->tag_points,
                           n_volumes, ptr );
        ++n_volumes;
    }

    if( n_valid_tags > 0 )
        ALLOC2D( labels, n_valid_tags, MAX_STRING_LENGTH+1 );

    tag_index = 0;
    for_less( i, 0, tags->n_tag_points )
    {
        if( tag_is_valid[i] )
        {
            (void) strcpy( labels[tag_index], tags->tag_points[i].name );
            ++tag_index;
        }
    }

    status = open_file_with_default_suffix( filename, TAG_FILE_SUFFIX,
                                            WRITE_FILE, ASCII_FORMAT, &file );

    if( status == OK )
    {
        create_comments( main, comments );

        status = output_tag_points( file, comments, n_volumes,
                                    n_valid_tags,
                                    tags_volume1, tags_volume2, labels );
    }

    if( n_valid_tags > 0 )
    {
        FREE2D( tags_volume1 );
        FREE2D( labels );

        if( n_volumes == 2 )
           FREE2D( tags_volume2 );
    }

    if( status == OK )
        status = close_file( file );

    if( tags->n_tag_points > 0 )
        FREE( tag_is_valid );

    main->tags.saved_flag = TRUE;

    return( status );
}

public  Status   load_tag_points(
    main_struct   *main,
    char          filename[] )
{
    int              i, d, n_tag_points;
    FILE             *file;
    Status           file_status, status;
    int              n_volumes;
    double           **tags_volume1, **tags_volume2;
    char             **labels;
    Real             position[N_DIMENSIONS];

    file_status = open_file_with_default_suffix( filename, TAG_FILE_SUFFIX,
                                                 READ_FILE,
                                                 ASCII_FORMAT, &file );

    status = file_status;

    if( status == OK &&
        input_tag_points( file, &n_volumes, &n_tag_points, &tags_volume1,
                          &tags_volume2, &labels ) != OK )
    {
        status = ERROR;
    }

    if( status == OK )
    {
        delete_tag_points( main );
        initialize_tag_points( main );

        for_less( i, 0, n_tag_points )
        {
            create_new_tag_point( main );

            for_less( d, 0, N_DIMENSIONS )
                position[d] = tags_volume1[i][d];

            set_tag_point_position( main, i, 0, position );

            if( n_volumes == 2 )
            {
                for_less( d, 0, N_DIMENSIONS )
                    position[d] = tags_volume2[i][d];

                set_tag_point_position( main, i, 1, position );
            }

            set_tag_point_name( main, i, labels[i] );
        }

        free_tag_points( n_volumes, n_tag_points, tags_volume1, tags_volume2,
                         labels );
    }

    if( file_status == OK )
        (void) close_file( file );

    update_all_tag_objects( main );

    main->tags.saved_flag = TRUE;

    return( status );
}

public  Status   save_transform(
    main_struct   *main,
    char          filename[] )
{
    Status           status;
    Transform        *transform;
    char             comments[1000];

    status = OK;

    if( get_tag_point_transform( main, &transform, (Transform **) NULL ) )
    {
        create_comments( main, comments );

        status = write_transform_file( filename, comments, transform );
    }

    return( status );
}
