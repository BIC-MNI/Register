#include  <def_register.h>

public  void  initialize_slices( main_struct  *main )
{
    Status   status;
    int      i, view, volume;

    for_less( volume, 0, N_VOLUMES )
    {
        main->trislice[volume].input_flag = FALSE;

        for_less( view, 0, N_VIEWS )
        {
            main->trislice[volume].slices[view].buffer_is_up_to_date[0] = FALSE;
            main->trislice[volume].slices[view].buffer_is_up_to_date[1] = FALSE;
            main->trislice[volume].slices[view].pixels_are_up_to_date = FALSE;
            main->trislice[volume].slices[view].n_pixels_alloced = 0;
            main->trislice[volume].slices[view].x_translation = 0.0;
            main->trislice[volume].slices[view].y_translation = 0.0;
            main->trislice[volume].slices[view].x_scale = 1.0;
            main->trislice[volume].slices[view].y_scale = 1.0;
        }

        ALLOC( status, main->trislice[volume].rgb_colour_map, 256 );

        if( status != OK ) {}

        for_less( i, 0, 256 )
            main->trislice[volume].rgb_colour_map[i] = make_Colour( i, i, i );

        main->trislice[volume].colour_index_offset = 0;
    }
}

public  void  reset_slice_view(
    main_struct  *main,
    int          volume,
    int          view )
{
    int      x_axis_index, y_axis_index, x_viewport_size, y_viewport_size;
    Real     x_pixel_width, y_pixel_width, x_factor, y_factor, scale_factor;

    get_slice_axes( view, &x_axis_index, &y_axis_index );

    x_pixel_width = (Real) main->trislice[volume].volume.sizes[x_axis_index] *
                    main->trislice[volume].volume.thickness[x_axis_index];

    y_pixel_width = (Real) main->trislice[volume].volume.sizes[y_axis_index] *
                    main->trislice[volume].volume.thickness[y_axis_index];

    x_viewport_size = (Real) (main->trislice[volume].slices[view].x_max -
                              main->trislice[volume].slices[view].x_min + 1);
    y_viewport_size = (Real) (main->trislice[volume].slices[view].y_max -
                              main->trislice[volume].slices[view].y_min + 1);

    x_factor = x_viewport_size / x_pixel_width;
    y_factor = y_viewport_size / y_pixel_width;

    scale_factor = MIN( x_factor, y_factor );

    scale_factor *= Slice_fit_size;

    main->trislice[volume].slices[view].x_translation =
              ROUND( (x_viewport_size - scale_factor * x_pixel_width) / 2.0 );
    main->trislice[volume].slices[view].y_translation =
              ROUND( (y_viewport_size - scale_factor * y_pixel_width) / 2.0 );

    main->trislice[volume].slices[view].x_scale = scale_factor;
    main->trislice[volume].slices[view].y_scale = scale_factor;
}
