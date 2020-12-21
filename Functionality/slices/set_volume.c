/**
 * \file set_volume.c
 * \brief Essential functions loading and manipulating volumes.
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
#include <float.h>


  VIO_BOOL  is_volume_active(
    main_struct    *main,
    int            volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.active_flag );
    else
        return( main->trislice[volume_index].input_flag );
}

  VIO_BOOL  is_resampled_volume_loaded(
    main_struct    *main )
{
    return( main->resampled_file_loaded );
}

  VIO_STR  get_volume_filename(
    main_struct    *main,
    int            volume_index )
{
    if( volume_index == RESAMPLED_VOLUME_INDEX &&
        is_resampled_volume_loaded(main) )
    {
        return( main->original_volume_filename );
    }
    else
        return( main->trislice[volume_index].filename );
}

/**
 * Compute the initial voxel position for either volume 0 or 1.
 *
 * Attempts to use world values from the global settings if they are
 * available, converting the into the voxel space and constraining
 * them to lie within the actual voxel space.
 *
 * Otherwise it falls back to using the midpoints of the voxel space.
 */
static void
compute_initial_voxel_position(VIO_Volume volume,
                               int volume_index,
                               VIO_Real initial_voxel[])
{
  VIO_Real world[VIO_N_DIMENSIONS];
  int      sizes[VIO_MAX_DIMENSIONS];
  VIO_Real voxel[VIO_MAX_DIMENSIONS];
  int      n_dims;
  int      i;
  VIO_BOOL is_set1;             /* True if volume 1 position specified. */
  VIO_BOOL is_set2;             /* True if volume 2 position specified. */

  n_dims = get_volume_n_dimensions( volume );
  get_volume_sizes( volume, sizes );

  /* Compute the default value we use if all else fails.
   */
  for (i = 0; i < n_dims; i++)
  {
    initial_voxel[i] = (sizes[i] - 1.0) / 2.0;
  }

  /* Non-spatial dimensions, if present, just default to zero.
   */
  while (i < VIO_MAX_DIMENSIONS)
  {
    initial_voxel[i++] = 0;
  }

  is_set1 = (Initial_volume_1_world_x < FLT_MAX ||
             Initial_volume_1_world_y < FLT_MAX ||
             Initial_volume_1_world_z < FLT_MAX);

  is_set2 = (Initial_volume_2_world_x < FLT_MAX ||
             Initial_volume_2_world_y < FLT_MAX ||
             Initial_volume_2_world_z < FLT_MAX);

  world[VIO_X] = world[VIO_Y] = world[VIO_Z] = 0;

  if (is_set1 && (volume_index == 0 || Initial_volumes_synced))
  {
    if (Initial_volume_1_world_x < FLT_MAX)
      world[VIO_X] = Initial_volume_1_world_x;
    if (Initial_volume_1_world_y < FLT_MAX)
      world[VIO_Y] = Initial_volume_1_world_y;
    if (Initial_volume_1_world_z < FLT_MAX)
      world[VIO_Z] = Initial_volume_1_world_z;
  }
  else if (is_set2 && (volume_index == 1 || Initial_volumes_synced))
  {
    if (Initial_volume_2_world_x < FLT_MAX)
      world[VIO_X] = Initial_volume_2_world_x;
    if (Initial_volume_2_world_y < FLT_MAX)
      world[VIO_Y] = Initial_volume_2_world_y;
    if (Initial_volume_2_world_z < FLT_MAX)
      world[VIO_Z] = Initial_volume_2_world_z;
  }
  else
  {
    return;                     /* Just use the default. */
  }

  convert_world_to_voxel(volume, world[VIO_X], world[VIO_Y], world[VIO_Z],
                         voxel);

  for (i = 0; i < n_dims; i++)
  {
    VIO_Real pos = voxel[i];

    if ( pos < -0.5 )
      pos = -0.5;

    if ( pos >= (VIO_Real) sizes[i] - 0.5 )
      pos = (VIO_Real) sizes[i] - 0.500001;

    initial_voxel[i] = pos;
  }
}

/**
 * Get the determinant of the 3x3 transform (i.e. the direction
 * cosines).
 *
 * This is useful, for example, in checking whether or not
 * we have a rigid body transform (the determinant of a rigid
 * transform is either 1.0 or -1.0).

 * \param volume The volume whose determinant we want.
 * \returns The determinant of the 3x3 matrix formed by the direction cosines.
 */
static VIO_Real get_volume_transform_determinant( VIO_Volume volume )
{
     VIO_Real m[VIO_N_DIMENSIONS][VIO_N_DIMENSIONS];
     int i;

     for_less( i, 0, VIO_N_DIMENSIONS )
         get_volume_direction_cosine( volume, i, m[i] );

     return (+ m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[2][1] * m[1][2]
             - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[2][1] * m[0][2]
             + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[1][1] * m[0][2] );
}

/**
 * Check whether this volume's voxel to world transform specifies a
 * rigid body transform.
 * \param volume The volume whose transform we want to check.
 * \returns TRUE if the transform is a rigid body transform.
 */
static VIO_BOOL is_volume_transform_rigid( VIO_Volume volume )
{
    VIO_Real det = get_volume_transform_determinant( volume );
    return (fabs( fabs( det ) - 1.0 ) < 1e-6 );
}

static  void   record_register_volume(
    main_struct    *main,
    int            volume_index,
    VIO_Volume         volume,
    VIO_STR         filename )
{
    int      view, axis;
    VIO_Real min_value, max_value, *cursor_ptr;
    VIO_Real initial_position[VIO_MAX_DIMENSIONS];

    if( is_volume_active( main, volume_index ) )
    {
        delete_register_volume( main, volume_index );
    }

    main->trislice[volume_index].volume = volume;
    main->trislice[volume_index].input_flag = TRUE;

    replace_string( &main->trislice[volume_index].filename,
                    create_string(filename) );

    compute_initial_voxel_position( volume, volume_index, initial_position );

    cursor_ptr = get_volume_cursor( main, volume_index );

    for_less( axis, 0, VIO_N_DIMENSIONS )
      cursor_ptr[axis] = initial_position[axis];

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                              get_slice_viewport_index(volume_index,view), TRUE );

        initialize_slice_view( main, volume_index, view );
        update_slice_tag_objects( main, volume_index, view );
        update_volume_cursors( main, volume_index );
        set_recreate_slice_flag( main, volume_index, view );
    }

    if( volume_index < N_VOLUMES )
    {
        if (volume_index == 0)
        {
            min_value = Volume_1_colour_coding_min;
            max_value = Volume_1_colour_coding_max;
        }
        else
        {
            min_value = Volume_2_colour_coding_min;
            max_value = Volume_2_colour_coding_max;
        }
        if (min_value >= max_value)
        {
            get_volume_value_range( main, volume_index,
                                    &min_value, &max_value );
        }

        set_volume_colour_coding_limits( main, volume_index,
                                         min_value, max_value );
    }

    if ( !is_volume_transform_rigid( volume ) )
    {
        char *message = {
            "WARNING: Volume transform is not rigid. This may cause distortion or\n"
            "         other odd effects if this volume is superimposed on another volume.\n"
        };
        printf( "%s", message );
    }
}


  void   set_register_volume(
    main_struct    *main,
    int            volume_index,
    VIO_STR         filename )
{
    record_register_volume( main, volume_index,
                            main->trislice[volume_index].volume_being_input,
                            filename );

    if( volume_index == RESAMPLED_VOLUME_INDEX )
        main->resampled_file_loaded = FALSE;
}

  void   set_register_resampled_volume(
    main_struct            *main,
    int                    volume_index,
    VIO_STR                 filename,
    VIO_STR                 original_filename,
    VIO_General_transform      *resampling_transform )
{
    record_register_volume( main, RESAMPLED_VOLUME_INDEX,
                            main->trislice[volume_index].volume_being_input,
                            filename );

    main->resampled_file_loaded = TRUE;

    delete_general_transform( &main->resampling_transform );
    copy_general_transform( resampling_transform, &main->resampling_transform );
    replace_string( &main->original_volume_filename,
                    create_string(original_filename) );
}

  void  delete_register_volume(
    main_struct    *main,
    int            volume_index )
{
    int            view;

    delete_volume( main->trislice[volume_index].volume );
    main->trislice[volume_index].input_flag = FALSE;

    if (main->trislice[volume_index].rgb_colour_map != NULL)
    {
        free(main->trislice[volume_index].rgb_colour_map +
             main->trislice[volume_index].rgb_colour_offset);

        main->trislice[volume_index].rgb_colour_map = NULL;
        main->trislice[volume_index].rgb_colour_offset = 0;
    }

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                                get_slice_viewport_index(volume_index,view),
                                FALSE );
    }
}

static  void  set_merged_volume_visibility(
    main_struct    *main,
    VIO_BOOL        visible )
{
    int   view;

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                           get_slice_viewport_index(MERGED_VOLUME_INDEX,view),
                           visible );
    }
}

  void  set_merged_volume_activity(
    main_struct    *main,
    VIO_BOOL        activity )
{
    int      view, axis;
    VIO_Real position[VIO_N_DIMENSIONS];

    if( activity )
    {
        for_less( view, 0, N_VIEWS )
            initialize_slice_view( main, MERGED_VOLUME_INDEX, view );

        for_less( axis, 0, VIO_N_DIMENSIONS )
            position[axis] = main->trislice[0].position[axis];

        main->merged.active_flag = TRUE;

        set_volume_voxel_position( main, MERGED_VOLUME_INDEX, position );
    }
    else
    {
        main->merged.active_flag = FALSE;
    }

    set_merged_volume_visibility( main, activity );

    update_volume_tag_objects( main, MERGED_VOLUME_INDEX );

    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

  VIO_BOOL  get_merged_volume_activity(
    main_struct    *main )
{
    return( main->merged.active_flag );
}

char *XYZT_dimension_names[] = { MIxspace, MIyspace, MIzspace, MItime, MIvector_dimension };

  VIO_Status  start_loading_volume(
    main_struct    *main,
    int            volume_index,
    VIO_STR         filename )
{
    VIO_Status                 status;
    minc_input_options     options;

    set_default_minc_input_options( &options );

    set_minc_input_vector_to_colour_flag( &options, Convert_vectors_to_rgb );

    status = start_volume_input( filename, 0, XYZT_dimension_names,
                              Volume_voxel_type, Volume_voxel_signed,
                              0.0, 0.0, TRUE,
                              &main->trislice[volume_index].volume_being_input,
                              &options,
                              &main->trislice[volume_index].volume_input );
    return( status );
}

  VIO_BOOL  load_more_of_volume(
    main_struct    *main,
    int            volume_index,
    VIO_Real           max_time,
    VIO_Real           *fraction_done )
{
    VIO_BOOL       done_loading;
    VIO_Real          end_time;

    end_time = current_realtime_seconds() + max_time;

    do
    {
        done_loading = !input_more_of_volume(
                              main->trislice[volume_index].volume_being_input,
                              &main->trislice[volume_index].volume_input,
                              fraction_done );
    }
    while( !done_loading && current_realtime_seconds() < end_time );

    if( done_loading )
    {
        delete_volume_input( &main->trislice[volume_index].volume_input );
    }

    return( done_loading );
}

  void  cancel_loading_volume(
    main_struct    *main,
    int            volume_index )
{
    delete_volume_input( &main->trislice[volume_index].volume_input );
    delete_volume( main->trislice[volume_index].volume_being_input );
}

  VIO_BOOL  is_volume_rgb(
    main_struct    *main,
    int            volume_index )
{
    return( is_volume_active(main,volume_index) &&
            is_an_rgb_volume(get_slice_volume(main,volume_index)) );
}

VIO_BOOL  volume_has_time_axis(
    main_struct    *main,
    int            volume_index )
{
    return( is_volume_active( main, volume_index ) &&
            get_volume_n_dimensions( get_slice_volume( main, volume_index ) ) > 3 );
}
