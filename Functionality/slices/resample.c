#include  <def_register.h>

public  Status  resample_the_volume(
    main_struct  *main,
    char         resampled_filename[] )
{
    Status             status;
    FILE               *file;
    char               command_str[10000];
    String             tmp_transform_filename;
    General_transform  gen_transform;
    Transform          *transform;

    status = OK;

    if( !get_tag_point_transform( main, &transform, (Transform **) NULL ) )
    {
        print( "No transform present.\n" );
        status = ERROR;
    }

    if( status == OK )
    {
        (void) tmpnam( tmp_transform_filename );

        status = open_file( tmp_transform_filename, WRITE_FILE, ASCII_FORMAT,
                            &file );
    }

    create_linear_transform( &gen_transform, transform );

    if( status == OK && !output_transform( file, (char *) NULL, &gen_transform))
        status = ERROR;

    if( status == OK )
        status = close_file( file );

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
