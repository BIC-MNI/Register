#include <register.h>

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
    if( volume == MERGED_VOLUME_INDEX )
    {
        main->merged.slices[view].pixels_are_up_to_date = FALSE;
    }
    else
    {
        main->trislice[volume].slices[view].pixels_are_up_to_date = FALSE;
    }
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
