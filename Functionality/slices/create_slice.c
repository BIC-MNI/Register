#include  <def_register.h>

private   int     x_axes[] = { Y, X, X };
private   int     y_axes[] = { Z, Z, Y };

private  void  check_axes_assigned()
{
    private  Boolean  first = TRUE;

    if( first )
    {
        first = FALSE;
        x_axes[0] = Slice_1_x_axis_index;
        x_axes[1] = Slice_2_x_axis_index;
        x_axes[2] = Slice_3_x_axis_index;
        y_axes[0] = Slice_1_y_axis_index;
        y_axes[1] = Slice_2_y_axis_index;
        y_axes[2] = Slice_3_y_axis_index;
    }
}

public  void  get_slice_axes(
    int   axis_index,
    int   *x_axis_index,
    int   *y_axis_index )
{
    check_axes_assigned();

    *x_axis_index = x_axes[axis_index];
    *y_axis_index = y_axes[axis_index];
}

public  void  create_slice_pixels(
    main_struct   *main,
    int           volume,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_min, x_max, y_min, y_max;
    slice_struct     *slice;
    Pixel_types      pixel_type;

    slice = &main->trislice[volume].slices[view];

    get_slice_axes( view, &x_axis_index, &y_axis_index );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport( main, volume, view,
                        &x_min, &x_max, &y_min, &y_max );

    create_volume_slice( &main->trislice[volume].volume,
                         main->trislice[volume].position[view],
                         x_axis_index, FALSE,
                         y_axis_index, FALSE,
                         slice->x_translation,
                         slice->y_translation,
                         slice->x_scale,
                         slice->y_scale,
                         x_max - x_min + 1,
                         y_max - y_min + 1,
                         pixel_type,
                         main->interpolation_flag,
                         main->trislice[volume].cmode_colour_map,
                         main->trislice[volume].rgb_colour_map,
                         &slice->n_pixels_alloced,
                         slice->pixels );
}

public  Boolean   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    Real            voxel_position[N_DIMENSIONS] )
{
    Boolean    in_volume;
    int        x_axis_index, y_axis_index;

    get_slice_axes( view_index, &x_axis_index, &y_axis_index );

    in_volume = convert_slice_pixel_to_voxel(
              &main->trislice[volume_index].volume,
              x_pixel, y_pixel,
              main->trislice[volume_index].position,
              x_axis_index, FALSE,
              y_axis_index, FALSE,
              main->trislice[volume_index].slices[view_index].x_translation,
              main->trislice[volume_index].slices[view_index].y_translation,
              main->trislice[volume_index].slices[view_index].x_scale,
              main->trislice[volume_index].slices[view_index].y_scale,
              voxel_position );

    return( in_volume );
}

public  void   convert_voxel_to_pixel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    Real            voxel_position[N_DIMENSIONS],
    int             *x_pixel,
    int             *y_pixel )
{
    int        x_axis_index, y_axis_index;

    get_slice_axes( view_index, &x_axis_index, &y_axis_index );

    convert_voxel_to_slice_pixel(
              &main->trislice[volume_index].volume,
              voxel_position,
              x_axis_index, FALSE,
              y_axis_index, FALSE,
              main->trislice[volume_index].slices[view_index].x_translation,
              main->trislice[volume_index].slices[view_index].y_translation,
              main->trislice[volume_index].slices[view_index].x_scale,
              main->trislice[volume_index].slices[view_index].y_scale,
              x_pixel, y_pixel );
}

public  int  get_slice_viewport_index( int volume, int view )
{
    return( volume * N_VIEWS + view );
}

public  void  get_volume_view_index(
    int   viewport_index,
    int   *volume,
    int   *view )
{
    *volume = viewport_index / N_VIEWS;
    *view = viewport_index % N_VIEWS;
}

public  void  get_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_min,
    int          *x_max,
    int          *y_min,
    int          *y_max )
{
    get_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           x_min, x_max, y_min, y_max );
}

public  void  set_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_min,
    int          x_max,
    int          y_min,
    int          y_max )
{
    set_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           x_min, x_max, y_min, y_max );
}

public  void  translate_slice(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_translation,
    int          y_translation )
{
    main->trislice[volume].slices[view].x_translation += (Real) x_translation;
    main->trislice[volume].slices[view].y_translation += (Real) y_translation;
    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
}
