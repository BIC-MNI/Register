#include  <def_register.h>

public  Boolean  update_slice_display(
    main_struct     *main,
    window_struct   *window,
    int             current_buffer )
{
    Boolean   drawn;
    int       volume, view;

    for_less( volume, 0, N_VOLUMES )
    {
        if( main->trislice[volume].input_flag )
        {
            for_less( view, 0, N_VIEWS )
            {
                if( !main->trislice[volume].slices[view].pixels_are_up_to_date )
                {
                    create_slice_pixels( main, volume, view );
                    main->trislice[volume].slices[view].pixels_are_up_to_date =
                                                                TRUE;

                    set_viewport_update_flag( &main->graphics,
                               get_slice_viewport_index(volume,view),
                               NORMAL_PLANES );
                }
            }
        }
    }

    drawn = redraw_out_of_date_viewports( &main->graphics, window,
                                          current_buffer );

    return( drawn );
}
