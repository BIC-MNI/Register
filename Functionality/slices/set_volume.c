#include  <register.h>

public  BOOLEAN  is_volume_active(
    main_struct    *main,
    int            volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.active_flag );
    else
        return( main->trislice[volume_index].input_flag );
}

public  BOOLEAN  is_resampled_volume_loaded(
    main_struct    *main )
{
    return( main->resampled_file_loaded );
}

public  char  *get_volume_filename(
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
    Volume         volume,
    char           filename[] )
{
    int     view, axis, sizes[N_DIMENSIONS];
    Real    min_value, max_value, position[N_DIMENSIONS];

    if( is_volume_active( main, volume_index ) )
    {
        delete_register_volume( main, volume_index );
    }

    main->trislice[volume_index].volume = volume;
    main->trislice[volume_index].input_flag = TRUE;

    (void) strcpy( main->trislice[volume_index].filename, filename );

    get_volume_sizes( volume, sizes );

    for_less( axis, 0, N_DIMENSIONS )
    {
        position[axis] = (Real) (sizes[axis]-1) / 2.0;
    }

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                                get_slice_viewport_index(volume_index,view),
                                ON );

        initialize_slice_view( main, volume_index, view );

        set_recreate_slice_flag( main, volume_index, view );
    }

    set_volume_voxel_position( main, volume_index, position );

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
    Volume         volume,
    char           filename[] )
{
    record_register_volume( main, volume_index, volume, filename );

    main->resampled_file_loaded = FALSE;
}

public  void   set_register_resampled_volume(
    main_struct            *main,
    Volume                 volume,
    char                   filename[],
    char                   original_filename[],
    General_transform      *resampling_transform )
{
    record_register_volume( main, RESAMPLED_VOLUME_INDEX, volume, filename );

    main->resampled_file_loaded = TRUE;

    delete_general_transform( &main->resampling_transform );
    copy_general_transform( resampling_transform, &main->resampling_transform );
    (void) strcpy( main->original_volume_filename, original_filename );
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
    BOOLEAN        visible )
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
    BOOLEAN        activity )
{
    int     view, axis, sizes[N_DIMENSIONS];
    Real    position[N_DIMENSIONS];

    if( activity && !main->merged.active_flag )
    {
        for_less( view, 0, N_VIEWS )
            initialize_slice_view( main, MERGED_VOLUME_INDEX, view );

        get_volume_sizes( get_slice_volume(main,0), sizes );

        for_less( axis, 0, N_DIMENSIONS )
        {
            position[axis] = (Real) (sizes[axis]-1) / 2.0;
        }

        set_volume_voxel_position( main, MERGED_VOLUME_INDEX, position );
    }

    main->merged.active_flag = activity;

    set_merged_volume_visibility( main, activity );

    update_volume_tag_objects( main, MERGED_VOLUME_INDEX );

    if( activity )
        update_colour_maps( main, MERGED_VOLUME_INDEX );

    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

public  BOOLEAN  get_merged_volume_activity(
    main_struct    *main )
{
    return( main->merged.active_flag );
}
