#include  <def_register.h>

#define  PREFIX   "register_image_"

private  int  frame_number = 1;

private  void  save_rgb_image(
    char   filename[],
    int    x_min,
    int    x_max,
    int    y_min,
    int    y_max )
{
    String   command;

    (void) sprintf( command, "scrsave %s %d %d %d %d", filename, x_min, x_max,
                                                       y_min, y_max );
    (void) printf( "%s", command );
    (void) flush_file( stdout );
    (void) system( command );
    (void) printf( "  ------ done\n" );
}

public  void  save_image(
    main_struct   *main_info,
    int           volume_index,
    int           view_index )
{
    int     axis, sizes[N_DIMENSIONS];
    Real    *position, voxel[N_DIMENSIONS];
    Volume  volume;
    Real    x_min_real, y_min_real, x_max_real, y_max_real;
    int     x_min, y_min, x_max, y_max;
    int     x_size, y_size;
    int     viewport_x_min, viewport_y_min, viewport_x_max, viewport_y_max;
    int     x_origin, y_origin;
    String  filename;

    if( !is_volume_active( main_info, volume_index ) )
        return;

    position = get_volume_cursor( main_info, volume_index );
    axis = get_slice_axis( view_index );

    voxel[X] = -0.5;
    voxel[Y] = -0.5;
    voxel[Z] = -0.5;
    voxel[axis] = position[axis];

    convert_voxel_to_pixel( main_info, volume_index, view_index,
                            voxel, &x_min_real, &y_min_real );

    volume = get_slice_volume( main_info, volume_index );
    get_volume_sizes( volume, sizes );

    voxel[X] = (Real) sizes[X]-0.5;
    voxel[Y] = (Real) sizes[Y]-0.5;
    voxel[Z] = (Real) sizes[Z]-0.5;
    voxel[axis] = position[axis];

    convert_voxel_to_pixel( main_info, volume_index, view_index,
                            voxel, &x_max_real, &y_max_real );

    get_slice_viewport_size( main_info, volume_index, view_index,
                             &x_size, &y_size );

    x_min = (int) x_min_real + 1;
    x_max = (int) x_max_real;
    y_min = (int) y_min_real + 1;
    y_max = (int) y_max_real;

    if( x_min < 0 )
        x_min = 0;
    if( y_min < 0 )
        y_min = 0;
    if( x_max >= x_size )
        x_max = x_size-1;
    if( y_max >= y_size )
        y_max = y_size-1;

    G_get_window_position( main_info->window, &x_origin, &y_origin );

    get_slice_viewport( main_info, volume_index, view_index,
                        &viewport_x_min, &viewport_x_max,
                        &viewport_y_min, &viewport_y_max );

    (void) sprintf( filename, "%s%d.rgb", PREFIX, frame_number );
    save_rgb_image( filename, x_origin + viewport_x_min + x_min,
                              x_origin + viewport_x_min + x_max,
                              y_origin + viewport_y_min + y_min,
                              y_origin + viewport_y_min + y_max );
    ++frame_number;
}
