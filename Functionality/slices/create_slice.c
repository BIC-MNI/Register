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

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/slices/create_slice.c,v 1.27 1995-12-19 15:46:56 david Exp $";
#endif

#include  <register.h>

public  void  create_slice_pixels(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    int              x_size, y_size;
    Pixel_types      pixel_type;
    Filter_types     filter_type;
    Real             filter_width;
    Real             x_translation, y_translation, x_scale, y_scale;
    Real             origin[MAX_DIMENSIONS];
    Real             x_axis[MAX_DIMENSIONS], y_axis[MAX_DIMENSIONS];
    Volume           volume;
    unsigned short   *cmode_colour_map;
    Colour           *rgb_colour_map, **rgb_colour_map_ptr;

    volume = get_slice_volume( main, volume_index );
    get_slice_transform( main, volume_index, view,
                         &x_translation, &y_translation, &x_scale, &y_scale );
    filter_type = get_slice_filter_type( main, volume_index, view );
    filter_width = get_slice_filter_width( main, volume_index, view );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport_size( main, volume_index, view, &x_size, &y_size );

    cmode_colour_map = main->trislice[volume_index].cmode_colour_map;

    if( is_an_rgb_volume(volume) )
        rgb_colour_map_ptr = NULL;
    else
    {
        rgb_colour_map = main->trislice[volume_index].rgb_colour_map;
        rgb_colour_map_ptr = &rgb_colour_map;
    }

    get_slice_plane( main, volume_index, view, origin, x_axis, y_axis );

    create_volume_slice( volume, filter_type, filter_width,
                         origin, x_axis, y_axis,
                         x_translation, y_translation, x_scale, y_scale,
                         (Volume) NULL, NEAREST_NEIGHBOUR, 0.0,
                         (Real *) NULL, (Real *) NULL, (Real *) NULL,
                         0.0, 0.0, 0.0, 0.0,
                         x_size, y_size, 0, -1, 0, -1, pixel_type,
                         main->degrees_continuity,
                         &cmode_colour_map,
                         rgb_colour_map_ptr, make_rgba_Colour( 0, 0, 0, 0 ),
                    main->render_storage, TRUE,
                    &main->trislice[volume_index].slices[view].n_pixels_alloced,
                    main->trislice[volume_index].slices[view].pixels );
}

private  void  convert_volume1_voxel_to_volume2(
    main_struct   *main,
    Real          voxel1[],
    Real          voxel2[] )
{
    Volume  volume1;
    Real    world[N_DIMENSIONS], original_world2[N_DIMENSIONS];

    volume1 = get_slice_volume( main, 0 );

    convert_voxel_to_world( volume1, voxel1, &world[X], &world[Y], &world[Z] );

    convert_world_to_original_world( main, RESAMPLED_VOLUME_INDEX,
                                     world[X], world[Y], world[Z],
                                     &original_world2[X], &original_world2[Y],
                                     &original_world2[Z] );

    convert_original_world_to_voxel( main, RESAMPLED_VOLUME_INDEX,
                                     original_world2[X], original_world2[Y],
                                     original_world2[Z],
                                     &voxel2[X], &voxel2[Y], &voxel2[Z] );
}

public  void  create_merged_pixels(
    main_struct   *main,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_size, y_size, axis;
    int              c, sizes1[N_DIMENSIONS];
    Real             separations2[N_DIMENSIONS], x_len, y_len;
    Pixel_types      pixel_type;
    Filter_types     filter_type;
    Real             filter_width;
    Real             x_scale1, y_scale1, x_scale2, y_scale2;
    Real             x_translation1, y_translation1;
    Real             x_translation2, y_translation2;
    Volume           volume1, volume2;
    Real             *position1, voxel1[MAX_DIMENSIONS];
    Real             x_lower_left_pixel, y_lower_left_pixel;
    Real             x_upper_left_pixel, y_upper_left_pixel;
    Real             x_lower_right_pixel, y_lower_right_pixel;
    Real             dx_pixel, dy_pixel;
    Real             lower_left_voxel2[MAX_DIMENSIONS];
    Real             upper_left_voxel2[MAX_DIMENSIONS];
    Real             lower_right_voxel2[MAX_DIMENSIONS];
    Real             origin1[MAX_DIMENSIONS];
    Real             x_axis1[MAX_DIMENSIONS], y_axis1[MAX_DIMENSIONS];
    Real             origin2[MAX_DIMENSIONS];
    Real             x_axis2[MAX_DIMENSIONS], y_axis2[MAX_DIMENSIONS];
    Colour           *rgb_colour_map, **rgb_colour_map_ptr;
    pixels_struct    *merged_pixels, pixels1, pixels2;

    volume1 = get_slice_volume( main, 0 );
    volume2 = get_slice_volume( main, 1 );
    get_volume_sizes( volume1, sizes1 );
    get_volume_separations( volume2, separations2 );

    position1 = get_volume_cursor( main, MERGED_VOLUME_INDEX );

    get_slice_axes( view, &x_axis_index, &y_axis_index );
    axis = get_slice_axis( view );

    voxel1[X] = 0.0;
    voxel1[Y] = 0.0;
    voxel1[Z] = 0.0;
    voxel1[axis] = position1[axis];
    convert_volume1_voxel_to_volume2( main, voxel1, lower_left_voxel2 );
    convert_voxel_to_pixel( main, MERGED_VOLUME_INDEX, view, voxel1,
                            &x_lower_left_pixel, &y_lower_left_pixel );

    voxel1[x_axis_index] = (Real) sizes1[x_axis_index];
    convert_volume1_voxel_to_volume2( main, voxel1, lower_right_voxel2 );
    convert_voxel_to_pixel( main, MERGED_VOLUME_INDEX, view, voxel1,
                            &x_lower_right_pixel, &y_lower_right_pixel );
    voxel1[x_axis_index] = 0.0;

    voxel1[y_axis_index] = (Real) sizes1[y_axis_index];
    convert_volume1_voxel_to_volume2( main, voxel1, upper_left_voxel2 );
    convert_voxel_to_pixel( main, MERGED_VOLUME_INDEX, view, voxel1,
                            &x_upper_left_pixel, &y_upper_left_pixel );
    voxel1[y_axis_index] = 0.0;

    /*--- this assumes that x_axis1 and y_axis1 are principal axes */

    dx_pixel = x_lower_right_pixel - x_lower_left_pixel;
    if( dx_pixel == 0.0 )
        dx_pixel = 1.0;
    dy_pixel = y_upper_left_pixel - y_lower_left_pixel;
    if( dy_pixel == 0.0 )
        dy_pixel = 1.0;

    for_less( c, 0, get_volume_n_dimensions(volume2) )
    {
        x_axis2[c] = (lower_right_voxel2[c] - lower_left_voxel2[c]) / dx_pixel;
        y_axis2[c] = (upper_left_voxel2[c] - lower_left_voxel2[c]) / dy_pixel;
        origin2[c] = lower_left_voxel2[c];
    }

    x_len = 0.0;
    y_len = 0.0;
    for_less( c, 0, get_volume_n_dimensions(volume2) )
    {
        Real  comp;

        comp = x_axis2[c] * separations2[c];
        x_len += comp * comp;
        comp = y_axis2[c] * separations2[c];
        y_len += comp * comp;
    }

    x_len = sqrt( x_len );
    if( x_len == 0.0 )
        x_len = 1.0;
    y_len = sqrt( y_len );
    if( y_len == 0.0 )
        y_len = 1.0;

    x_scale2 = 1.0 / x_len;
    y_scale2 = 1.0 / y_len;
    x_translation2 = x_lower_left_pixel;
    y_translation2 = y_lower_left_pixel;

    get_slice_transform( main, MERGED_VOLUME_INDEX, view,
                         &x_translation1, &y_translation1,
                         &x_scale1, &y_scale1 );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport_size( main, MERGED_VOLUME_INDEX, view, &x_size, &y_size);

    get_slice_plane( main, MERGED_VOLUME_INDEX, view,
                     origin1, x_axis1, y_axis1 );

    filter_type = get_slice_filter_type( main, MERGED_VOLUME_INDEX, view );
    filter_width = get_slice_filter_width( main, MERGED_VOLUME_INDEX, view );

    merged_pixels = main->merged.slices[view].pixels;

    if( is_an_rgb_volume(volume1) || is_an_rgb_volume(volume2) )
    {
        set_volume_slice_pixel_range( volume1, filter_type, filter_width,
                    origin1, x_axis1, y_axis1,
                    x_translation1, y_translation1, x_scale1, y_scale1,
                    volume2, filter_type, filter_width,
                    origin2, x_axis2, y_axis2,
                    x_translation2, y_translation2, x_scale2, y_scale2,
                    x_size, y_size, pixel_type,
                    &main->merged.slices[view].n_pixels_alloced,
                    merged_pixels );

        if( is_an_rgb_volume(volume1) )
            rgb_colour_map_ptr = NULL;
        else
        {
            rgb_colour_map = main->trislice[0].rgb_colour_map;
            rgb_colour_map_ptr = &rgb_colour_map;
        }

        initialize_pixels( &pixels1,
                           merged_pixels->x_position,
                           merged_pixels->y_position,
                           merged_pixels->x_size,
                           merged_pixels->y_size,
                           merged_pixels->x_zoom,
                           merged_pixels->y_zoom,
                           RGB_PIXEL );

        create_volume_slice( volume1, filter_type, filter_width,
                    origin1, x_axis1, y_axis1,
                    x_translation1, y_translation1, x_scale1, y_scale1,
                    (Volume) NULL, NEAREST_NEIGHBOUR, 0.0,
                    (Real *) NULL, (Real *) NULL, (Real *) NULL,
                    0.0, 0.0, 0.0, 0.0,
                    x_size, y_size, 0, -1, 0, -1, pixel_type,
                    main->degrees_continuity,
                    NULL,
                    rgb_colour_map_ptr,
                    make_rgba_Colour( 0, 0, 0, 0 ),
                    main->render_storage,
                    FALSE, NULL, &pixels1 );

        if( is_an_rgb_volume(volume2) )
            rgb_colour_map_ptr = NULL;
        else
        {
            rgb_colour_map = main->trislice[1].rgb_colour_map;
            rgb_colour_map_ptr = &rgb_colour_map;
        }
        initialize_pixels( &pixels2,
                           merged_pixels->x_position,
                           merged_pixels->y_position,
                           merged_pixels->x_size,
                           merged_pixels->y_size,
                           merged_pixels->x_zoom,
                           merged_pixels->y_zoom,
                           RGB_PIXEL );

        create_volume_slice( volume2, filter_type, filter_width,
                    origin2, x_axis2, y_axis2,
                    x_translation2, y_translation2, x_scale2, y_scale2,
                    (Volume) NULL, NEAREST_NEIGHBOUR, 0.0,
                    (Real *) NULL, (Real *) NULL, (Real *) NULL,
                    0.0, 0.0, 0.0, 0.0,
                    x_size, y_size, 0, -1, 0, -1, pixel_type,
                    main->degrees_continuity,
                    NULL,
                    rgb_colour_map_ptr,
                    make_rgba_Colour( 0, 0, 0, 0 ),
                    main->render_storage,
                    FALSE, NULL, &pixels2 );

        composite_merged_pixels( main, &pixels1, &pixels2,
                                 main->merged.slices[view].pixels );

        delete_pixels( &pixels1 );
        delete_pixels( &pixels2 );
    }
    else
    {
        create_volume_slice( volume1, filter_type, filter_width,
                    origin1, x_axis1, y_axis1,
                    x_translation1, y_translation1, x_scale1, y_scale1,
                    volume2, filter_type, filter_width,
                    origin2, x_axis2, y_axis2,
                    x_translation2, y_translation2, x_scale2, y_scale2,
                    x_size, y_size, 0, -1, 0, -1, pixel_type,
                    main->degrees_continuity,
                    main->merged.cmode_colour_map,
                    main->merged.rgb_colour_map,
                    make_rgba_Colour( 0, 0, 0, 0 ),
                    main->render_storage,
                    TRUE, &main->merged.slices[view].n_pixels_alloced,
                    main->merged.slices[view].pixels );
    }
}

public  BOOLEAN   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    Real            voxel_position[N_DIMENSIONS] )
{
    BOOLEAN        in_volume;
    Real           x_translation, y_translation, x_scale, y_scale;
    Volume         volume;
    Real           origin[MAX_DIMENSIONS];
    Real           x_axis[MAX_DIMENSIONS], y_axis[MAX_DIMENSIONS];

    volume = get_slice_volume( main, volume_index );
    get_slice_plane( main, volume_index, view_index, origin, x_axis, y_axis );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    in_volume = convert_slice_pixel_to_voxel(
                    volume, x_pixel, y_pixel, origin, x_axis, y_axis,
                    x_translation, y_translation, x_scale, y_scale,
                    voxel_position );

    return( in_volume );
}

public  void   convert_voxel_to_pixel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    Real            voxel_position[N_DIMENSIONS],
    Real            *x_pixel,
    Real            *y_pixel )
{
    Real           origin[MAX_DIMENSIONS];
    Real           x_axis[MAX_DIMENSIONS], y_axis[MAX_DIMENSIONS];
    Real           x_translation, y_translation, x_scale, y_scale;
    Volume         volume;

    volume = get_slice_volume( main, volume_index );
    get_slice_plane( main, volume_index, view_index, origin, x_axis, y_axis );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    convert_voxel_to_slice_pixel( volume, voxel_position,
                                  origin, x_axis, y_axis,
                                  x_translation, y_translation,
                                  x_scale, y_scale, x_pixel, y_pixel );
}

private  void  record_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_viewport_size,
    int          y_viewport_size,
    int          used_x_viewport_size,
    int          used_y_viewport_size )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume, view );

    slice->prev_viewport_x_size = x_viewport_size;
    slice->prev_viewport_y_size = y_viewport_size;
    slice->used_viewport_x_size = used_x_viewport_size;
    slice->used_viewport_y_size = used_y_viewport_size;
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

    set_slice_translation( main, volume, view, x_translation, y_translation );

    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
    update_slice_tag_objects( main, volume, view );
}

public  void  scale_slice(
    main_struct  *main,
    int          volume,
    int          view,
    Real         scale_factor )
{
    Real  x_translation, y_translation, x_scale, y_scale;
    int   x_size, y_size;

    get_slice_viewport_size( main, volume, view, &x_size, &y_size );
    get_slice_transform( main, volume, view, &x_translation, &y_translation,
                         &x_scale, &y_scale );

    scale_slice_about_viewport_centre( scale_factor, x_size, y_size,
                                       &x_translation, &y_translation,
                                       &x_scale, &y_scale );

    set_slice_translation( main, volume, view, x_translation, y_translation );
    set_slice_scale( main, volume, view, x_scale, y_scale );

    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
    update_slice_tag_objects( main, volume, view );
}

public  void  resize_slice(
    main_struct  *main,
    int          volume_index,
    int          view )
{
    int            used_x_viewport_size, used_y_viewport_size;
    int            x_viewport_size, y_viewport_size;
    Real           x_scale, y_scale, x_trans, y_trans;
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );
    get_slice_viewport_size( main, volume_index, view, &x_viewport_size,
                             &y_viewport_size );
    get_slice_transform( main, volume_index, view,
                         &x_trans, &y_trans, &x_scale, &y_scale );

    resize_volume_slice( slice->prev_viewport_x_size,
                         slice->prev_viewport_y_size,
                         slice->used_viewport_x_size,
                         slice->used_viewport_y_size,
                         x_viewport_size, y_viewport_size,
                         &x_trans, &y_trans, &x_scale, &y_scale,
                         &used_x_viewport_size, &used_y_viewport_size );

    record_slice_viewport( main, volume_index, view,
                           x_viewport_size, y_viewport_size,
                           used_x_viewport_size, used_y_viewport_size );

    set_slice_translation( main, volume_index, view, x_trans, y_trans );
    set_slice_scale( main, volume_index, view, x_scale, y_scale );

    update_volume_cursor( main, volume_index, view );
    set_recreate_slice_flag( main, volume_index, view );
    update_slice_tag_objects( main, volume_index, view );
}

public  void  initialize_slice_view(
    main_struct  *main,
    int          volume_index,
    int          view )
{
    int            x_viewport_size, y_viewport_size;
    int            used_x_viewport_size, used_y_viewport_size;
    Volume         volume;
    Real           x_trans, y_trans, x_scale, y_scale;
    Real           origin[MAX_DIMENSIONS];
    Real           x_axis[MAX_DIMENSIONS], y_axis[MAX_DIMENSIONS];

    volume = get_slice_volume( main, volume_index );
    get_slice_plane( main, volume_index, view, origin, x_axis, y_axis );
    get_slice_viewport_size( main, volume_index, view, &x_viewport_size,
                             &y_viewport_size );

    fit_volume_slice_to_viewport( volume, origin, x_axis, y_axis,
                                  x_viewport_size, y_viewport_size,
                                  1.0 - Slice_fit_size,
                                  &x_trans, &y_trans, &x_scale, &y_scale,
                                  &used_x_viewport_size,
                                  &used_y_viewport_size );

    set_slice_translation( main, volume_index, view, x_trans, y_trans );
    set_slice_scale( main, volume_index, view, x_scale, y_scale );

    record_slice_viewport( main, volume_index, view,
                           x_viewport_size, y_viewport_size,
                           used_x_viewport_size, used_y_viewport_size );

    update_volume_cursor( main, volume_index, view );
    set_recreate_slice_flag( main, volume_index, view );
    update_slice_tag_objects( main, volume_index, view );
}

public  void  set_slice_interpolation(
    main_struct  *main,
    int          degrees_continuity )
{
    if( degrees_continuity == -1 ||
        degrees_continuity == 0 ||
        degrees_continuity == 2 )
    {
        main->degrees_continuity = degrees_continuity;
    }
}
