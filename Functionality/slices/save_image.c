/**
 * \file Functionality/slices/save_image.c
 * \brief Save a portion of the window to a file.
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
#include  <bicpl/images.h>

#define  PREFIX   "register_image_"

static  int  frame_number = 1;

static VIO_Status   save_window_to_file(
    Gwindow         window,
    VIO_STR         filename,
    int             x_min,
    int             x_max,
    int             y_min,
    int             y_max )
{
    VIO_Status      status;
    int             x_size, y_size;
    pixels_struct   pixels;

    if( x_min > x_max || y_min > y_max )
    {
        G_get_window_size( window, &x_size, &y_size );
        x_min = 0;
        x_max = x_size-1;
        y_min = 0;
        y_max = y_size-1;
    }
    else
    {
        x_size = x_max - x_min + 1;
        y_size = y_max - y_min + 1;
    }
    initialize_pixels( &pixels, 0, 0, x_size, y_size, 1.0, 1.0, RGB_PIXEL );
    G_read_pixels( window, x_min, x_max, y_min, y_max,
                   &PIXEL_RGB_COLOUR(pixels, 0, 0) );
    status = output_rgb_file( filename, &pixels );
    delete_pixels( &pixels );
    return( status );
}

void  save_image(
    main_struct   *main_info,
    int           volume_index,
    int           view_index )
{
    int        axis, sizes[VIO_MAX_DIMENSIONS];
    VIO_Real   *position, voxel[VIO_MAX_DIMENSIONS];
    VIO_Volume volume;
    VIO_Real   x_min_real, y_min_real, x_max_real, y_max_real;
    int        x_min, y_min, x_max, y_max;
    int        vp_x_min, vp_y_min, vp_x_max, vp_y_max;
    int        x_size, y_size;
    char       filename[VIO_EXTREMELY_LARGE_STRING_SIZE];
    VIO_Status status;

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

    get_slice_viewport( main_info, volume_index, view_index, 
                        &vp_x_min, &vp_x_max, &vp_y_min, &vp_y_max );

    if (x_min_real < x_max_real)
    {
        x_min = (int) x_min_real + 1;
        x_max = (int) x_max_real;
    }
    else
    {
        x_min = (int) x_max_real + 1;
        x_max = (int) x_min_real;
    }

    if (y_min_real < y_max_real)
    {
        y_min = (int) y_min_real + 1;
        y_max = (int) y_max_real;
    }
    else
    {
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

    do
    {
        (void) sprintf( filename, "%s%d.ppm", PREFIX, frame_number );
        ++frame_number;
    }
    while( file_exists( filename ) );

    status = save_window_to_file( main_info->window, filename,
                                  vp_x_min+x_min, vp_x_min+x_max,
                                  vp_y_min+y_min, vp_y_min+y_max);
    if (status == VIO_OK)
    {
      printf( "Saved image to '%s'.\n", filename );
    }
}
