#include <def_register.h>

public  void  set_recreate_3_slices_flags(
    main_struct  *main,
    int          volume_index )
{
    int  view;

    for_less( view, 0, N_VIEWS )
        set_recreate_slice_flag( main, volume_index, view );
}

public  void  set_recreate_slice_flag(
    main_struct  *main,
    int          volume,
    int          view )
{
    main->trislice[volume].slices[view].pixels_are_up_to_date = FALSE;
}

public  void  set_update_slice_viewport_flag(
    main_struct     *main,
    int             volume,
    int             view,
    Bitplane_types  bitplane )
{
    set_viewport_update_flag( &main->graphics,
                              get_slice_viewport_index(volume,view),
                              bitplane );
}

public  void  set_volume_voxel_position(
    main_struct    *main,
    int            volume,
    Real           position[N_DIMENSIONS] )
{
    int   view;
    Real  pos;

    for_less( view, 0, N_VIEWS )
    {
        if( position[view] < -0.5 )
            pos = -0.5;
        else if( position[view] >
                 (Real) main->trislice[volume].volume.sizes[view] - 0.5 )
            pos = (Real) main->trislice[volume].volume.sizes[view] - 0.5;
        else
            pos = position[view];

        if( pos != main->trislice[volume].position[view] )
        {
            set_recreate_slice_flag( main, volume, view );
            main->trislice[volume].position[view] = pos;
        }

        update_volume_cursors( main, volume );
    }
}

public  void  get_volume_voxel_position(
    main_struct   *main,
    int           volume,
    Real          position[N_DIMENSIONS] )
{
    position[X] = main->trislice[volume].position[X];
    position[Y] = main->trislice[volume].position[Y];
    position[Z] = main->trislice[volume].position[Z];
}

public  void  set_volume_world_position(
    main_struct    *main,
    int            volume,
    Real           world_position[N_DIMENSIONS] )
{
    Real  voxel_position[N_DIMENSIONS];

    convert_world_to_voxel( &main->trislice[volume].volume,
                            world_position[X],
                            world_position[Y],
                            world_position[Z],
                            &voxel_position[X],
                            &voxel_position[Y],
                            &voxel_position[Z] );

    set_volume_voxel_position( main, volume, voxel_position );
}

public  void  get_volume_world_position(
    main_struct   *main,
    int           volume,
    Real          world_position[N_DIMENSIONS] )
{
    Real  voxel_position[N_DIMENSIONS];

    get_volume_voxel_position( main, volume, voxel_position );

    convert_voxel_to_world( &main->trislice[volume].volume,
                            voxel_position[X],
                            voxel_position[Y],
                            voxel_position[Z],
                            &world_position[X],
                            &world_position[Y],
                            &world_position[Z] );
}
