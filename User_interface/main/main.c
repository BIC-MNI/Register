#include  <def_register.h>

#define  X_SIZE   256
#define  Y_SIZE   256
#define  Z_SIZE   128

#define  N_VALUES   256

main()
{
    Status           status;
    window_struct    *window;
    int              i, x, y, z, val, slice_offset;
    Real             dx, dy, dz;
    int              colour_index_offset;
    int              x_stride, y_stride;
    int              a, a1, a2, y_image, indices[3], sizes[3];
    Pixel_types      pixel_type;
    pixels_struct    pixels[3][3];
    Boolean          interpolation_flag;
    Volume_type      volume[X_SIZE*Y_SIZE*Z_SIZE];
    Volume_type      *volume_start;
    Colour           rgb_colour_map[N_VALUES];
    Real             thickness[3];
    Real             start_time, compute_time, render_time;

    pixel_type = RGB_PIXEL;
    interpolation_flag = FALSE;

    sizes[X] = X_SIZE;
    sizes[Y] = Y_SIZE;
    sizes[Z] = Z_SIZE;

    for_less( i, 0, N_VALUES )
        rgb_colour_map[i] = make_Colour( i, i, i );

    for_less( x, 0, X_SIZE )
    {
        dx = 2.0 * (Real) x / X_SIZE - 1.0;
        for_less( y, 0, Y_SIZE )
        {
            dy = 2.0 * (Real) y / Y_SIZE - 1.0;
            for_less( z, 0, Z_SIZE )
            {
                dz = 2.0 * (Real) z / Z_SIZE - 1.0;
                val = (int) ((ABS(dx) + ABS(dy) + ABS(dz)) * N_VALUES);
                if( val > N_VALUES-1 )  val = 0;
                volume[IJK(x,y,z,Y_SIZE,Z_SIZE)] = val;
            }
        }
        if( x % 10 == 0 )
            (void) printf( "%d/%d\n", x, X_SIZE );
    }

    /* ---------------- open window --------------------- */

    status = G_create_window( "Test Volume Slice", -1, -1, 900, 900, &window );

    G_set_colour_map_state( window, pixel_type != RGB_PIXEL );

    /* ---------------- render pixels --------------------- */

    colour_index_offset = 0;

    slice_offset = 0;

    start_time = current_realtime_seconds();

    for_less( a, 0, 3 )
    {
        a1 = (a + 1) % 3;
        a2 = (a + 2) % 3;

        for_less( y_image, 0, 3 )
        {
            indices[X] = 0;
            indices[Y] = 0;
            indices[Z] = 0;
            indices[a] = (sizes[a] / 2 + slice_offset + y_image) % sizes[a];
            volume_start = &volume[IJK(indices[X],indices[Y],indices[Z],
                                   Y_SIZE,Z_SIZE)];

            indices[X] = 0;
            indices[Y] = 0;
            indices[Z] = 0;
            indices[a1] = 1;
            x_stride = IJK(indices[X],indices[Y],indices[Z],Y_SIZE,Z_SIZE) -
                       IJK(0,0,0,Y_SIZE,Z_SIZE);

            if( x_stride > X_SIZE * Y_SIZE )
                abort();

            indices[X] = 0;
            indices[Y] = 0;
            indices[Z] = 0;
            indices[a2] = 1;
            y_stride = IJK(indices[X],indices[Y],indices[Z],Y_SIZE,Z_SIZE) -
                       IJK(0,0,0,Y_SIZE,Z_SIZE);

            status = initialize_pixels( &pixels[a][y_image], sizes[a1],
                                        sizes[a2], pixel_type );

            render_volume_to_slice( volume_start, x_stride, y_stride,
                                    1.0, 1.0,
                                    interpolation_flag, rgb_colour_map,
                                    colour_index_offset,
                                    &pixels[a][y_image] );
        }
    }

    compute_time = current_realtime_seconds() - start_time;
    start_time = current_realtime_seconds();

    for_less( a, 0, 3 )
    {
        for_less( y_image, 0, 3 )
            G_draw_pixels( window, 10 + a * 300, 10 + y_image * 300, 0,
                           &pixels[a][y_image] );
    }

    render_time = current_realtime_seconds() - start_time;

    G_update_window( window );

    (void) printf( "%g %g\n", compute_time, render_time );
    (void) printf( "Hit return: " );
    (void) getchar();

    status = G_delete_window( window );

    return( status != OK );
}
