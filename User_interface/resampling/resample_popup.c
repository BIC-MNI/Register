/**
 * \file User_interface/resampling/resample_popup.c
 * \brief Implement the popup window shown during resampling.
 *
 * \copyright
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <user_interface.h>

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( cancel_button_callback )
{
    cancel_resampling( (resample_volume_struct *) callback_data );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    cancel_resampling( (resample_volume_struct *) callback_data );
}

public  void  initialize_resample_popup(
    resample_volume_struct   *data,
    int                      x_position,
    int                      y_position,
    STRING                   filename )
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
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
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
    VIO_Real                     fraction_done )
{
    set_meter_position( &data->popup, data->meter_background,
                        data->meter, fraction_done );
}
