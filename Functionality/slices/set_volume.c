#include  <def_register.h>

public  Boolean  is_volume_active(
    main_struct    *main,
    int            volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.active_flag );
    else
        return( main->trislice[volume_index].input_flag );
}

public  void   set_register_volume(
    main_struct    *main,
    int            volume_index,
    volume_struct  *volume )
{
    int     view, axis;
    Real    position[N_DIMENSIONS];

    if( is_volume_active( main, volume_index ) )
    {
        delete_register_volume( main, volume_index );
    }

    main->trislice[volume_index].volume = *volume;
    main->trislice[volume_index].input_flag = TRUE;

    for_less( axis, 0, N_DIMENSIONS )
    {
        position[axis] =
              (Real) (main->trislice[volume_index].volume.sizes[axis]-1) / 2.0;
    }

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                                get_slice_viewport_index(volume_index,view),
                                ON );

        reset_slice_view( main, volume_index, view );

        set_recreate_slice_flag( main, volume_index, view );
    }

    set_volume_voxel_position( main, volume_index, position );

    update_colour_maps( main, volume_index );
}

public  void  delete_register_volume(
    main_struct    *main,
    int            volume_index )
{
    int            view;

    delete_volume( &main->trislice[volume_index].volume );
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
    Boolean        visible )
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
    Boolean        activity )
{
    int     view, axis;
    Real    position[N_DIMENSIONS];

    if( activity && !main->merged.active_flag )
    {
        for_less( view, 0, N_VIEWS )
        {
            reset_slice_view( main, MERGED_VOLUME_INDEX, view );
        }

        for_less( axis, 0, N_DIMENSIONS )
        {
            position[axis] =
              (Real) (main->trislice[0].volume.sizes[axis]-1) / 2.0;
        }

        set_volume_voxel_position( main, MERGED_VOLUME_INDEX, position );
    }

    main->merged.active_flag = activity;

    set_merged_volume_visibility( main, activity );

    if( activity )
        update_colour_maps( main, MERGED_VOLUME_INDEX );
}

public  Boolean  get_merged_volume_activity(
    main_struct    *main )
{
    return( main->merged.active_flag );
}
