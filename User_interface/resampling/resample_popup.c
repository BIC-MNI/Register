#include  <def_user_interface.h>

private  DEFINE_WIDGET_CALLBACK( cancel_button_callback )  /* ARGSUSED */
{
    cancel_resampling( (resample_volume_struct *) callback_data );
}

private  DEFINE_EVENT_FUNCTION( quit_window_callback )    /* ARGSUSED */
{
    cancel_resampling( (resample_volume_struct *) callback_data );
}

public  void  initialize_resample_popup(
    resample_volume_struct   *data,
    int                      x_position,
    int                      y_position,
    char                     filename[] )
{
    widget_struct   *widget;

    create_popup_window( &data->popup, filename,
                         x_position, y_position,
                         Resample_popup_x_size, Resample_popup_y_size,
                         quit_window_callback, (void *) data );

    initialize_meter( &data->popup, Resample_meter_x_size,
                      Resample_meter_y_size,
                      (Colour) Resample_meter_background_colour,
                      (Colour) Resample_meter_colour,
                      &data->meter_background,
                      &data->meter );

    widget = create_button( &data->popup.graphics,
                   0, Interface_x_spacing, Interface_y_spacing,
                   Button_width, Button_height,
                   "Cancel",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   cancel_button_callback, (void *) data );

    (void) add_widget_to_list( &data->popup.widgets, widget );
}

public  void  delete_resample_popup(
    resample_volume_struct   *data )
{
    delete_popup_window( &data->popup );
}

public  void  set_resample_popup_meter(
    resample_volume_struct   *data,
    Real                     fraction_done )
{
    set_meter_position( &data->popup, data->meter_background,
                        data->meter, fraction_done );
}
