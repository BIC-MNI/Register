#include  <register.h>

public  Status  resample_the_volume(
    main_struct  *main,
    char         resampled_filename[] )
{
    Status             status;
    char               command_str[10000];
    STRING             tmp_transform_filename;
    General_transform  *transform;

    status = OK;

    if( !get_tag_point_transform( main, &transform ) )
    {
        print( "No transform present.\n" );
        status = ERROR;
    }

    (void) tmpnam( tmp_transform_filename );
    (void) strcat( tmp_transform_filename, "." );
    (void) strcat( tmp_transform_filename, get_default_transform_file_suffix());

    if( status == OK )
        status = output_transform_file( tmp_transform_filename, NULL,
                                        transform );

    if( status == OK )
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

    return( status );
}
