#include  <def_register.h>

public  void  initialize_slices( main_struct  *main )
{
    int      view, volume;

    for_less( volume, 0, N_VOLUMES )
    {
        main->trislice[volume].input_flag = FALSE;

        for_less( view, 0, N_VIEWS )
        {
            main->trislice[volume].slices[view].pixels_are_up_to_date = FALSE;
            main->trislice[volume].slices[view].n_pixels_alloced = 0;
            main->trislice[volume].slices[view].x_translation = 0.0;
            main->trislice[volume].slices[view].y_translation = 0.0;
            main->trislice[volume].slices[view].x_scale = 1.0;
            main->trislice[volume].slices[view].y_scale = 1.0;
        }
    }
}

public  void  terminate_slices( main_struct  *main )    /* ARGSUSED */
{
}

public  void  reset_slice_view(
    main_struct  *main,
    int          volume,
    int          view )
{
    int      x_min, x_max, y_min, y_max;
    int      x_axis_index, y_axis_index, x_viewport_size, y_viewport_size;
    Real     x_pixel_width, y_pixel_width, x_factor, y_factor, scale_factor;

    get_slice_axes( view, &x_axis_index, &y_axis_index );

    x_pixel_width = (Real) main->trislice[volume].volume.sizes[x_axis_index] *
                    main->trislice[volume].volume.thickness[x_axis_index];

    y_pixel_width = (Real) main->trislice[volume].volume.sizes[y_axis_index] *
                    main->trislice[volume].volume.thickness[y_axis_index];

    get_slice_viewport( main, volume, view, &x_min, &x_max, &y_min, &y_max );

    x_viewport_size = (Real) (x_max - x_min + 1);
    y_viewport_size = (Real) (y_max - y_min + 1);

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

    set_recreate_slice_flag( main, volume, view );
    update_volume_cursor( main, volume, view );
}
