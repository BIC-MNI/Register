#include  <def_register.h>

public  Boolean  is_volume_loaded(
    main_struct    *main,
    int            volume_index )
{
    return( main->trislice[volume_index].input_flag );
}

public  void   set_register_volume(
    main_struct    *main,
    int            volume_index,
    volume_struct  *volume )
{
    int     view;
    Real    position[N_DIMENSIONS];

    if( is_volume_loaded( main, volume_index ) )
    {
        delete_register_volume( main, volume_index );
    }

    main->trislice[volume_index].volume = *volume;
    main->trislice[volume_index].input_flag = TRUE;

    for_less( view, 0, N_VIEWS )
    {
        set_viewport_objects_visibility( &main->graphics,
                                get_slice_viewport_index(volume_index,view),
                                ON );

        position[view] =
              (Real) (main->trislice[volume_index].volume.sizes[view]-1) / 2.0;

        reset_slice_view( main, volume_index, view );

        set_recreate_slice_flag( main, volume_index, view );
    }

    set_volume_voxel_position( main, volume_index, position );
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
