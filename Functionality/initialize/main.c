#include  <def_register.h>

#define  X_SIZE          100
#define  Y_SIZE          100
#define  Z_SIZE          100

#define  VIEWPORT_SIZE   300

#define  N_VALUES        256

main()
{
    Status           status;
    window_struct    *window;
    int              i, x, y, z, val, iter;
    Real             dx, dy, dz;
    int              colour_index_offset;
    Real             x_scale[3][3], y_scale[3][3];
    int              x_translation[3][3], y_translation[3][3];
    int              x_offset[3][3], y_offset[3][3];
    int              a, a1, a2, y_image;
    Pixel_types      pixel_type;
    pixels_struct    pixels[3][3];
    Boolean          interpolation_flag;
    volume_struct    volume;
    Colour           rgb_colour_map[N_VALUES];
    Real             start_time, compute_time, render_time;

    pixel_type = RGB_PIXEL;
    interpolation_flag = FALSE;

    for_less( i, 0, N_VALUES )
        rgb_colour_map[i] = make_Colour( i, i, i );

    volume.sizes[X] = X_SIZE;
    volume.sizes[Y] = Y_SIZE;
    volume.sizes[Z] = Z_SIZE;

    ALLOC3D( status, volume.data, volume.sizes[X], volume.sizes[Y],
             volume.sizes[Z] );

    volume.thickness[X] = 1.0;
    volume.thickness[Y] = 0.3;
    volume.thickness[Z] = 0.2;

    for_less( x, 0, X_SIZE )
    {
        dx = 2.0 * (Real) x / (Real) X_SIZE - 1.0;
        if( dx < 0.0 ) dx = -dx;
        for_less( y, 0, Y_SIZE )
        {
            dy = 2.0 * (Real) y / (Real) Y_SIZE - 1.0;
            if( dy < 0.0 ) dy = -dy;
            for_less( z, 0, Z_SIZE )
            {
                dz = 2.0 * (Real) z / (Real) Z_SIZE - 1.0;
                if( dz < 0.0 ) dz = -dz;
                val = (int) ((dx + dy + dz) * (Real) N_VALUES / 2.0 );
                if( val > N_VALUES-1 )  val = 0;
                volume.data[x][y][z] = val;
            }
        }
        if( x % 10 == 0 )
            (void) printf( "%d/%d\n", x, X_SIZE );
    }

    /* ---------------- open window --------------------- */

    status = G_create_window( "Test Volume Slice", -1, -1, 900, 900, &window );

    G_set_colour_map_state( window, pixel_type != RGB_PIXEL );

    colour_index_offset = 256;

    if( pixel_type != RGB_PIXEL )
    {
        for_less( i, 0, N_VALUES )
            G_set_colour_map_entry( window, NORMAL_PLANES,
                                    i + colour_index_offset,
                                    make_Colour(i,i,i) );
    }

    /* ---------------- render pixels --------------------- */

    for_less( a, 0, 3 )
    {
        for_less( y_image, 0, 3 )
        {
            x_translation[a][y_image] = 0;
            y_translation[a][y_image] = 0;
            x_scale[a][y_image] = 1.0;
            y_scale[a][y_image] = 1.0;
        }
    }

    for_less( iter, 0, 10 )
    {

    for_less( a, 0, 3 )
    {
        for_less( y_image, 0, 3 )
        {
/*
            x_scale[a][y_image] += 0.05;
            y_scale[a][y_image] += 0.03;
            x_translation[a][y_image] -= 10;
            y_translation[a][y_image] += 30;
*/
        }
    }
    start_time = current_realtime_seconds();

    for_less( a, 0, 3 )
    {
        a1 = (a + 1) % 3;
        a2 = (a + 2) % 3;

        for_less( y_image, 0, 3 )
        {
            create_volume_slice( &volume, volume.sizes[a] / 2.0,
                                 a1, FALSE, a2, FALSE,
                                 x_translation[a][y_image],
                                 y_translation[a][y_image],
                                 x_scale[a][y_image],
                                 y_scale[a][y_image],
                                 VIEWPORT_SIZE, VIEWPORT_SIZE,
                                 pixel_type,
                                 interpolation_flag,
                                 rgb_colour_map,
                                 colour_index_offset,
                                 &x_offset[a][y_image],
                                 &y_offset[a][y_image],
                                 &pixels[a][y_image] );
        }
    }

    compute_time = current_realtime_seconds() - start_time;
    start_time = current_realtime_seconds();

    for_less( a, 0, 3 )
    {
        for_less( y_image, 0, 3 )
            G_draw_pixels( window,
                           10 + a * (VIEWPORT_SIZE + 10) + x_offset[a][y_image],
                           10 + y_image * (VIEWPORT_SIZE + 10) +
                               y_offset[a][y_image],
                           0,
                           &pixels[a][y_image] );
    }

    render_time = current_realtime_seconds() - start_time;

    G_update_window( window );

    (void) printf( "%g %g\n", compute_time, render_time );
    }

    (void) printf( "Hit return: " );
    (void) getchar();

    status = G_delete_window( window );

    return( status != OK );
}
