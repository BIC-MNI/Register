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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/tags/save_and_load.c,v 1.13 1995-10-02 18:34:44 david Exp $";
#endif

#include  <register.h>

private  void   create_tags_array(
    int                n_valid_tags,
    BOOLEAN            valid_tags[],
    int                n_tag_points,
    tag_point_struct   tag_points[],
    int                which_volume,
    Real               ***tag_array )
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

private  STRING  create_comments(
    main_struct   *main )
{
    char    buffer1[EXTREMELY_LARGE_STRING_SIZE];
    char    buffer2[EXTREMELY_LARGE_STRING_SIZE];

    if( main->trislice[0].input_flag )
    {
        (void) sprintf( buffer1, "Volume: %s\n",
                        get_volume_filename(main,0) );
    }
    else
        buffer1[0] = (char) 0;

    if( main->trislice[1].input_flag )
    {
        (void) sprintf( buffer2, "Volume: %s\n",
                        get_volume_filename(main,1) );
    }
    else
        buffer2[0] = (char) 0;

    return( concat_strings( buffer1, buffer2 ) );
}

public  Status   save_tag_points(
    main_struct   *main,
    STRING        filename )
{
    int              i, n_valid_tags;
    FILE             *file;
    Status           status;
    int              n_volumes, tag_index;
    Real             **tags_volume1, **tags_volume2, ***ptr;
    STRING           *labels;
    tag_list_struct  *tags;
    BOOLEAN          *tag_is_valid, both_volumes_flag;
    STRING           comments;

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
    {
        ALLOC( labels, n_valid_tags );
    }

    tag_index = 0;
    for_less( i, 0, tags->n_tag_points )
    {
        if( tag_is_valid[i] )
        {
            labels[tag_index] = create_string( tags->tag_points[i].name );
            ++tag_index;
        }
    }

    status = open_file_with_default_suffix( filename, TAG_FILE_SUFFIX,
                                            WRITE_FILE, ASCII_FORMAT, &file );

    if( status == OK )
    {
        comments = create_comments( main );

        status = output_tag_points( file, comments, n_volumes,
                                    n_valid_tags,
                                    tags_volume1, tags_volume2,
                                    (Real *) NULL, (int *) NULL, (int *) NULL,
                                    labels );

        delete_string( comments );
    }

    if( n_valid_tags > 0 )
    {
        FREE2D( tags_volume1 );

        for_less( i, 0, n_valid_tags )
            delete_string( labels[i] );

        FREE( labels );

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
    STRING        filename )
{
    int              i, d, n_tag_points;
    FILE             *file;
    Status           file_status, status;
    int              n_volumes;
    Real             **tags_volume1, **tags_volume2;
    STRING           *labels;
    Real             position[N_DIMENSIONS];

    file_status = open_file_with_default_suffix( filename, TAG_FILE_SUFFIX,
                                                 READ_FILE,
                                                 ASCII_FORMAT, &file );

    status = file_status;

    if( status == OK &&
        input_tag_points( file, &n_volumes, &n_tag_points, &tags_volume1,
                          &tags_volume2, (Real **) NULL, (int **) NULL,
                          (int **) NULL, &labels ) != OK )
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
                         (Real *) NULL, (int *) 0, (int *) 0, labels );
    }

    if( file_status == OK )
        (void) close_file( file );

    update_all_tag_objects( main );

    main->tags.saved_flag = TRUE;

    return( status );
}

public  Status   save_transform(
    main_struct   *main,
    STRING        filename )
{
    Status                   status;
    General_transform        *transform;
    STRING                   comments;

    status = OK;

    if( get_tag_point_transform( main, &transform ) )
    {
        comments = create_comments( main );

        status = output_transform_file( filename, comments, transform );

        delete_string( comments );
    }

    return( status );
}
