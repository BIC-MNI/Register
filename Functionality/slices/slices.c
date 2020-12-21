/**
 * \file Functionality/slices/slices.c
 * \brief Basic volume and slice manipulation.
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

static   int         x_axes[] = { VIO_Y, VIO_X, VIO_X };
static   int         y_axes[] = { VIO_Z, VIO_Z, VIO_Y };
static   VIO_BOOL     x_axes_flip[] = { FALSE, FALSE, FALSE };
static   VIO_BOOL     y_axes_flip[] = { FALSE, FALSE, FALSE };

static  void  check_axes_assigned( void )
{
    static  VIO_BOOL  first = TRUE;

    if( first )
    {
        first = FALSE;
        x_axes[0] = Slice_1_x_axis_index;
        x_axes[1] = Slice_2_x_axis_index;
        x_axes[2] = Slice_3_x_axis_index;
        y_axes[0] = Slice_1_y_axis_index;
        y_axes[1] = Slice_2_y_axis_index;
        y_axes[2] = Slice_3_y_axis_index;
        x_axes_flip[0] = Slice_1_x_axis_flip;
        x_axes_flip[1] = Slice_2_x_axis_flip;
        x_axes_flip[2] = Slice_3_x_axis_flip;
        y_axes_flip[0] = Slice_1_y_axis_flip;
        y_axes_flip[1] = Slice_2_y_axis_flip;
        y_axes_flip[2] = Slice_3_y_axis_flip;
    }
}

  void  get_slice_axes(
    int   view_index,
    int   *x_axis_index,
    int   *y_axis_index )
{
    check_axes_assigned();

    *x_axis_index = x_axes[view_index];
    *y_axis_index = y_axes[view_index];
}

  void  get_slice_axes_flip(
    int       view_index,
    VIO_BOOL   *x_axis_flip,
    VIO_BOOL   *y_axis_flip )
{
    check_axes_assigned();

    *x_axis_flip = x_axes_flip[view_index];
    *y_axis_flip = y_axes_flip[view_index];
}

  int  get_slice_axis(
    int   view_index )
{
    int   axis;

    check_axes_assigned();

    for_less( axis, 0, VIO_N_DIMENSIONS )
    {
        if( x_axes[view_index] != axis && 
            y_axes[view_index] != axis )
        {
            return( axis );
        }
    }

    return( 0 );
}

  void  get_slice_plane(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          origin[],
    VIO_Real          x_axis[],
    VIO_Real          y_axis[] )
{
    VIO_Volume   volume;
    int      c, axis, x_index, y_index;
    VIO_Real     *slice_position, separations[VIO_MAX_DIMENSIONS];
    VIO_BOOL  x_flip, y_flip;

    get_slice_axes( view, &x_index, &y_index );
    axis = get_slice_axis( view );

    get_slice_axes_flip( view, &x_flip, &y_flip );

    volume = get_slice_volume( main, volume_index );
    get_volume_separations( volume, separations );

    for_less( c, 0, get_volume_n_dimensions(volume) )
    {
        origin[c] = 0.0;
        x_axis[c] = 0.0;
        y_axis[c] = 0.0;
    }

    slice_position = get_volume_cursor( main, volume_index );

    origin[axis] = slice_position[axis];

    get_volume_time_position(main, volume_index, &origin[VIO_T]);

    if( x_flip )
        x_axis[x_index] = -VIO_FSIGN(separations[x_index]);
    else
        x_axis[x_index] = VIO_FSIGN(separations[x_index]);

    if( y_flip )
        y_axis[y_index] = -VIO_FSIGN(separations[y_index]);
    else
        y_axis[y_index] = VIO_FSIGN(separations[y_index]);
}

  int  get_slice_viewport_index( int volume, int view )
{
    return( volume * N_VIEWS + view );
}

  void  get_volume_view_index(
    int   viewport_index,
    int   *volume,
    int   *view )
{
    *volume = viewport_index / N_VIEWS;
    *view = viewport_index % N_VIEWS;
}

  void  get_slice_viewport(
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

  void  get_slice_viewport_size(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_size,
    int          *y_size )
{
    int   x_min, x_max, y_min, y_max;
    get_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           &x_min, &x_max, &y_min, &y_max );

    *x_size = x_max - x_min + 1;
    *y_size = y_max - y_min + 1;
}

  void  set_slice_viewport(
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

    if( is_volume_active( main, volume ) )
        resize_slice( main, volume, view );
}

  void  set_volume_voxel_position(
    main_struct    *main,
    int            volume_index,
    VIO_Real           position[VIO_N_DIMENSIONS] )
{
    VIO_BOOL        changed;
    VIO_Volume         volume;
    int            view, axis, sizes[VIO_MAX_DIMENSIONS];
    VIO_Real           pos;
    VIO_Real           *cursor_ptr;

    volume = get_slice_volume( main, volume_index );
    get_volume_sizes( volume, sizes );
    cursor_ptr = get_volume_cursor( main, volume_index );

    changed = FALSE;

    for_less( axis, 0, VIO_N_DIMENSIONS )
    {
        if( position[axis] < -0.5 )
            pos = -0.5;
        else if( position[axis] >= (VIO_Real) sizes[axis] - 0.5 )
            pos = (VIO_Real) sizes[axis] - 0.500001;
        else
            pos = position[axis];

        if( pos != cursor_ptr[axis] )
        {
            changed = TRUE;

            cursor_ptr[axis] = pos;

            for_less( view, 0, N_VIEWS )
            {
                if( get_slice_axis( view ) == axis )
                {
                    set_recreate_slice_flag( main, volume_index, view );
                    update_slice_tag_objects( main, volume_index, view );
                }
            }
        }
    }

    if( changed )
        update_volume_cursors( main, volume_index );
}

  void  get_volume_voxel_position(
    main_struct   *main,
    int           volume,
    VIO_Real          position[VIO_N_DIMENSIONS] )
{
    VIO_Real           *cursor_ptr;
    VIO_Real           original_world_position1[VIO_N_DIMENSIONS];
    VIO_Real           original_world_position2[VIO_N_DIMENSIONS];
    VIO_Real           world_position[VIO_N_DIMENSIONS];

    if( volume <= MERGED_VOLUME_INDEX )
    {
        cursor_ptr = get_volume_cursor( main, volume );

        position[VIO_X] = cursor_ptr[VIO_X];
        position[VIO_Y] = cursor_ptr[VIO_Y];
        position[VIO_Z] = cursor_ptr[VIO_Z];
    }
    else
    {
        volume = volume - MERGED_VOLUME_INDEX;
        cursor_ptr = get_volume_cursor( main, MERGED_VOLUME_INDEX );

        convert_voxel_to_original_world( main, MERGED_VOLUME_INDEX,
                                         cursor_ptr[VIO_X],
                                         cursor_ptr[VIO_Y],
                                         cursor_ptr[VIO_Z],
                                         &original_world_position1[VIO_X],
                                         &original_world_position1[VIO_Y],
                                         &original_world_position1[VIO_Z] );
        convert_original_world_to_world( main, MERGED_VOLUME_INDEX,
                                         original_world_position1[VIO_X],
                                         original_world_position1[VIO_Y],
                                         original_world_position1[VIO_Z],
                                         &world_position[VIO_X],
                                         &world_position[VIO_Y],
                                         &world_position[VIO_Z] );
        convert_world_to_original_world( main, volume,
                                         world_position[VIO_X],
                                         world_position[VIO_Y],
                                         world_position[VIO_Z],
                                         &original_world_position2[VIO_X],
                                         &original_world_position2[VIO_Y],
                                         &original_world_position2[VIO_Z] );
        convert_original_world_to_voxel( main, volume,
                                         original_world_position2[VIO_X],
                                         original_world_position2[VIO_Y],
                                         original_world_position2[VIO_Z],
                                         &position[VIO_X],
                                         &position[VIO_Y],
                                         &position[VIO_Z] );
    }
}

  void  convert_original_world_to_world(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_original,
    VIO_Real           y_original,
    VIO_Real           z_original,
    VIO_Real           *x_world,
    VIO_Real           *y_world,
    VIO_Real           *z_world )
{
    VIO_General_transform  *transform;

    if( volume_index == RESAMPLED_VOLUME_INDEX &&
        get_tag_point_transform( main, &transform ) )
    {
        general_transform_point( transform, x_original, y_original, z_original,
                                 x_world, y_world, z_world );
    }
    else
    {
        *x_world = x_original;
        *y_world = y_original;
        *z_world = z_original;
    }
}

  void  convert_world_to_original_world(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_world,
    VIO_Real           y_world,
    VIO_Real           z_world,
    VIO_Real           *x_original,
    VIO_Real           *y_original,
    VIO_Real           *z_original )
{
    VIO_General_transform  *transform;

    if( volume_index == RESAMPLED_VOLUME_INDEX &&
        get_tag_point_transform( main, &transform ) )
    {
        general_inverse_transform_point( transform, x_world, y_world, z_world,
                                         x_original, y_original, z_original );
    }
    else
    {
        *x_original = x_world;
        *y_original = y_world;
        *z_original = z_world;
    }
}

  void  set_volume_world_position(
    main_struct    *main,
    int            volume_index,
    VIO_Real           world_position[VIO_N_DIMENSIONS] )
{
    VIO_Real           original_world[VIO_N_DIMENSIONS];
    VIO_Real           voxel_position[VIO_N_DIMENSIONS];

    convert_world_to_original_world( main, volume_index,
                                     world_position[VIO_X],
                                     world_position[VIO_Y],
                                     world_position[VIO_Z],
                                     &original_world[VIO_X],
                                     &original_world[VIO_Y],
                                     &original_world[VIO_Z] );
    convert_original_world_to_voxel( main, volume_index,
                                     original_world[VIO_X],
                                     original_world[VIO_Y],
                                     original_world[VIO_Z],
                                     &voxel_position[VIO_X],
                                     &voxel_position[VIO_Y],
                                     &voxel_position[VIO_Z] );

    set_volume_voxel_position( main, volume_index, voxel_position );
}

  void  get_volume_world_position(
    main_struct   *main,
    int           volume_index,
    VIO_Real          world_position[VIO_N_DIMENSIONS] )
{
    VIO_Real           voxel_position[VIO_N_DIMENSIONS];
    VIO_Real           original_world[VIO_N_DIMENSIONS];

    get_volume_voxel_position( main, volume_index, voxel_position );

    convert_voxel_to_original_world( main, volume_index,
                                     voxel_position[VIO_X],
                                     voxel_position[VIO_Y],
                                     voxel_position[VIO_Z],
                                     &original_world[VIO_X],
                                     &original_world[VIO_Y],
                                     &original_world[VIO_Z] );
    convert_original_world_to_world( main, volume_index,
                                     original_world[VIO_X],
                                     original_world[VIO_Y],
                                     original_world[VIO_Z],
                                     &world_position[VIO_X],
                                     &world_position[VIO_Y],
                                     &world_position[VIO_Z] );
}

  void  convert_original_world_to_voxel(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_original,
    VIO_Real           y_original,
    VIO_Real           z_original,
    VIO_Real           *x_voxel,
    VIO_Real           *y_voxel,
    VIO_Real           *z_voxel )
{
    VIO_Real   x_world, y_world, z_world, voxel[VIO_MAX_DIMENSIONS];

    if( volume_index == RESAMPLED_VOLUME_INDEX && main->resampled_file_loaded )
    {
        general_transform_point( &main->resampling_transform,
                                 x_original, y_original, z_original,
                                 &x_world, &y_world, &z_world );
    }
    else
    {
        x_world = x_original;
        y_world = y_original;
        z_world = z_original;
    }

    convert_world_to_voxel( get_slice_volume(main,volume_index),
                            x_world, y_world, z_world, voxel );

    *x_voxel = voxel[VIO_X];
    *y_voxel = voxel[VIO_Y];
    *z_voxel = voxel[VIO_Z];
}

  void  convert_voxel_to_original_world(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_voxel,
    VIO_Real           y_voxel,
    VIO_Real           z_voxel,
    VIO_Real           *x_original,
    VIO_Real           *y_original,
    VIO_Real           *z_original )
{
    VIO_Real   voxel[VIO_MAX_DIMENSIONS], world_position[VIO_N_DIMENSIONS];

    voxel[VIO_X] = x_voxel;
    voxel[VIO_Y] = y_voxel;
    voxel[VIO_Z] = z_voxel;

    convert_voxel_to_world( get_slice_volume(main,volume_index),
                            voxel,
                            &world_position[VIO_X],
                            &world_position[VIO_Y],
                            &world_position[VIO_Z] );

    if( volume_index == RESAMPLED_VOLUME_INDEX && main->resampled_file_loaded )
    {
        general_inverse_transform_point( &main->resampling_transform,
                        world_position[VIO_X], world_position[VIO_Y], world_position[VIO_Z],
                        x_original, y_original, z_original );
    }
    else
    {
        *x_original = world_position[VIO_X];
        *y_original = world_position[VIO_Y];
        *z_original = world_position[VIO_Z];
    }
}

  void  set_volume_original_world_position(
    main_struct    *main,
    int            volume_index,
    VIO_Real           original_world_position[VIO_N_DIMENSIONS] )
{
    VIO_Real           voxel_position[VIO_N_DIMENSIONS];

    convert_original_world_to_voxel( main, volume_index,
                                     original_world_position[VIO_X],
                                     original_world_position[VIO_Y],
                                     original_world_position[VIO_Z],
                                     &voxel_position[VIO_X],
                                     &voxel_position[VIO_Y],
                                     &voxel_position[VIO_Z] );

    set_volume_voxel_position( main, volume_index, voxel_position );
}


  void  get_volume_time_position(main_struct *main_ptr,
                                       int volume_index,
                                       VIO_Real *t_voxel_ptr)
{
    *t_voxel_ptr = main_ptr->trislice[volume_index].position[VIO_T];
}

  void  set_volume_time_position(main_struct *main_ptr,
                                       int volume_index,
                                       VIO_Real t_voxel)
{
    int sizes[VIO_MAX_DIMENSIONS];
    VIO_Volume volume;

    volume = get_slice_volume( main_ptr, volume_index );

    /* If we're dealing with a 3D volume, assume there is no time
     * dimension.
     */
    if (volume_has_time_axis( main_ptr, volume_index ))
    {
        get_volume_sizes( volume, sizes );

        /* Force time within range.
         */
        if (t_voxel < 0.0) {
            t_voxel = 0;
        }

        if (t_voxel >= (VIO_Real) sizes[VIO_T]) {
            t_voxel = (VIO_Real) sizes[VIO_T] - 1.0;
        }

        main_ptr->trislice[volume_index].position[VIO_T] = t_voxel;

        set_recreate_3_slices_flags(main_ptr, volume_index);

        set_recreate_3_slices_flags(main_ptr, MERGED_VOLUME_INDEX);
    }
}


  void  get_volume_original_world_position(
    main_struct   *main,
    int           volume_index,
    VIO_Real          original_world_position[VIO_N_DIMENSIONS] )
{
    VIO_Real           voxel_position[VIO_N_DIMENSIONS];

    get_volume_voxel_position( main, volume_index, voxel_position );

    convert_voxel_to_original_world( main, volume_index,
                                     voxel_position[VIO_X],
                                     voxel_position[VIO_Y],
                                     voxel_position[VIO_Z],
                                     &original_world_position[VIO_X],
                                     &original_world_position[VIO_Y],
                                     &original_world_position[VIO_Z] );
}

  VIO_Volume  get_slice_volume(
    main_struct   *main,
    int           volume_index )
{
    if( volume_index < main->n_volumes_displayed - 1 )
        return( main->trislice[volume_index].volume );
    else
        return( main->trislice[volume_index-MERGED_VOLUME_INDEX].volume );
}

  void  get_volume_range_of_voxels(
    main_struct   *main,
    int           volume_index,
    int           *min_value,
    int           *max_value )
{
    VIO_Real    real_min, real_max;
    VIO_Volume  volume;

    *min_value = 0;
    *max_value = -1;

    if( is_volume_active( main, volume_index ) )
    {
        volume = get_slice_volume( main, volume_index );

        if( !is_an_rgb_volume(volume) )
        {
            get_volume_voxel_range( volume, &real_min, &real_max );
            *min_value = VIO_ROUND( real_min );
            *max_value = VIO_ROUND( real_max );
        }
    }
}

  void  get_volume_value_range(
    main_struct   *main,
    int           volume_index,
    VIO_Real          *min_value,
    VIO_Real          *max_value )
{
    get_volume_real_range( get_slice_volume(main,volume_index),
                           min_value, max_value );
}

  VIO_Real  *get_volume_cursor(
    main_struct   *main,
    int           volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.position );
    else
        return( main->trislice[volume_index].position );
}

  slice_struct  *get_slice_struct(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( &main->merged.slices[view] );
    else
        return( &main->trislice[volume_index].slices[view] );
}

  void  get_slice_transform(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          *x_trans,
    VIO_Real          *y_trans,
    VIO_Real          *x_scale,
    VIO_Real          *y_scale )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    *x_trans = slice->x_translation;
    *y_trans = slice->y_translation;
    *x_scale = slice->x_scale;
    *y_scale = slice->y_scale;
}

  void  set_slice_translation(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          x_trans,
    VIO_Real          y_trans )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->x_translation = x_trans;
    slice->y_translation = y_trans;
}

  void  set_slice_scale(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          x_scale,
    VIO_Real          y_scale )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->x_scale = x_scale;
    slice->y_scale = y_scale;
}

  VIO_Real  get_voxel_value(
    main_struct   *main,
    int           volume_index,
    VIO_Real          x_voxel,
    VIO_Real          y_voxel,
    VIO_Real          z_voxel )
{
    VIO_Real           value, position[VIO_MAX_DIMENSIONS];
    VIO_Volume         volume;
    VIO_Real           t_voxel;

    volume = get_slice_volume( main, volume_index );

    t_voxel = main->trislice[volume_index].position[VIO_T];

    position[VIO_X] = x_voxel;
    position[VIO_Y] = y_voxel;
    position[VIO_Z] = z_voxel;
    position[VIO_T] = t_voxel;

    if( voxel_is_within_volume( volume, position ) )
    {
        value = get_volume_real_value( volume, VIO_ROUND(x_voxel), 
                                       VIO_ROUND(y_voxel), VIO_ROUND(z_voxel), 
                                       VIO_ROUND(t_voxel), 0 );
    }
    else
        value = 0.0;

    return( value );
}

  void  set_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Filter_types  filter_type )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->filter_type = filter_type;

    set_recreate_slice_flag( main, volume_index, view );
}

  VIO_Filter_types  get_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    return( slice->filter_type );
}

  void  set_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          filter_width )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->filter_width = filter_width;

    set_recreate_slice_flag( main, volume_index, view );
}

  VIO_Real  get_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    return( slice->filter_width );
}

  void  set_interpolation_mode(
    main_struct   *main,
    VIO_BOOL       smooth_flag )
{
    int   volume;

    if( smooth_flag )
        main->degrees_continuity = 0;
    else
        main->degrees_continuity = -1;

    for_less( volume, 0, main->n_volumes_displayed )
        set_recreate_3_slices_flags( main, volume );
}

  VIO_BOOL  get_interpolation_mode(
    main_struct   *main )
{
    return( main->degrees_continuity == 0 );
}
