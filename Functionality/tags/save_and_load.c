#include  <def_register.h>

private  void   create_tags_array(
    int                n_tag_points,
    tag_point_struct   tag_points[],
    int                which_volume,
    double             ***tag_array )
{
    int   i, d;

    ALLOC2D( *tag_array, n_tag_points, N_DIMENSIONS );

    for_less( i, 0, n_tag_points )
    {
        for_less( d, 0, N_DIMENSIONS )
        {
            (*tag_array)[i][d] = Point_coord
                                    (tag_points[i].position[which_volume], d );
        }
    }
}

public  Status   save_tag_points(
    main_struct   *main,
    char          filename[] )
{
    int              i;
    FILE             *file;
    Status           status;
    int              n_volumes;
    double           **tags_volume1, **tags_volume2, ***ptr;
    char             **labels;
    tag_list_struct  *tags;

    tags = &main->tags;

    n_volumes = 0;

    if( main->trislice[0].input_flag )
    {
        create_tags_array( tags->n_tag_points, tags->tag_points,
                           n_volumes, &tags_volume1 );
        ++n_volumes;
    }

    if( main->trislice[1].input_flag )
    {
        if( n_volumes == 0 )
            ptr = &tags_volume1;
        else
            ptr = &tags_volume2;

        create_tags_array( tags->n_tag_points, tags->tag_points,
                           n_volumes, ptr );
        ++n_volumes;
    }

    if( n_volumes == 0 )
        return( ERROR );

    ALLOC2D( labels, tags->n_tag_points, MAX_STRING_LENGTH+1 );

    for_less( i, 0, tags->n_tag_points )
    {
        (void) strcpy( labels[i], tags->tag_points[i].name );
    }

    status = open_file_with_default_suffix( filename, "tag", WRITE_FILE,
                                            ASCII_FORMAT, &file );

    if( status == OK )
    {
        (void) output_tag_points( file, n_volumes, tags->n_tag_points,
                                  tags_volume1, tags_volume2, labels );
    }

    FREE2D( tags_volume1 );
    FREE2D( labels );

    if( n_volumes == 2 )
       FREE2D( tags_volume2 );

    if( status == OK )
        status = close_file( file );

    return( status );
}

public  Status   load_tag_points(
    main_struct   *main,
    char          filename[] )
{
    int              i, d, n_tag_points;
    FILE             *file;
    Status           status;
    int              n_volumes;
    double           **tags_volume1, **tags_volume2;
    char             **labels;
    Real             position[N_DIMENSIONS];

    status = open_file_with_default_suffix( filename, "tag", READ_FILE,
                                            ASCII_FORMAT, &file );

    if( status == OK &&
        !input_tag_points( file, &n_volumes, &n_tag_points, &tags_volume1,
                           &tags_volume2, &labels ) )
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

     return( status );
}
