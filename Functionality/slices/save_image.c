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

#include  <register.h>

#define  PREFIX   "register_image_"

static  int  frame_number = 1;

static  void  save_rgb_image(
    VIO_STR   filename,
    int      x_min,
    int      x_max,
    int      y_min,
    int      y_max )
{
    char   command[VIO_EXTREMELY_LARGE_STRING_SIZE];

    (void) sprintf( command, "import -window root -crop %dx%d+%d+%d %s ", 
                    x_max-x_min, y_max-y_min, x_min, y_min, filename );
    (void) printf( "%s", command );
    (void) flush_file( stdout );
    (void) system( command );
    (void) printf( "  ------ done\n" );
}

  void  save_image(
    main_struct   *main_info,
    int           volume_index,
    int           view_index )
{
    int     axis, sizes[VIO_MAX_DIMENSIONS];
    VIO_Real    *position, voxel[VIO_MAX_DIMENSIONS];
    VIO_Volume  volume;
    VIO_Real    x_min_real, y_min_real, x_max_real, y_max_real;
    int     x_min, y_min, x_max, y_max;
    int     x_size, y_size;
    int     viewport_x_min, viewport_y_min, viewport_x_max, viewport_y_max;
    int     x_origin, y_origin;
    char    filename[VIO_EXTREMELY_LARGE_STRING_SIZE];

    if( !is_volume_active( main_info, volume_index ) )
        return;

    position = get_volume_cursor( main_info, volume_index );
    axis = get_slice_axis( view_index );

    voxel[VIO_X] = -0.5;
    voxel[VIO_Y] = -0.5;
    voxel[VIO_Z] = -0.5;
    voxel[axis] = position[axis];

    convert_voxel_to_pixel( main_info, volume_index, view_index,
                            voxel, &x_min_real, &y_min_real );

    volume = get_slice_volume( main_info, volume_index );
    get_volume_sizes( volume, sizes );

    voxel[VIO_X] = (VIO_Real) sizes[VIO_X]-0.5;
    voxel[VIO_Y] = (VIO_Real) sizes[VIO_Y]-0.5;
    voxel[VIO_Z] = (VIO_Real) sizes[VIO_Z]-0.5;
    voxel[axis] = position[axis];

    convert_voxel_to_pixel( main_info, volume_index, view_index,
                            voxel, &x_max_real, &y_max_real );

    get_slice_viewport_size( main_info, volume_index, view_index,
                             &x_size, &y_size );

    if (x_min_real < x_max_real) {
        x_min = (int) x_min_real + 1;
        x_max = (int) x_max_real;
    }
    else {
        x_min = (int) x_max_real + 1;
        x_max = (int) x_min_real;
    }

    if (y_min_real < y_max_real) {
        y_min = (int) y_min_real + 1;
        y_max = (int) y_max_real;
    }
    else {
        y_min = (int) y_max_real + 1;
        y_max = (int) y_min_real;
    }

    if( x_min < 0 )
        x_min = 0;
    if( y_min < 0 )
        y_min = 0;
    if( x_max >= x_size )
        x_max = x_size-1;
    if( y_max >= y_size )
        y_max = y_size-1;

    G_get_window_position( main_info->window, &x_origin, &y_origin );
    G_get_window_size(main_info->window, &x_size, &y_size );

    get_slice_viewport( main_info, volume_index, view_index,
                        &viewport_x_min, &viewport_x_max,
                        &viewport_y_min, &viewport_y_max );

    /* Now invert the sense of the viewport w.r.t. the Y axis */
    viewport_y_min = y_size - viewport_y_max;

    do
    {
        (void) sprintf( filename, "%s%d.tiff", PREFIX, frame_number );
        ++frame_number;
    }
    while( file_exists( filename ) );

    save_rgb_image( filename, x_origin + viewport_x_min + x_min,
                              x_origin + viewport_x_min + x_max,
                              y_origin + viewport_y_min + y_min,
                              y_origin + viewport_y_min + y_max );
}
