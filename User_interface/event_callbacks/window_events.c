#include  <user_interface.h>

private  DEFINE_EVENT_FUNCTION( redraw_overlay_callback )   /* ARGSUSED */
{
    set_clear_and_update_flags( get_ui_struct() );
}

private  DEFINE_EVENT_FUNCTION( redraw_window_callback )   /* ARGSUSED */
{
    set_clear_and_update_flags( get_ui_struct() );
}

private  DEFINE_EVENT_FUNCTION( resize_window_callback )   /* ARGSUSED */
{
    resize_layout( get_ui_struct() );

    position_main_widgets( get_ui_struct() );

    set_recreate_all_slice_flags();

    set_clear_and_update_flags( get_ui_struct() );
}

private  DEFINE_EVENT_FUNCTION( quit_window_callback )   /* ARGSUSED */
{
    popup_quit_confirm( get_ui_struct() );
}

/* ---- a hook to force update of colour map, when other program has
        screwed up the colour map ------------------------------------ */

private  DEFINE_EVENT_FUNCTION( deiconize_window_callback )   /* ARGSUSED */
{
    set_window_colours( get_ui_struct()->graphics_window.window );
}

public  void  install_window_events(
    UI_struct  *ui )
{
    set_event_viewport( &ui->graphics_window.event_viewports,
                        Whole_window_event_viewport, -1, -1, -1, -1 );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 REDRAW_OVERLAY_EVENT, -1, -1, -1, -1,
                                 redraw_overlay_callback, ANY_MODIFIER,
                                 (void *) NULL );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_REDRAW_EVENT, -1, -1, -1, -1,
                                 redraw_window_callback, ANY_MODIFIER,
                                 (void *) NULL );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_RESIZE_EVENT, -1, -1, -1, -1,
                                 resize_window_callback, ANY_MODIFIER,
                                 (void *) NULL );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_QUIT_EVENT, -1, -1, -1, -1,
                                 quit_window_callback, ANY_MODIFIER,
                                 (void *) NULL );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_DEICONIZED_EVENT, -1, -1, -1, -1,
                                 deiconize_window_callback, ANY_MODIFIER,
                                 (void *) NULL );
}
