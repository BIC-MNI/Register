#include  <user_interface.h>

public  BOOLEAN  make_window_up_to_date(
    window_struct    *window,
    graphics_struct  *graphics,
    int              current_buffer )
{
    BOOLEAN          something_was_drawn;

    something_was_drawn = redraw_out_of_date_viewports( graphics, window,
                                                        current_buffer );

    if( window == get_ui_struct()->graphics_window.window &&
        IF_redraw_slices( current_buffer ) )
        something_was_drawn = TRUE;

    if( something_was_drawn )
    {
        G_set_bitplanes( window, NORMAL_PLANES );
        G_update_window( window );

        if( G_has_overlay_planes() )
        {
            G_set_bitplanes( window, OVERLAY_PLANES );
            G_update_window( window );
            G_set_bitplanes( window, NORMAL_PLANES ); /* leave in normal mode */
        }
    }

    return( something_was_drawn );
}

public  void  set_clear_and_update_flags( UI_struct  *ui_struct )
{
    int             volume, view;
    Bitplane_types  bitplane;
    Viewport_types  viewport;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &ui_struct->graphics_window.graphics,
                                  bitplane );

        for_enum( viewport, N_UI_viewports, Viewport_types )
        {
            set_viewport_update_flag( &ui_struct->graphics_window.graphics,
                                      viewport, bitplane );
        }
    }

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
                IF_set_update_slice_viewport_flag( volume, view, bitplane );
        }
    }

    ui_struct->graphics_window.current_buffer = 0;
}

public  void  set_recreate_all_slice_flags()
{
    int             volume, view;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            IF_set_recreate_slice_flag( volume, view );
        }
    }
}
