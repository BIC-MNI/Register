#include  <def_register.h>

private  void  update_graphics_window( main_struct * );
private  void  draw_slice( main_struct *, slice_struct * );

public  void  make_window_up_to_date( main_struct  *main )
{
    if( main->window_update_required )
    {
        update_graphics_window( main );
        main->window_update_required = FALSE;
    }
}

private  void  update_graphics_window( main_struct *main )
{
    int  view, volume;

    if( main->update_all_flag[main->current_buffer] )
    {
        G_clear_window( main->graphics_window.window );
        main->update_all_flag[main->current_buffer] = FALSE;
    }

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
                }

                if( !main->trislice[volume].slices[view].buffer_is_up_to_date[
                         main->current_buffer] )
                {
                    draw_slice( main, &main->trislice[volume].slices[view] );

                    main->trislice[volume].slices[view].buffer_is_up_to_date[
                         main->current_buffer] = TRUE;
                }
            }
        }
    }

    G_update_window( main->graphics_window.window );

    if( main->double_buffer_mode )
        main->current_buffer = 1 - main->current_buffer;
}

private  void  draw_slice(
    main_struct   *main,
    slice_struct  *slice )
{
    G_set_viewport( main->graphics_window.window, slice->x_min, slice->x_max,
                    slice->y_min, slice->y_max );

    G_clear_viewport( main->graphics_window.window );

    G_draw_pixels( main->graphics_window.window,
                   slice->x_min + slice->x_position,
                   slice->y_min + slice->y_position,
                   0, &slice->pixels );
}
