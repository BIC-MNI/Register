/**
 * \file Functionality/tags/save_and_load.c
 * \brief Read and write tag objects to and from a file.
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

static  void   create_tags_array(
    VIO_BOOL            valid_tags[],
    int                n_tag_points,
    tag_point_struct   tag_points[],
    int                which_volume,
    VIO_Real               ***tag_array )
{
    int   i, tag_index, d, n_valid;

    n_valid = 0;

    for_less( i, 0, n_tag_points )
        if( valid_tags[i] )
            ++n_valid;

    if( n_valid > 0 )
    {
        VIO_ALLOC2D( *tag_array, n_valid, VIO_N_DIMENSIONS );

        tag_index = 0;

        for_less( i, 0, n_tag_points )
        {
            if( valid_tags[i] )
            {
                for_less( d, 0, VIO_N_DIMENSIONS )
                {
                    (*tag_array)[tag_index][d] = (VIO_Real) Point_coord
                                    (tag_points[i].position[which_volume], d );
                }

                ++tag_index;
            }
        }
    }
}

static  VIO_STR  create_comments(
    main_struct   *main )
{
    char    buffer1[VIO_EXTREMELY_LARGE_STRING_SIZE];
    char    buffer2[VIO_EXTREMELY_LARGE_STRING_SIZE];

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

  VIO_Status   save_tag_points(
    main_struct   *main,
    VIO_STR        filename )
{
    int              i, n_valid_tags;
    FILE             *file;
    VIO_Status           status;
    int              n_volumes, tag_index;
    VIO_Real             **tags_volume1, **tags_volume2, ***ptr;
    VIO_STR           *labels;
    tag_list_struct  *tags;
    VIO_BOOL          *tag_is_valid;
    VIO_STR           comments;

    tags = &main->tags;
    tags_volume2 = NULL;

    if( tags->n_tag_points > 0 )
        ALLOC( tag_is_valid, tags->n_tag_points );

    if( !main->trislice[0].input_flag && !main->trislice[1].input_flag )
        return( VIO_ERROR );

    n_valid_tags = 0;

    for_less( i, 0, tags->n_tag_points )
    {
        if( !get_tag_point_activity( main, i ) )
            tag_is_valid[i] = FALSE;
        else
        {
            tag_is_valid[i] = ((main->trislice[0].input_flag &&
                                tags->tag_points[i].position_exists[0]) ||
                               (main->trislice[1].input_flag &&
                                tags->tag_points[i].position_exists[1]));
        }

        if( tag_is_valid[i] )
            ++n_valid_tags;
    }

    n_volumes = 0;

    if( main->trislice[0].input_flag )
    {
        create_tags_array( tag_is_valid,
                           tags->n_tag_points, tags->tag_points,
                           0, &tags_volume1 );
        ++n_volumes;
    }

    if( main->trislice[1].input_flag )
    {
        if( n_volumes == 0 )
            ptr = &tags_volume1;
        else
            ptr = &tags_volume2;

        create_tags_array( tag_is_valid,
                           tags->n_tag_points, tags->tag_points,
                           1, ptr );
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

    if( status == VIO_OK )
    {
        comments = create_comments( main );

        status = output_tag_points( file, comments, n_volumes,
                                    n_valid_tags,
                                    tags_volume1, tags_volume2,
                                    (VIO_Real *) NULL, (int *) NULL, (int *) NULL,
                                    labels );

        delete_string( comments );
    }

    if( n_valid_tags > 0 )
    {
        VIO_FREE2D( tags_volume1 );

        for_less( i, 0, n_valid_tags )
            delete_string( labels[i] );

        FREE( labels );

        if( n_volumes == 2 )
           VIO_FREE2D( tags_volume2 );
    }

    if( status == VIO_OK )
        status = close_file( file );

    if( tags->n_tag_points > 0 )
        FREE( tag_is_valid );

    main->tags.saved_flag = TRUE;

    return( status );
}

  VIO_Status   load_tag_points(
    main_struct   *main,
    VIO_STR        filename )
{
    int              i, d, n_tag_points;
    FILE             *file;
    VIO_Status           file_status, status;
    int              n_volumes;
    VIO_Real             **tags_volume1, **tags_volume2;
    VIO_STR           *labels;
    VIO_Real             position[VIO_N_DIMENSIONS];

    file_status = open_file_with_default_suffix( filename, TAG_FILE_SUFFIX,
                                                 READ_FILE,
                                                 ASCII_FORMAT, &file );

    status = file_status;

    if( status == VIO_OK &&
        input_tag_points( file, &n_volumes, &n_tag_points, &tags_volume1,
                          &tags_volume2, (VIO_Real **) NULL, (int **) NULL,
                          (int **) NULL, &labels ) != VIO_OK )
    {
        status = VIO_ERROR;
    }

    if( status == VIO_OK )
    {
        delete_tag_points( main );
        initialize_tag_points( main );

        for_less( i, 0, n_tag_points )
        {
            create_new_tag_point( main );

            for_less( d, 0, VIO_N_DIMENSIONS )
                position[d] = tags_volume1[i][d];

            set_tag_point_position( main, i, 0, position );

            if( n_volumes == 2 )
            {
                for_less( d, 0, VIO_N_DIMENSIONS )
                    position[d] = tags_volume2[i][d];

                set_tag_point_position( main, i, 1, position );
            }

            set_tag_point_name( main, i, labels[i] );
        }

        free_tag_points( n_volumes, n_tag_points, tags_volume1, tags_volume2,
                         (VIO_Real *) NULL, (int *) 0, (int *) 0, labels );
    }

    if( file_status == VIO_OK )
        (void) close_file( file );

    update_all_tag_objects( main );

    main->tags.saved_flag = TRUE;

    return( status );
}

  VIO_Status   save_transform(
    main_struct   *main,
    VIO_STR        filename )
{
    VIO_Status                   status;
    VIO_General_transform        *transform;
    VIO_STR                   comments;

    status = VIO_OK;

    if( get_tag_point_transform( main, &transform ) )
    {
        comments = create_comments( main );

        status = output_transform_file( filename, comments, transform );

        delete_string( comments );
    }

    return( status );
}
