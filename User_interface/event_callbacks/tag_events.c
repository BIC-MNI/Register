#include  <user_interface.h>

private  DEFINE_EVENT_FUNCTION( add_tag_point_callback )   /* ARGSUSED */
{
    record_tag( get_ui_struct(), IF_get_n_tag_points() );
}

public  void  install_tag_events(
    UI_struct                *ui )
{
    int             volume, view;
    Viewport_types  viewport;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            add_event_viewport_callback(
                   &ui->graphics_window.event_viewports,
                   ui_get_slice_viewport_index(volume,view),
                   KEY_DOWN_EVENT, -1, -1, -1, -1,
                   slice_key_down_callback, NO_SHIFT_ONLY, (void *) 0 );
            add_event_viewport_callback(
                   &ui->graphics_window.event_viewports,
                   ui_get_slice_viewport_index(volume,view),
                   RIGHT_MOUSE_DOWN_EVENT, -1, -1, -1, -1,
                   add_tag_point_callback, NO_SHIFT_ONLY, (void *) 0 );
        }
    }

    for_less( viewport, 0, N_UI_viewports )
    {
        add_event_viewport_callback(
               &ui->graphics_window.event_viewports, viewport,
               KEY_DOWN_EVENT, -1, -1, -1, -1,
               slice_key_down_callback, ANY_MODIFIER, (void *) 0 );
        add_event_viewport_callback(
               &ui->graphics_window.event_viewports, viewport,
               RIGHT_MOUSE_DOWN_EVENT, -1, -1, -1, -1,
               add_tag_point_callback, NO_SHIFT_ONLY, (void *) 0 );
    }
}
