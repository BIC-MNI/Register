#include  <def_register.h>

public  void  create_slice_pixels(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_min, x_max, y_min, y_max;
    Pixel_types      pixel_type;
    Real             x_translation, y_translation, x_scale, y_scale;
    volume_struct    *volume;
    Real             *position;

    volume = get_slice_volume( main, volume_index );
    position = get_volume_cursor( main, volume_index );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport( main, volume_index, view,
                        &x_min, &x_max, &y_min, &y_max );

    create_volume_slice( volume, position[view],
                    x_axis_index, FALSE,
                    y_axis_index, FALSE,
                    x_translation, y_translation, x_scale, y_scale,
                    x_max - x_min + 1,
                    y_max - y_min + 1,
                    pixel_type,
                    main->interpolation_flag,
                    main->trislice[volume_index].cmode_colour_map,
                    main->trislice[volume_index].rgb_colour_map,
                    &main->trislice[volume_index].slices[view].n_pixels_alloced,
                    main->trislice[volume_index].slices[view].pixels );
}

public  Boolean   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    Real            voxel_position[N_DIMENSIONS] )
{
    Boolean        in_volume;
    int            x_axis_index, y_axis_index;
    Real           x_translation, y_translation, x_scale, y_scale;
    volume_struct  *volume;
    Real           *position;

    volume = get_slice_volume( main, volume_index );
    position = get_volume_cursor( main, volume_index );
    get_slice_axes( view_index, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    in_volume = convert_slice_pixel_to_voxel(
              volume, x_pixel, y_pixel, position,
              x_axis_index, FALSE, y_axis_index, FALSE,
              x_translation, y_translation, x_scale, y_scale,
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
    int            x_axis_index, y_axis_index;
    Real           x_translation, y_translation, x_scale, y_scale;
    volume_struct  *volume;

    volume = get_slice_volume( main, volume_index );
    get_slice_axes( view_index, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    convert_voxel_to_slice_pixel( volume, voxel_position,
                                  x_axis_index, FALSE, y_axis_index, FALSE,
                                  x_translation, y_translation,
                                  x_scale, y_scale, x_pixel, y_pixel );
}

public  void  translate_slice(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_translation_offset,
    int          y_translation_offset )
{
    Real           x_translation, y_translation, x_scale, y_scale;

    get_slice_transform( main, volume, view, &x_translation, &y_translation,
                         &x_scale, &y_scale );

    x_translation += (Real) x_translation_offset;
    y_translation += (Real) y_translation_offset;

    set_slice_translation( main, volume, x_translation, y_translation );
    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
}
