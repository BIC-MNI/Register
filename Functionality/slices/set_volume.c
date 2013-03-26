
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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/slices/set_volume.c,v 1.20 2005-02-28 21:05:18 bert Exp $";
#endif

#include  <register.h>

public  VIO_BOOL  is_volume_active(
    main_struct    *main,
    int            volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.active_flag );
    else
        return( main->trislice[volume_index].input_flag );
}

public  VIO_BOOL  is_resampled_volume_loaded(
    main_struct    *main )
{
    return( main->resampled_file_loaded );
}

public  VIO_STR  get_volume_filename(
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

private  void   record_register_volume(
    main_struct    *main,
    int            volume_index,
    VIO_Volume         volume,
    VIO_STR         filename )
{
    int     view, axis, sizes[VIO_N_DIMENSIONS];
    VIO_Real    min_value, max_value, *cursor_ptr;

    if( is_volume_active( main, volume_index ) )
    {
        delete_register_volume( main, volume_index );
    }

    main->trislice[volume_index].volume = volume;
    main->trislice[volume_index].input_flag = TRUE;

    replace_string( &main->trislice[volume_index].filename,
                    create_string(filename) );

    get_volume_sizes( volume, sizes );

    cursor_ptr = get_volume_cursor( main, volume_index );

    for_less( axis, 0, VIO_N_DIMENSIONS )
        cursor_ptr[axis] = (VIO_Real) (sizes[axis]-1) / 2.0;

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                              get_slice_viewport_index(volume_index,view), ON );

        initialize_slice_view( main, volume_index, view );
        update_slice_tag_objects( main, volume_index, view );
        update_volume_cursors( main, volume_index );
        set_recreate_slice_flag( main, volume_index, view );
    }

    if( volume_index < N_VOLUMES )
    {
        get_volume_value_range( main, volume_index, &min_value, &max_value );

        set_volume_colour_coding_limits( main, volume_index,
                                         min_value, max_value );
    }

    if( G_get_colour_map_state(main->window) )
        repartition_colour_maps( main );

    update_colour_maps( main, volume_index );
}

public  void   set_register_volume(
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

public  void   set_register_resampled_volume(
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

public  void  delete_register_volume(
    main_struct    *main,
    int            volume_index )
{
    int            view;

    delete_volume( main->trislice[volume_index].volume );
    main->trislice[volume_index].input_flag = FALSE;

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                                get_slice_viewport_index(volume_index,view),
                                FALSE );
    }
}

private  void  set_merged_volume_visibility(
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

public  void  set_merged_volume_activity(
    main_struct    *main,
    VIO_BOOL        activity )
{
    int     view, axis, sizes[VIO_N_DIMENSIONS];
    VIO_Real    position[VIO_N_DIMENSIONS];

    if( activity )
    {
        for_less( view, 0, N_VIEWS )
            initialize_slice_view( main, MERGED_VOLUME_INDEX, view );

        get_volume_sizes( get_slice_volume(main,0), sizes );

        for_less( axis, 0, VIO_N_DIMENSIONS )
            position[axis] = (VIO_Real) (sizes[axis]-1) / 2.0;

        set_volume_voxel_position( main, MERGED_VOLUME_INDEX, position );
    }

    main->merged.active_flag = activity;

    set_merged_volume_visibility( main, activity );

    update_volume_tag_objects( main, MERGED_VOLUME_INDEX );

    if( activity )
        update_colour_maps( main, MERGED_VOLUME_INDEX );

    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

public  VIO_BOOL  get_merged_volume_activity(
    main_struct    *main )
{
    return( main->merged.active_flag );
}

char *XYZT_dimension_names[] = { MIxspace, MIyspace, MIzspace, MItime };

public  VIO_Status  start_loading_volume(
    main_struct    *main,
    int            volume_index,
    VIO_STR         filename )
{
    VIO_Status                 status;
    minc_input_options     options;

    set_default_minc_input_options( &options );

    set_minc_input_vector_to_colour_flag( &options, Convert_vectors_to_rgb );

    status = start_volume_input( filename, 0, XYZT_dimension_names,
                                 VOXEL_TYPE, FALSE, 0.0, 0.0, TRUE,
                              &main->trislice[volume_index].volume_being_input,
                              &options,
                              &main->trislice[volume_index].volume_input );

    if( status == OK &&
        is_an_rgb_volume(main->trislice[volume_index].volume_being_input) &&
        G_get_colour_map_state( main->window ) )
    {
        print( "Warning: reading rgb volume as intensity volume.\n" );
        print( "         This is because the program is in colour map mode.\n");

        cancel_loading_volume( main, volume_index );

        set_minc_input_vector_to_colour_flag( &options, FALSE );

        status = start_volume_input( filename, 0, XYZT_dimension_names,
                                     VOXEL_TYPE, FALSE, 0.0, 0.0, TRUE,
                              &main->trislice[volume_index].volume_being_input,
                              &options,
                              &main->trislice[volume_index].volume_input );
    }

    return( status );
}

public  VIO_BOOL  load_more_of_volume(
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

public  void  cancel_loading_volume(
    main_struct    *main,
    int            volume_index )
{
    delete_volume_input( &main->trislice[volume_index].volume_input );
    delete_volume( main->trislice[volume_index].volume_being_input );
}

public  VIO_BOOL  is_volume_rgb(
    main_struct    *main,
    int            volume_index )
{
    return( is_volume_active(main,volume_index) &&
            is_an_rgb_volume(get_slice_volume(main,volume_index)) );
}
