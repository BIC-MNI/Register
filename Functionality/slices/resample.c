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
static char rcsid[] = "$Header: /static-cvsroot/visualization/Register/Functionality/slices/resample.c,v 1.10 1998-06-29 15:01:46 david Exp $";
#endif

#include  <register.h>

  VIO_Status  resample_the_volume(
    main_struct  *main,
    VIO_STR       resampled_filename )
{
    VIO_Status             status;
    char               command_str[VIO_EXTREMELY_LARGE_STRING_SIZE];
    char               tmp_name[L_tmpnam];
    VIO_STR             tmp_transform_filename;
    VIO_General_transform  *transform;

    status = VIO_OK;

    if( !get_tag_point_transform( main, &transform ) )
    {
        print( "No transform present.\n" );
        status = VIO_ERROR;
    }

    (void) tmpnam( tmp_name );

    tmp_transform_filename = concat_strings( tmp_name, "." );
    concat_to_string( &tmp_transform_filename,
                      get_default_transform_file_suffix() );

    if( status == VIO_OK )
        status = output_transform_file( tmp_transform_filename, NULL,
                                        transform );

    if( status == VIO_OK )
    {
        (void) sprintf( command_str, "%s %s %s %s %s",
                        Resample_command_name,
                        get_volume_filename( main, 1 - RESAMPLED_VOLUME_INDEX ),
                        get_volume_filename( main, RESAMPLED_VOLUME_INDEX ),
                        tmp_transform_filename,
                        resampled_filename );

        (void) system( command_str );

        remove_file( tmp_transform_filename );
    }

    delete_string( tmp_transform_filename );

    return( status );
}
